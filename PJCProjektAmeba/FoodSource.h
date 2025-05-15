#pragma once
#include "Vec2.h"
#include <vector>


class FoodSource{

private:
	Vec2 FoodSourcePosition;
	float Intensity;
	float Radius;

public:
	FoodSource(const Vec2& pos, float intensity, float radius);

	const Vec2& getFoodSourcePosition() const;

	float getIntensity() const;
	float getRadius() const;

};
