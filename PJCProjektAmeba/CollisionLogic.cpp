#define _USE_MATH_DEFINES
#include "CollisionField.h"
#include "Vec2.h"
#include "Vec2i.h"
#include <cmath>
#include <algorithm>
#include <iostream>


CollisionLogic::CollisionLogic(Plasmodium& plasmodium, int width, int height, float cellSize)
    : Grid(plasmodium, width, height), cellSize(cellSize)
{}

float CollisionLogic::getCellSize() const {
    return cellSize;
}

float CollisionLogic::getDensity(const Vec2& pos) const {
    Vec2i gridPos = toGridCoords(pos);
    if (isInside(gridPos)) {
        return getCellVal(gridPos);
    }
    return 0.f;
}