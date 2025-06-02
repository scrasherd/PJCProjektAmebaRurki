#pragma once
#include"IPlasmodiumController.h"

class IGrowthModel {
public:
    IGrowthModel(IPlasmodiumController& pController);

    virtual void simulate() = 0;

    virtual ~IGrowthModel() = default;
    virtual void growOneStep() = 0;
    
    virtual float computeAngle(Node* parent) = 0;
    
    virtual float normalizeAngle(float angle) = 0;

    virtual void decreaseFailedGrowthFlag(const std::vector<Node*>& endingNodes) = 0;
    virtual int getActiveCount(const std::vector<Node*>& endingNodes) = 0;
    virtual bool shouldGrow() = 0;
    virtual int howManyBranches() = 0;
    virtual int leftOrRight() = 0;
    virtual bool isAngleAllowed(float angle, const std::vector<std::pair<float, float>>& ranges) = 0;
    virtual std::vector<Node*> sortEndings(const std::vector<Node*> endingNodes) = 0;
    virtual void assignRankingToNodes(const std::vector<Node*>& nodes) = 0;

    virtual void addStartStructure(const Vec2& centerPos, float radius, float cytValue) = 0;
};

