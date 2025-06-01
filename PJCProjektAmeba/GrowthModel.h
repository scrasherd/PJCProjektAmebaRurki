#pragma once
#include "IPlasmodiumModel.h"
#include "Vec2.h"
#include "Node.h"
#include "IPlasmodiumController.h"
#include <random>

class GrowthModel : public IPlasmodiumModel {
private:
    
    IPlasmodiumController& pController;

    std::mt19937 rng;

    //Parametry
    float growthPrecentage = 15.f;
    float skipThreshold = 0.05f;
    double branchesDistribution[3] = { 50, 25, 25 };
    int timeoutFromGrowing = 10;
    float foodInfluence = 0.5f;
    float spread = 0.5f;

public:
    GrowthModel(IPlasmodiumController& pController);

    void simulate() override;

    void growOneStep();
    void growSingleNode(Node* parent, const std::vector<Node*>& endingNodes, const CollisionField& colField, const NodeField& nodeField, const FoodField& foodField, const std::vector<std::unique_ptr<Node>>& nodes, const std::vector<std::unique_ptr<Tube>>& tubes);
    float computeAngle(Node* parent);
    Node* generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, const NodeField& nodeField, const CollisionField& colField, const Vec2& pos);
    float normalizeAngle(float angle);

    void decreaseFailedGrowthFlag(const std::vector<Node*>& endingNodes);
    int getActiveCount(const std::vector<Node*>& endingNodes);
    bool shouldGrow();
    int howManyBranches();
    int leftOrRight();
    bool isAngleAllowed(float angle, const std::vector<std::pair<float, float>>& ranges);
    std::vector<Node*> sortEndings(const std::vector<Node*> endingNodes);
    void assignRankingToNodes(const std::vector<Node*>& nodes);

    void addStartStructure(const Vec2& centerPos, float radius, float cytValue);

    //bool isNodeA(Node* node, Tube* tube);
};