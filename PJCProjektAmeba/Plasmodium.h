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
    CollisionField colField;        
    FlowModel flowModel;
    std::mt19937 rng;

    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<Node*> EndingNodes;

    std::vector<std::unique_ptr<Tube>> tubes;

    // Parametry
    int growthPrecentage = 10;
    int skipThreshold = 0.05;
    double branchesDistribution[3] = { 50, 25, 25 };

public:
    Plasmodium(const Vec2& startPosition, int fieldWidth, int fieldHeight, float tubeLength, FoodField& foodField);

    void addStartStructure(const Vec2& centerPos, float radius, float tubeDiameter, float cytValue);
    void addGridStructure(float spacing, float tubeDiameter, float cytValue);

    // Rozrost
    void growOneStep(const FoodField& foodField);
    void sortEndings(const FoodField& foodField);
    float computeAngle(Node* parent);
    float generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, const Vec2& position);
    float normalizeAngle(float angle);

    void decreaseFailedGrowthFlag();
    int getActiveCount();
    bool shouldGrow();
    int howManyBranches();
    int leftOrRight();

    // Przep³yw
    void simulateFlow(float dt);

    const std::vector<std::unique_ptr<Node>>& getNodes() const;
    const std::vector<std::unique_ptr<Tube>>& getTubes() const;

    float getTubeLength() const;

    // Kolizja
    const CollisionField& getCollisionField() const;
};

