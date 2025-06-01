#pragma once
#include<memory>
#include<vector>
#include"Vec2.h"

class Node;
class Tube;
class CollisionField;
class RadarField;
class NodeField;
class FoodField;

class IPlasmodiumController {
public:
    virtual void update() = 0;

    virtual const FoodField& getFoodField() const = 0;
    virtual float getTubeLength() = 0;

    virtual const std::vector<std::unique_ptr<Node>>& getNodes() const = 0;
    virtual const std::vector<std::unique_ptr<Tube>>& getTubes() const = 0;

    virtual const std::vector<Node*>& getEndingNodes() const = 0;

    virtual Node* addNode(const Vec2& pos) = 0 ;
    virtual void addTube(Node* a, Node* b, float CytValue) = 0;

    virtual Node* removeEndingNode(const Node* node) = 0;


    //virtual void notifyNodeAdded(Node* node) = 0;
    //virtual void notifyTubeAdded(Tube* tube) = 0;

    virtual ~IPlasmodiumController() = default;

    virtual const CollisionField& getCollisionField() const = 0;
    virtual const RadarField& getRadarField() const = 0;
    virtual const NodeField& getNodeField() const = 0;
};

