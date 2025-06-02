#include "PlasmodiumController.h"

PlasmodiumController::PlasmodiumController(int width, int height, float tubeLen, const FoodField& foodField)
    : tubeLength(tubeLen),
    pData(),
    pSpatialState(width, height, tubeLen, cellSizeCollision, cellSizeNodes),
    foodField(foodField)
{
    growthModel = std::make_unique<RandomGrowthModel>(*this);
    flowModel = std::make_unique<FlowModel>(*this);
    addObserver(&pSpatialState);
}

void PlasmodiumController::update() {
    for (auto* obs : observers)
        obs->onStepMade();

    growthModel->simulate();
    flowModel->simulate();
}

void PlasmodiumController::addObserver(IPlasmodiumObserver* observer) {
    observers.push_back(observer);
}

const FoodField& PlasmodiumController::getFoodField() const {
    return foodField;
}

float PlasmodiumController::getTubeLength() {
    return tubeLength;
}

const std::vector<std::unique_ptr<Node>>& PlasmodiumController::getNodes() const {
    return pData.getNodes();
}

const std::vector<std::unique_ptr<Tube>>& PlasmodiumController::getTubes() const {
    return pData.getTubes();
}

const std::vector<Node*>& PlasmodiumController::getEndingNodes() const {
    return pData.getEndingNodes();
}

Node* PlasmodiumController::addNode(const Vec2& pos) {
    Node* node = pData.addNode(pos);
    for (auto* obs : observers)
        obs->onNodeAdded(pos, node);
    return node;
}

void PlasmodiumController::addTube(Node* a, Node* b, float CytValue) {
    pData.addTube(a, b, CytValue);

    for (auto* obs : observers)
        obs->onTubeAdded(a->getPosition(), b->getPosition());
}

void PlasmodiumController::informRadar(Vec2 pos, float angle) {
    for (auto* obs : observers)
        obs->onNodeAddedAttempt(pos, angle);
}

Node* PlasmodiumController::removeEndingNode(const Node* node) {
    return pData.removeEndingNode(node);
}

const CollisionField& PlasmodiumController::getCollisionField() const {
    return pSpatialState.getCollisionField();
}

const RadarField& PlasmodiumController::getRadarField() const {
    return pSpatialState.getRadarField();
}

const NodeField& PlasmodiumController::getNodeField() const {
    return pSpatialState.getNodeField();
}