#define _USE_MATH_DEFINES
#include "Grid.h"
#include "Vec2.h"
#include "Vec2i.h"
#include <cmath>
#include <algorithm>
#include <iostream>


Grid::Grid(int width, int height)
    :
    width(width),
    height(height) {
}

Vec2i Grid::toGridCoords(const Vec2& pos) const {
    float cellSize = getCellSize();
    int gX = static_cast<int>(std::floor(pos.getX() / cellSize));
    int gY = static_cast<int>(std::floor(pos.getY() / cellSize));

    return Vec2i(gX, gY);
}

bool Grid::isInside(const Vec2i& gridPos) const {
    if (gridPos.getXi() >= 0 && gridPos.getYi() >= 0 && gridPos.getXi() < width && gridPos.getYi() < height) {
        return true;
    }

    return false;
}

int Grid::getWidth() const {
    return width;
}

int Grid::getHeight() const {
    return height;
}




