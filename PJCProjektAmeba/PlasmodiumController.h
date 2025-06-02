#pragma once
#include "IPlasmodiumController.h"
#include "PlasmodiumData.h"
#include "PlasmodiumSpatialState.h"
#include "IPlasmodiumObserver.h"
#include "GrowthModel.h"
#include "FlowModel.h"
#include "RandomGrowthModel.h"
#include "IGrowthModel.h"
#include "IFlowModel.h"
#include <vector>
#include <memory>


class PlasmodiumController : public IPlasmodiumController {
private:
    float tubeLength = 2.f;
    float cellSizeCollision = 0.1f;
    float cellSizeNodes = 0.3f;

    PlasmodiumData pData;

    std::vector<IPlasmodiumObserver*> observers;
    PlasmodiumSpatialState pSpatialState;

    std::unique_ptr<IGrowthModel> growthModel;
    std::unique_ptr<IFlowModel> flowModel;

    const FoodField& foodField;
public:
    PlasmodiumController(int width, int height, float tubeLength, const FoodField& foodField);

    void update() override;

    void addObserver(IPlasmodiumObserver* observer);

    const std::vector<std::unique_ptr<Node>>& getNodes() const override;
    const std::vector<std::unique_ptr<Tube>>& getTubes() const override;
    const std::vector<Node*>& getEndingNodes() const override;

    Node* addNode(const Vec2& pos) override;
    void addTube(Node* a, Node* b, float CytValue) override;
    Node* removeEndingNode(const Node* node)override;

    void informRadar(Vec2 pos, float angle) override;

    const FoodField& getFoodField() const override;

    float getTubeLength() override;

    const CollisionField& getCollisionField() const;
    const RadarField& getRadarField() const;
    const NodeField& getNodeField() const;
};
