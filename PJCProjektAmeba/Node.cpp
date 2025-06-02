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

const float Node::getPressure() const {
    return pressure;
}

void Node::setPressure(float p) {
    if (p > 0.f && p < 3.f ) {
        pressure = p;
    };
}

const float Node::getPhase() const{
    return phase;
}

void Node::setPhase(float value) {
    phase = value;
}

int Node::getFailedGrowthFlag() {
    return FailedGrowthFlag;
}

void Node::setFailedGrowthFlag(int value) {
    FailedGrowthFlag = value;
}

void Node::decreaseFailedGrowthFlag() {
    if (FailedGrowthFlag > 0) {
        FailedGrowthFlag--;
    }
}

float Node::getRankingValue() {
    return RankingValue;
}

void Node::setRankingValue(float value) {
    RankingValue = value;
}