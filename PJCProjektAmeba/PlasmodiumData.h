#pragma once
#include "Node.h"
#include "Tube.h"
#include <vector>
#include <memory>
#include <random>

class PlasmodiumData {

private:

    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<Node*> endingNodes;
    std::vector<std::unique_ptr<Tube>> tubes;


public:
    PlasmodiumData();

    //void addStartStructure(const Vec2& centerPos, float radius, float tubeDiameter, float cytValue);
    //void addGridStructure(float spacing, float tubeDiameter, float cytValue);


    const std::vector<std::unique_ptr<Node>>& getNodes() const;
    Node* addNode(const Vec2& pos);
    //void removeNode(Node* node);

    const std::vector<std::unique_ptr<Tube>>& getTubes() const;
    void addTube(Node* nodeA, Node* nodeB, float CytValue);
    //void removeTube(Tube* tube);

    const std::vector<Node*>& getEndingNodes() const;
    Node* removeEndingNode(const Node* node);
};

