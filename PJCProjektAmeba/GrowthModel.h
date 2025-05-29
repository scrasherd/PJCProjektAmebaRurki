#pragma once
#include <vector>
#include <random>
#include "Vec2.h"
#include "CollisionField.h"
#include "NodeField.h"
class Plasmodium;
class FoodField;
class Node;
class Tube;

class GrowthModel {
private:
    Plasmodium& plasmodium;
    FoodField& foodField;

    std::mt19937 rng;

    //Parametry
    float growthPrecentage = 15.f;
    int skipThreshold = 0.05;
    double branchesDistribution[3] = { 50, 25, 25 };
    int timeoutFromGrowing = 10;
    float foodInfluence = 0.5f;
    float spread = 0.5f;

public:
    GrowthModel(Plasmodium& owner, FoodField& foodField);

    void growOneStep();
    void growSingleNode(Node* parent, std::vector<Node*>& endingNodes, CollisionField& colField, NodeField& nodeField, std::vector<std::unique_ptr<Node>>& nodes, std::vector<std::unique_ptr<Tube>>& tubes);
    void sortEndings(const FoodField& foodField, const std::vector<Node*>& endingNodes);
    float computeAngle(Node* parent);
    Node* generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, CollisionField& colField, const Vec2& pos);
    float normalizeAngle(float angle);

    void decreaseFailedGrowthFlag(std::vector<Node*>& endingNodes);
    int getActiveCount(std::vector<Node*>& endingNodes);
    bool shouldGrow();
    int howManyBranches();
    int leftOrRight();



    //bool isNodeA(Node* node, Tube* tube);
};