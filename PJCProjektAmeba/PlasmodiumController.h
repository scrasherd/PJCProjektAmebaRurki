#pragma once
#include "IPlasmodiumController.h"
#include "PlasmodiumData.h"
#include "PlasmodiumSpatialState.h"
#include "GrowthModel.h"
#include "FlowModel.h"
#include "FoodField.h"
#include <vector>
#include <memory>


class PlasmodiumController : public IPlasmodiumController {
private:
    int fieldWidth = 1000;
    int fieldHeight = 1000;
    float tubeLength = 2.f;
    float cellSizeCollision = 0.1f;
    float cellSizeNodes = 0.3f;

    PlasmodiumData pData;

    PlasmodiumSpatialState pSpatialState;

    GrowthModel growthModel;
    FlowModel flowModel;

    const FoodField& foodField;

    //std::vector<std::unique_ptr<IPlasmodiumModel>> models;

public:
    PlasmodiumController(int width, int height, float tubeLength, const FoodField& foodField);

    void update() override; // g³ówny krok symulacji

    const std::vector<std::unique_ptr<Node>>& getNodes() const override;
    const std::vector<std::unique_ptr<Tube>>& getTubes() const override;
    const std::vector<Node*>& getEndingNodes() const override;

    Node* addNode(const Vec2& pos) override;
    void addTube(Node* a, Node* b, float CytValue) override;

    Node* removeEndingNode(const Node* node)override;

    // IPlasmodiumMediator:
   // PlasmodiumData& getData() override;

    const FoodField& getFoodField() const override;

    float getTubeLength() override;

    //void notifyNodeAdded(Node* node) override;
    //void notifyTubeAdded(Tube* tube) override;

    //void registerModel(std::unique_ptr<IPlasmodiumModel> model);
    //void registerFieldProjector(std::unique_ptr<IFieldProjector> projector);

    const CollisionField& getCollisionField() const;
    const RadarField& getRadarField() const;
    const NodeField& getNodeField() const;
};
