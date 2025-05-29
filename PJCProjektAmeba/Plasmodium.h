#pragma once
#include "FoodField.h"
#include "CollisionField.h"
#include "RadarField.h"
#include "NodeField.h"
#include "FlowModel.h"
#include "GrowthModel.h"
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
    RadarField radarField;
    NodeField nodeField;

    FlowModel flowModel;
    GrowthModel growthModel;

    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<Node*> endingNodes;
    std::vector<std::unique_ptr<Tube>> tubes;


public:
    Plasmodium(const Vec2& startPosition, int fieldWidth, int fieldHeight, float tubeLength, FoodField& foodField);

    void addStartStructure(const Vec2& centerPos, float radius, float tubeDiameter, float cytValue);
    void addGridStructure(float spacing, float tubeDiameter, float cytValue);

    //void Live();

    // Rozrost
    void simulateGrowth();

    // Przep³yw
    void simulateFlow(float dt);

    const std::vector<std::unique_ptr<Node>>& getStaticNodes() const;
    const std::vector<std::unique_ptr<Tube>>& getStaticTubes() const;
    const std::vector<Node*>& getStaticEndingNodes() const;

    const std::vector<std::unique_ptr<Node>>& getNodes() const;
    void addNode(const Node node);
    void removeNode(Node* node);

    const std::vector<std::unique_ptr<Tube>>& getTubes() const;
    void addTube(const Tube tube);
    void removeTube(Tube* tube);

    const std::vector<Node*>& getEndingNodes() const;
    void addEndingNode(const Node node);
    void removeEndingNode(Node* node);
    std::vector<Node*> sortEndings(const FoodField& foodField);
    void assignRankingToNodes(const std::vector<Node*>& nodes);

    float getTubeLength() const;

    // Kolizja
    CollisionField& getCollisionField();
    const CollisionField& getStaticCollisionField() const;
    RadarField& getRadarField();
    const RadarField& getStaticRadarField() const;
    NodeField& getNodeField();
    const NodeField& getStaticNodeField() const;
};

