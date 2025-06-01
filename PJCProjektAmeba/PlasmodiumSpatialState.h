#pragma once
#include "IPlasmodiumController.h"
#include "CollisionField.h"
#include "RadarField.h"
#include "NodeField.h"

class PlasmodiumSpatialState {
private:
	IPlasmodiumController& pController;

	CollisionField colField;
	RadarField radarField;
	NodeField nodeField;

public:

	PlasmodiumSpatialState(IPlasmodiumController& pController, int fieldWidth, int fieldHeight, float tubeLength, float cellSizeCollision, float cellSizeNodes);

	void onNodeAdded(const Vec2& pos, Node* node);
	void onTubeAdded(const Vec2& posA, const Vec2& posB);

	void onStepMade();

	CollisionField& getCollisionField();
	const CollisionField& getCollisionField() const;

	RadarField& getRadarField();
	const RadarField& getRadarField() const;

	NodeField& getNodeField();
	const NodeField& getNodeField() const;


};
