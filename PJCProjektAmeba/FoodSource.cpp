#include "FoodSource.h"

FoodSource::FoodSource(const Vec2& pos, float intensity, float radius)
    : FoodSourcePosition(pos), Intensity(intensity), Radius(radius) {}

const Vec2& FoodSource::getFoodSourcePosition() const {
    return FoodSourcePosition;
}

float FoodSource::getIntensity() const {
    return Intensity;
}

float FoodSource::getRadius() const {
    return Radius;
}