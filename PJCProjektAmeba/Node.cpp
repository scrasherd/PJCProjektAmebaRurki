#include "Node.h"
#include "Tube.h"

Node::Node(const Vec2& pos) : NodePosition(pos) {}

const Vec2& Node::getPosition() const {
    return NodePosition;
}

void Node::connectTube(Tube* tube) {
    ConnectedTubes.push_back(tube);
}

const std::vector<Tube*>& Node::getConnectedTubes() const {
    return ConnectedTubes;
}
