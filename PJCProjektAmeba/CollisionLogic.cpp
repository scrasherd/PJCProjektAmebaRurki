#define _USE_MATH_DEFINES
#include "CollisionField.h"
#include "Vec2.h"
#include "Vec2i.h"
#include <cmath>
#include <algorithm>
#include <iostream>


CollisionLogic::CollisionLogic(int width, int height, float cellSize, float tubeLength)
    : Grid(width, height), cellSize(cellSize), tubeLength(tubeLength)
{}

float CollisionLogic::getCellSize() const {
    return cellSize;
}

float CollisionLogic::getTubeLength() const {
    return tubeLength;
}

float CollisionLogic::getDensity(const Vec2& pos) const {
    Vec2i gridPos = toGridCoords(pos);
    if (isInside(gridPos)) {
        return getCellVal(gridPos);
    }
    return 0.f;
}