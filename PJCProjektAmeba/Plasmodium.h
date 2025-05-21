#pragma once
#include "FoodField.h"
#include "CollisionField.h"
#include "FlowModel.h"
#include <vector>
#include <memory>
#include <random>
#include "Node.h"
#include "Tube.h"

class Plasmodium {

private:
    int fieldWidth, fieldHeight;
    float tubeLength;
    CollisionField colField;         // ? jako obiekt, nie wskaünik
    FlowModel flowModel;
    std::mt19937 rng;

    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<Node*> EndingNodes;

    std::vector<std::unique_ptr<Tube>> tubes;

public:
    Plasmodium(const Vec2& startPosition, int fieldWidth, int fieldHeight, float tubeLength, FoodField& foodField);

    void addStartStructure(const Vec2& centerPos, float radius, float tubeDiameter, float cytValue);

    // Rozrost
    void growOneStep(const FoodField& foodField);
    void sortEndings(const FoodField& foodField);
    float computeAngle(Node* parent);
    float generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, const Vec2& position);

    // Przep≥yw
    void simulateFlow(float dt);

    const std::vector<std::unique_ptr<Node>>& getNodes() const;
    const std::vector<std::unique_ptr<Tube>>& getTubes() const;

    float getTubeLength() const;

    // Kolizja
    const CollisionField& getCollisionField() const;
};

