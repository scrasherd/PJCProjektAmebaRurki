#define _USE_MATH_DEFINES
#include "RandomGrowthModel.h"
#include "Tube.h"
#include "Node.h"
#include "FoodField.h"
#include "CollisionField.h"
#include "RadarField.h"
#include "NodeField.h"
#include <vector>
#include <iostream>

RandomGrowthModel::RandomGrowthModel(IPlasmodiumController& pController) : pController(pController)
{
    std::random_device rd;
    rng.seed(rd());
    Vec2 centerPos(500.f, 500.f);

    addStartStructure(centerPos, 2.0f, 1000.f);
}

void RandomGrowthModel::simulate() {
    growOneStep();
}

void RandomGrowthModel::growOneStep() {

    auto& rawEndingNodes = pController.getEndingNodes();
    auto endingNodes = sortEndings(rawEndingNodes);

    auto& nodes = pController.getNodes();
    auto& tubes = pController.getTubes();

    auto& colField = pController.getCollisionField();
    auto& nodeField = pController.getNodeField();

    if (endingNodes.empty()) return;

    int activeCount = getActiveCount(endingNodes);

    for (int i = 0; i < activeCount; i++) {

        Node* parent = endingNodes[i];

        if (shouldGrow()) {
            growSingleNode(parent, endingNodes, colField, nodeField, nodes, tubes);
        }

    }

    decreaseFailedGrowthFlag(endingNodes);

}

void RandomGrowthModel::growSingleNode(Node* parent, const std::vector<Node*>& endingNodes, const CollisionField& colField, const NodeField& nodeField, const std::vector<std::unique_ptr<Node>>& nodes, const std::vector<std::unique_ptr<Tube>>& tubes) {
    Vec2 parentPos = parent->getPosition();

    int branchesCount = howManyBranches();

    int prefSide = 0;
    if (branchesCount > 1) prefSide = leftOrRight();

    float baseAngle = computeAngle(parent);
    int attempts = 0;


    while (attempts < branchesCount) {
        Node* node = generateAngle(baseAngle, attempts, prefSide, nodeField, colField, parentPos);

        if (node) {

            pController.addTube(parent, node, 0.0f);
            pController.removeEndingNode(parent);
        }
        else {
            parent->setFailedGrowthFlag(timeoutFromGrowing);
        }

        ++attempts;
    }
}


float RandomGrowthModel::computeAngle(Node* parent) {
    float baseAngle = 0;

    if (parent->getConnectedTubes().empty()) {
        // brak rurek losuj dowolny k¹t
        std::uniform_real_distribution<float> angleDist(0.0f, 2 * M_PI);
        baseAngle = angleDist(rng);
    }
    else {
        // kierunek ostatniej rurki
        const Tube* tube = parent->getConnectedTubes()[0];
        const Node* neighbor = (tube->getNodeA() == parent) ? tube->getNodeB() : tube->getNodeA();
        Vec2 lastDir = (parent->getPosition() - neighbor->getPosition()).normalized();
        baseAngle = std::atan2(lastDir.getY(), lastDir.getX());
    }

    return baseAngle;
}

Node* RandomGrowthModel::generateAngle(float baseAngle, int generated, int direction, const NodeField& nodeField, const CollisionField& colField, const Vec2& pos) {
    float tubeLength = pController.getTubeLength();
    float meanAngle = baseAngle;
    float newAngle = 0;

    if (generated == 0) {
        meanAngle = baseAngle;
    }
    else if (generated == 1) {
        meanAngle = baseAngle + direction * M_PI * 3.f / 8.f;
    }
    else if (generated == 2) {
        meanAngle = baseAngle - direction * M_PI * 3.f / 8.f;
    }

    Vec2 FoodGradient = foodField.getFoodGradient(pos);

    if (FoodGradient.length() > 0.001f) {
        FoodGradient = FoodGradient.normalized();
        float foodAngle = std::atan2(FoodGradient.getY(), FoodGradient.getX());

        // przesun k¹t o ileœ procent w strone jedzenia
        meanAngle = std::lerp(meanAngle, foodAngle, foodInfluence);
    }

    std::normal_distribution<float> angleDist(meanAngle, spread);
    newAngle = normalizeAngle(angleDist(rng));

    auto availableRanges = colField.getAvailableAngles(pos, baseAngle);

    if (availableRanges.empty()) return nullptr;

    Vec2 dir(std::cos(newAngle), std::sin(newAngle));
    Vec2 newPos = pos + dir * tubeLength;

    if (isAngleAllowed(newAngle, availableRanges)) {
        return pController.addNode(newPos);
    }

    if (!isAngleAllowed(newAngle, availableRanges)) {
        return nodeField.findNodeToConnect(pos, newPos, newAngle, tubeLength);
    }

}

