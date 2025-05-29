#define _USE_MATH_DEFINES
#include "GrowthModel.h"
#include "Plasmodium.h"
#include "Tube.h"
#include "Node.h"
#include <vector>
#include <iostream>

GrowthModel::GrowthModel(Plasmodium& owner, FoodField& food) : plasmodium(owner), foodField(food) 
{
    std::random_device rd;
    rng.seed(rd());
}

void GrowthModel::growOneStep() {
    

    auto& endingNodes = plasmodium.getEndingNodes();
    auto& colField = plasmodium.getCollisionField();
    auto& radarField = plasmodium.getRadarField();
    auto& nodeField = plasmodium.getNodeField();
    auto& nodes = plasmodium.getNodes();
    auto& tubes = plasmodium.getTubes();

    if (endingNodes.empty()) return;

    radarField.clearGrid();

    sortEndings(foodField, endingNodes);

    int activeCount = getActiveCount(endingNodes);
    std::cout << activeCount << std::endl;

    for (int i = 0; i < activeCount; i++) {

        Node* parent = endingNodes[i];

        if (shouldGrow()) {
            growSingleNode(parent, endingNodes, colField, nodeField, nodes, tubes);
        }

    }

    decreaseFailedGrowthFlag(endingNodes);

}

void GrowthModel::growSingleNode(Node* parent, std::vector<Node*>& endingNodes, CollisionField& colField, NodeField& nodeField, std::vector<std::unique_ptr<Node>>& nodes, std::vector<std::unique_ptr<Tube>>& tubes) {
    Vec2 parentPos = parent->getPosition();

    int branchesCount = howManyBranches();

    int prefSide = 0;
    if (branchesCount > 1) prefSide = leftOrRight();

    float baseAngle = computeAngle(parent);
    int attempts = 0;

    while (attempts < branchesCount) {
        float newAngle = generateAngle(baseAngle, attempts, prefSide, foodField, colField, parentPos);

        if (newAngle > -9.0f) {
            float tubeLength = plasmodium.getTubeLength();
            //std::cout << newAngle << std::endl;

            Vec2 dir(std::cos(newAngle), std::sin(newAngle));
            Vec2 newPos = parentPos + dir * tubeLength;

            auto newNode = std::make_unique<Node>(newPos);
            Node* newNodePtr = newNode.get();
            auto newTube = std::make_unique<Tube>(parent, newNode.get(), 1.0f, 0.0f);

            endingNodes.push_back(newNode.get());
            colField.markNode(newPos);
            nodeField.addNodeToGrid(newPos, newNodePtr);
            nodes.push_back(std::move(newNode));

            colField.markTube(parentPos, newPos);
            tubes.push_back(std::move(newTube));

            auto it = std::find(endingNodes.begin(), endingNodes.end(), parent);
            if (it != endingNodes.end()) {
                endingNodes.erase(it);
            }

        }
        else {
            parent->setFailedGrowthFlag(timeoutFromGrowing);
        }

        ++attempts;
    }
}

void GrowthModel::sortEndings(const FoodField& foodField, const std::vector<Node*>& endingNodes) {
    std::sort(endingNodes.begin(), endingNodes.end(),
        [&](Node* A, Node* B) {
            int failA = A->getFailedGrowthFlag();
            int failB = B->getFailedGrowthFlag();

            if (failA != failB)
                return failA < failB; // mniejsze flagi na górze

            float valueA = foodField.getValueAt(A->getPosition());
            float valueB = foodField.getValueAt(B->getPosition());

            return valueA > valueB; // wiêksze wartoœci jedzenia na górze
        });

    //Kolor ranking do wizualizacji
    int total = static_cast<int>(endingNodes.size());
    if (total == 0) return;

    for (int i = 0; i < total; ++i) {
        float rank = 1.0f - static_cast<float>(i) / (total - 1);
        float value255 = rank * 255.0f;
        endingNodes[i]->setRankingValue(value255);
    }
}



float GrowthModel::computeAngle(Node* parent) {
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

Node* GrowthModel::generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, CollisionField& colField, const Vec2& pos) {
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
    float angle = normalizeAngle(angleDist(rng));

    auto availableRanges = colField.getAvailableAngles(pos, baseAngle);
    //std::cout << "Base angle: " << baseAngle << "\n";
    //std::cout << "Available ranges (base angle: " << baseAngle << "):\n";
    //for (const auto& range : availableRanges) {
    //    std::cout << "  [" << range.first << ", " << range.second << "]\n";
    //}

    if (availableRanges.empty()) return nullptr;

    auto bestRange = availableRanges.front();
    float bestDistance = std::abs(normalizeAngle(meanAngle - (bestRange.first + bestRange.second) * 0.5f));

    for (const auto& range : availableRanges) {
        float center = (range.first + range.second) * 0.5f;
        float dist = std::abs(meanAngle - center);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestRange = range;
        }
    }

    float clampedMean = std::clamp(meanAngle, bestRange.first, bestRange.second);
    float effectiveSpread = (bestRange.second - bestRange.first) * spread;
    std::normal_distribution<float> angleDist(clampedMean, effectiveSpread);

    const int maxTries = 10;
    for (int i = 0; i < maxTries; ++i) {
        float candidate = normalizeAngle(angleDist(rng));
        float normStart = normalizeAngle(bestRange.first);
        float normEnd = normalizeAngle(bestRange.second);

        if ((normStart < normEnd && candidate >= normStart && candidate <= normEnd) ||
            (normStart > normEnd && (candidate >= normStart || candidate <= normEnd))) {
            newAngle = candidate;

            // DEBUG: wypisz wybrany nowy k¹t
            //std::cout << "Generated new angle: " << newAngle << "\n";
            return newAngle;
        }
    }

    return -10.f;

}

float GrowthModel::normalizeAngle(float angle) {
    while (angle <= -M_PI) angle += 2.0f * M_PI;
    while (angle > M_PI)  angle -= 2.0f * M_PI;
    return angle;
}

void GrowthModel::decreaseFailedGrowthFlag(std::vector<Node*>& endingNodes) {
    for (Node* node : endingNodes) {
        int current = node->getFailedGrowthFlag();
        if (current > 0)
            node->setFailedGrowthFlag(current - 1);
    }
}


int GrowthModel::getActiveCount(std::vector<Node*>& endingNodes) {
    int total = static_cast<int>(endingNodes.size());
    int activeCount = std::max(1.f, total * (growthPrecentage / 100.f));
    return activeCount;
}

int GrowthModel::howManyBranches() {
    std::discrete_distribution<int> branchDist({ branchesDistribution[0], branchesDistribution[1], branchesDistribution[2] });
    int newBranches = branchDist(rng) + 1;
    return newBranches;
}

bool GrowthModel::shouldGrow() {
    std::uniform_real_distribution<float> skipChance(0.0f, 1.0f);
    bool shouldGrow = skipChance(rng) >= 0.1f;
    return shouldGrow;
}

int GrowthModel::leftOrRight() {
    int direction = 0;
    std::uniform_int_distribution<int> DirectionDist(0, 1);
    direction = DirectionDist(rng) == 0 ? 1 : -1;
    return direction;
}