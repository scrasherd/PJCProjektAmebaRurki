#define _USE_MATH_DEFINES
#include "Plasmodium.h"
#include "Tube.h"
#include "Node.h"
#include "FoodField.h"
#include "CollisionField.h"
#include "math.h"
#include <cmath>
#include <memory>
#include <iostream>

Plasmodium::Plasmodium(const Vec2& startPosition, int fieldWidth, int fieldHeight, float tubeLength, FoodField& foodField)
    : colField(*this, static_cast<int>(fieldHeight / 0.1f), static_cast<int>(fieldWidth / 0.1f), 0.1f),
    flowModel(*this, foodField),
    rng(std::random_device{}()),
    tubeLength(tubeLength) {
    
    //colField.mark(Vec2(0.f, 0.f));         
    //colField.mark(Vec2(999.9f, 999.9f));   
    //colField.mark(Vec2(500.f, 500.f));
    addStartStructure(startPosition, tubeLength, 1.0f /*œrednica*/, 1000.0f /*cytoplazma*/);
    //addGridStructure(tubeLength, 1.0f /* œrednica */, 500.0f /* cytoplazma */);
}

Vec2 randomDirection(std::mt19937& rng) {
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
    float angle = angleDist(rng);
    return Vec2(std::cos(angle), std::sin(angle));
}

void Plasmodium::addStartStructure(const Vec2& centerPos, float radius, float tubeDiameter, float cytValue) {
    // Tworzenie wêz³a centralnego
    auto center = std::make_unique<Node>(centerPos);
    colField.markNode(centerPos, 1.0f);
    Node* centerPtr = center.get();
    nodes.push_back(std::move(center));

    // Rozmieszczanie 3 wêz³ów na okrêgu wokó³ centrum (k¹ty co 120 stopni)
    for (int i = 0; i < 3; ++i) {
        float angle = i * (2.0f * 3.1415926f / 3.0f); // 0, 120°, 240°
        Vec2 offset{ radius * std::cos(angle), radius * std::sin(angle) };
        Vec2 outerPos = centerPtr->getPosition() + offset;

        auto outer = std::make_unique<Node>(outerPos);
        Node* outerPtr = outer.get();
        nodes.push_back(std::move(outer));
        colField.markNode(outerPos, 1.0f);

        auto tube = std::make_unique<Tube>(centerPtr, outerPtr, tubeDiameter, cytValue);
        tubes.push_back(std::move(tube));
        colField.markTube(outerPos, centerPos);

        EndingNodes.push_back(outerPtr);
    }

    // Dodanie centralnego wêz³a jako potencjalnego zakoñczenia
}

void Plasmodium::addGridStructure(float spacing, float tubeDiameter, float cytValue) {
    int cols = 300;
    int rows = 300;

    std::vector<std::vector<Node*>> grid(rows, std::vector<Node*>(cols, nullptr));

    Vec2 origin(400.f, 400.f);  // Lewy górny róg siatki

    // Tworzenie wêz³ów
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Vec2 pos = origin + Vec2(x * spacing, y * spacing);
            auto node = std::make_unique<Node>(pos);
            Node* nodePtr = node.get();
            grid[y][x] = nodePtr;
            nodes.push_back(std::move(node));
        }
    }

    // Tworzenie rurek miêdzy s¹siednimi wêz³ami
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Node* current = grid[y][x];

            // Po³¹czenie z prawym s¹siadem
            if (x + 1 < cols) {
                Node* right = grid[y][x + 1];
                auto tube = std::make_unique<Tube>(current, right, tubeDiameter, cytValue);
                colField.markTube(current->getPosition(), right->getPosition());
                tubes.push_back(std::move(tube));
            }

            // Po³¹czenie z dolnym s¹siadem
            if (y + 1 < rows) {
                Node* down = grid[y + 1][x];
                auto tube = std::make_unique<Tube>(current, down, tubeDiameter, cytValue);
                colField.markTube(current->getPosition(), down->getPosition());
                tubes.push_back(std::move(tube));
            }

            // Dodaj tylko krawêdziowe jako EndingNode
            if (x == cols - 1 || y == rows - 1)
                EndingNodes.push_back(current);
        }
    }
}







void Plasmodium::growOneStep(const FoodField& foodField) {
    colField.clearRadarGrid(); // czyœæ pole radaru

    if (EndingNodes.empty()) return;

    sortEndings(foodField);

    int activeCount = getActiveCount();

    for (int i = 0; i < activeCount; i++) {

        Node* parent = EndingNodes[i];

        if (shouldGrow) {

            int branchesCount = howManyBranches();


        }

    }

}

void Plasmodium::sortEndings(const FoodField& foodField) {
    std::sort(EndingNodes.begin(), EndingNodes.end(),
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
    int total = static_cast<int>(EndingNodes.size());
    if (total == 0) return;

    for (int i = 0; i < total; ++i) {
        float rank = 1.0f - static_cast<float>(i) / (total - 1);
        float value255 = rank * 255.0f;
        EndingNodes[i]->setRankingValue(value255);
    }
}



float Plasmodium::computeAngle(Node* parent) {
    float baseAngle = 0;

    if (parent->getConnectedTubes().empty()) {
        // Brak rurek – losuj dowolny k¹t
        std::uniform_real_distribution<float> angleDist(0.0f, 2 * M_PI);
        baseAngle = angleDist(rng);
    }else {
        // Kierunek ostatniej rurki
        Tube* tube = parent->getConnectedTubes()[0];
        Node* neighbor = (tube->getNodeA() == parent) ? tube->getNodeB() : tube->getNodeA();
        Vec2 lastDir = (parent->getPosition() - neighbor->getPosition()).normalized();
        baseAngle = std::atan2(lastDir.getY(), lastDir.getX());
    }

    return baseAngle;
}

float Plasmodium::generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, const Vec2& position) {

}

float Plasmodium::normalizeAngle(float angle) {
    while (angle <= -M_PI) angle += 2.0f * M_PI;
    while (angle > M_PI)  angle -= 2.0f * M_PI;
    return angle;
}

void Plasmodium::decreaseFailedGrowthFlag() {
    for (Node* node : EndingNodes) {
        int current = node->getFailedGrowthFlag();
        if (current > 0)
            node->setFailedGrowthFlag(current - 1);
    }
}

int Plasmodium::getActiveCount() {
    int total = static_cast<int>(EndingNodes.size());
    int activeCount = std::max(1, total * (growthPrecentage/100));
    return activeCount;
}

int Plasmodium::howManyBranches() {
    std::discrete_distribution<int> branchDist({ branchesDistribution[0], branchesDistribution[1], branchesDistribution[2] });
    int newBranches = branchDist(rng) + 1;
    return newBranches;
}

bool Plasmodium::shouldGrow() {
    std::uniform_real_distribution<float> skipChance(0.0f, 1.0f);
    bool shouldGrow = skipChance(rng) >= 0.1f;
    return shouldGrow;
}

void Plasmodium::simulateFlow(float dt) {
    flowModel.updatePhasesAndPressures(dt);
    flowModel.computeFlow();
}


const std::vector<std::unique_ptr<Node>>& Plasmodium::getNodes() const {
    return nodes;
}

const std::vector<std::unique_ptr<Tube>>& Plasmodium::getTubes() const {
    return tubes;
}

const CollisionField& Plasmodium::getCollisionField() const {
    return colField;
}

float Plasmodium::getTubeLength() const {
    return tubeLength;
}
