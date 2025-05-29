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
    radarField(*this, static_cast<int>(fieldHeight / 0.1f), static_cast<int>(fieldWidth / 0.1f), 0.1f),
    nodeField(*this, static_cast<int>(fieldHeight / 0.2f), static_cast<int>(fieldWidth / 0.2f), 0.2f),
    flowModel(*this, foodField),
    growthModel(*this, foodField),
    tubeLength(tubeLength) {
    
    //colField.mark(Vec2(0.f, 0.f));         
    //colField.mark(Vec2(999.9f, 999.9f));   
    //colField.mark(Vec2(500.f, 500.f));
    addStartStructure(startPosition, tubeLength, 1.0f /*œrednica*/, 1000.0f /*cytoplazma*/);
    //addGridStructure(tubeLength, 1.0f /* œrednica */, 500.0f /* cytoplazma */);
}

void Plasmodium::addStartStructure(const Vec2& centerPos, float radius, float tubeDiameter, float cytValue) {
    // Tworzenie wêz³a centralnego
    auto center = std::make_unique<Node>(centerPos);
    colField.markNode(centerPos);
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
        colField.markNode(outerPos);
        nodeField.addNodeToGrid(outerPos, outerPtr);

        auto tube = std::make_unique<Tube>(centerPtr, outerPtr, tubeDiameter, cytValue);
        tubes.push_back(std::move(tube));
        colField.markTube(outerPos, centerPos);

        endingNodes.push_back(outerPtr);
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
                endingNodes.push_back(current);
        }
    }
}





void Plasmodium::simulateGrowth() {
    growthModel.growOneStep();
}

std::vector<Node*> Plasmodium::sortEndings(const FoodField& foodField) {
    std::vector<Node*> sorted = getEndingNodes(); // zak³adamy: const std::vector<Node*>&

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

    return sorted;
}

void Plasmodium::assignRankingToNodes(const std::vector<Node*>& nodes) {
    if (nodes.size() <= 1) return;

    for (size_t i = 0; i < nodes.size(); ++i) {
        float rank = 1.0f - static_cast<float>(i) / (nodes.size() - 1);
        float value255 = rank * 255.0f;
        nodes[i]->setRankingValue(value255);
    }
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

const std::vector<Node*>& Plasmodium::getEndingNodes() const {
    return endingNodes;
}


const CollisionField& Plasmodium::getCollisionField() const {
    return colField;
}

RadarField& Plasmodium::getRadarField() {
    return radarField;
}

const RadarField& Plasmodium::getStaticRadarField() const {
    return radarField;
}

NodeField& Plasmodium::getNodeField() {
    return nodeField;
}

const NodeField& Plasmodium::getStaticNodeField() const {
    return nodeField;
}

float Plasmodium::getTubeLength() const {
    return tubeLength;
}
