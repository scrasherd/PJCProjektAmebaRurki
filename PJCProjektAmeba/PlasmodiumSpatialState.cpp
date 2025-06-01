#include "PlasmodiumSpatialState.h"
#include "FoodField.h"

PlasmodiumSpatialState::PlasmodiumSpatialState(IPlasmodiumController& pController, int fieldWidth, int fieldHeight, float tubeLength, float cellSizeCollision, float cellSizeNodes) :
	pController(pController),
	colField(static_cast<int>(fieldWidth/cellSizeCollision), static_cast<int>(fieldHeight / cellSizeCollision), cellSizeCollision, tubeLength),
	radarField(static_cast<int>(fieldWidth / cellSizeCollision), static_cast<int>(fieldHeight / cellSizeCollision), cellSizeCollision, tubeLength),
	nodeField(static_cast<int>(fieldWidth / cellSizeNodes), static_cast<int>(fieldHeight / cellSizeNodes), cellSizeNodes) {
}

void PlasmodiumSpatialState::onNodeAdded(const Vec2& pos, Node* node) {
	colField.markNode(pos);
	nodeField.addNodeToGrid(pos, node);
}

void PlasmodiumSpatialState::onTubeAdded(const Vec2& posA, const Vec2& posB) {
	colField.markTube(posA, posB);
}

void PlasmodiumSpatialState::onStepMade() {
	radarField.clearGrid();
}

CollisionField& PlasmodiumSpatialState::getCollisionField() {
	return colField;
}

const CollisionField& PlasmodiumSpatialState::getCollisionField() const {
	return colField;
}

RadarField& PlasmodiumSpatialState::getRadarField() {
	return radarField;
}

const RadarField& PlasmodiumSpatialState::getRadarField() const {
	return radarField;
}

NodeField& PlasmodiumSpatialState::getNodeField() {
	return nodeField;
}

const NodeField& PlasmodiumSpatialState::getNodeField() const {
	return nodeField;
}
