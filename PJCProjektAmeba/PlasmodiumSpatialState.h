#pragma once
#include "IPlasmodiumController.h"
#include "IPlasmodiumObserver.h"
#include "CollisionField.h"
#include "RadarField.h"
#include "NodeField.h"

class PlasmodiumSpatialState : public IPlasmodiumObserver {
private:

	CollisionField colField;
	RadarField radarField;
	NodeField nodeField;

public:

	PlasmodiumSpatialState(int fieldWidth, int fieldHeight, float tubeLength, float cellSizeCollision, float cellSizeNodes);

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
