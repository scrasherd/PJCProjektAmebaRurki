#define _USE_MATH_DEFINES
#include "PlasmodiumData.h"
#include "Tube.h"
#include "Node.h"
#include "CollisionField.h"
#include "math.h"
#include <cmath>
#include <memory>
#include <iostream>
#include "PlasmodiumSpatialState.h"

PlasmodiumData::PlasmodiumData(){};

Node* PlasmodiumData::addNode(const Vec2& pos) {
    auto node = std::make_unique<Node>(pos);
    Node* rawPtr = node.get();               // zapamiêtaj wskaŸnik
    nodes.push_back(std::move(node));        // przenieœ do wektora
    endingNodes.push_back(rawPtr);

    return rawPtr;                           // zwróæ wskaŸnik
}

void PlasmodiumData::addTube(Node* nodeA, Node* nodeB, float CytValue) {
    auto newTube = std::make_unique<Tube>(nodeA, nodeB, 1.0f, CytValue);
    tubes.push_back(std::move(newTube));        // przenieœ do wektora      

}
 
Node* PlasmodiumData::removeEndingNode(const Node* node) {
    auto it = std::find(endingNodes.begin(), endingNodes.end(), node);
    if (it != endingNodes.end()) {
        Node* rawPtr = *it;
        endingNodes.erase(it);
        return rawPtr;
    }
    return nullptr;
}


const std::vector<std::unique_ptr<Node>>& PlasmodiumData::getNodes() const {
    return nodes;
}

const std::vector<std::unique_ptr<Tube>>& PlasmodiumData::getTubes() const {
    return tubes;
}

const std::vector<Node*>& PlasmodiumData::getEndingNodes() const {
    return endingNodes;
}