float RandomGrowthModel::normalizeAngle(float angle) {
    while (angle <= -M_PI) angle += 2.0f * M_PI;
    while (angle > M_PI)  angle -= 2.0f * M_PI;
    return angle;
}

void RandomGrowthModel::decreaseFailedGrowthFlag(const std::vector<Node*>& endingNodes) {
    for (Node* node : endingNodes) {
        node->decreaseFailedGrowthFlag();
    }
}

bool RandomGrowthModel::isAngleAllowed(float angle, const std::vector<std::pair<float, float>>& ranges) {
    for (const auto& [start, end] : ranges) {
        if (start <= end) {
            if (angle >= start && angle <= end)
                return true;
        }
        else {
            if (angle >= start || angle <= end)
                return true;
        }
    }
    return false;
}

int RandomGrowthModel::getActiveCount(const std::vector<Node*>& endingNodes) {
    int total = static_cast<int>(endingNodes.size());
    int activeCount = std::max(1.f, total * (growthPrecentage / 100.f));
    return activeCount;
}

int RandomGrowthModel::howManyBranches() {
    std::discrete_distribution<int> branchDist({ branchesDistribution[0], branchesDistribution[1], branchesDistribution[2] });
    int newBranches = branchDist(rng) + 1;
    return newBranches;
}

bool RandomGrowthModel::shouldGrow() {
    std::uniform_real_distribution<float> skipChance(0.0f, 1.0f);
    bool shouldGrow = skipChance(rng) >= 0.1f;
    return shouldGrow;
}

int RandomGrowthModel::leftOrRight() {
    int direction = 0;
    std::uniform_int_distribution<int> DirectionDist(0, 1);
    direction = DirectionDist(rng) == 0 ? 1 : -1;
    return direction;
}

std::vector<Node*> RandomGrowthModel::sortEndings(const std::vector<Node*> endingNodes) {
    auto& foodField = pController.getFoodField();
    std::vector<Node*> sorted = endingNodes;

    std::sort(sorted.begin(), sorted.end(),
        [&](Node* A, Node* B) {
            int failA = A->getFailedGrowthFlag();
            int failB = B->getFailedGrowthFlag();

            if (failA != failB)
                return failA < failB;

            float valueA = foodField.getValueAt(A->getPosition());
            float valueB = foodField.getValueAt(B->getPosition());

            return valueA > valueB;
        });
    assignRankingToNodes(sorted);

    return sorted;
}

void RandomGrowthModel::assignRankingToNodes(const std::vector<Node*>& nodes) {
    if (nodes.size() <= 1) return;

    for (size_t i = 0; i < nodes.size(); ++i) {
        float rank = 1.0f - static_cast<float>(i) / (nodes.size() - 1);
        float value255 = rank * 255.0f;
        nodes[i]->setRankingValue(value255);
    }
}

void GrowthModel::addStartStructure(const Vec2& centerPos, float radius, float cytValue) {
    // Tworzenie wêz³a centralnego
    auto center = pController.addNode(centerPos);

    for (int i = 0; i < 3; ++i) {
        float angle = i * (2.0f * 3.1415926f / 3.0f); // 0, 120°, 240°
        Vec2 offset{ radius * std::cos(angle), radius * std::sin(angle) };
        Vec2 outerPos = center->getPosition() + offset;

        auto outer = pController.addNode(outerPos);

        pController.addTube(center, outer, cytValue);
    }

}