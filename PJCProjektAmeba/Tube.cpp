#include "Tube.h"
#include "Node.h"

Tube::Tube(Node* a, Node* b, float diameter, float CytValue)
    : NodeA(a), NodeB(b), TubeDiameter(diameter), dP(0.0f), dPnorm(0.0f), CytoplasmAmount(CytValue){
    a->connectTube(this);
    b->connectTube(this);
}

const Node* Tube::getNodeA() const {
    return NodeA;
}

const Node* Tube::getNodeB() const {
    return NodeB;
}

const Node* Tube::getOtherNode(const Node* node) const{
    if (node == getNodeA()) {
        return getNodeB();
    }
    else {
        return getNodeA();
    }
}

float Tube::getDiameter() const {
    return TubeDiameter;
}

Vec2 Tube::getDirection() const {
    return (NodeA->getPosition() - NodeB->getPosition()).normalized();
}

float Tube::getLength() const {
    return (NodeB->getPosition() - NodeA->getPosition()).length();
}

float Tube::getdP() {
    return dP;
}

void Tube::setdP(float value) {
    dP = value;
}

float Tube::getdPnorm() {
    return dPnorm;
}

void Tube::setdPnorm(float value) {
    dPnorm = value;
}

float Tube::getCytoplasmAmount() {
    return CytoplasmAmount;
}

void Tube::setCytoplasmAmount(float value) {
    CytoplasmAmount = value;
}
