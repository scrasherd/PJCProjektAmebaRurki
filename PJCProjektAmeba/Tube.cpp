#include "Tube.h"
#include "Node.h"

Tube::Tube(Node* a, Node* b, float diameter, bool isEnding)
    : NodeA(a), NodeB(b), TubeDiameter(diameter), isEnding(isEnding) {
    a->connectTube(this);
    b->connectTube(this);
}

Node* Tube::getNodeA() const {
    return NodeA;
}

Node* Tube::getNodeB() const {
    return NodeB;
}

float Tube::getDiameter() const {
    return TubeDiameter;
}

bool Tube::getIsEnding() const {
    return isEnding;
}

void Tube::setIsEnding(bool value) {
    isEnding = value;
}

Vec2 Tube::direction() const {
    return (NodeA->getPosition() - NodeB->getPosition()).normalized();
}

float Tube::length() const {
    return (NodeB->getPosition() - NodeA->getPosition()).length();
}
