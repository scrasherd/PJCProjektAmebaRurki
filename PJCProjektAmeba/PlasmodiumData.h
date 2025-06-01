#pragma once
#include "Node.h"
#include "Tube.h"
#include "IPlasmodiumObserver.h"
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

    const std::vector<std::unique_ptr<Node>>& getNodes() const;
    Node* addNode(const Vec2& pos);

    const std::vector<std::unique_ptr<Tube>>& getTubes() const;
    void addTube(Node* nodeA, Node* nodeB, float CytValue);

    const std::vector<Node*>& getEndingNodes() const;
    Node* removeEndingNode(const Node* node);
};

