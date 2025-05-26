#define _USE_MATH_DEFINES
#include "CollisionField.h"
#include "Plasmodium.h"
#include "Vec2.h"
#include "Vec2i.h"
#include <cmath>
#include <algorithm>
#include <iostream>


CollisionField::CollisionField(Plasmodium& plasmodium, int width, int height, float cellSize)
    : plasmodium(plasmodium),
    cellSize(cellSize),
    width(width),
    height(height) {
    // sparseGrid pozostaje pusty do momentu pierwszego mark()
}

Vec2i CollisionField::toGridCoords(const Vec2& pos) const {
    int gX = static_cast<int>(std::floor(pos.getX() / cellSize));
    int gY = static_cast<int>(std::floor(pos.getY() / cellSize));

    return Vec2i(gX, gY);
}

bool CollisionField::isInside(const Vec2i& gridPos) const {
    if (gridPos.getXi() >= 0 && gridPos.getYi() >= 0 && gridPos.getXi() < width && gridPos.getYi() < height) {
        return true;
    }

    return false;
}

std::vector<std::pair<float, float>> CollisionField::getAvailableAngles(Vec2(pos), float baseAngle) {
        const float length = plasmodium.getTubeLength();
        const float AngleStep = 1 / 40.f;
        const float CollisionThreshold = 0.5f;
    
        std::vector<std::pair<float, float>> AvailableRanges;
        bool InFreeRange = false;
        float StartRange = 0;
    
        for (float angle = baseAngle - M_PI_2; angle <= baseAngle + M_PI_2; angle += AngleStep) {
            
            Vec2 dir(std::cos(angle), std::sin(angle));
            bool Collision = LineCollisionCheck(pos, dir, length, CollisionThreshold);
    
            if (!Collision) {
                if (!InFreeRange) {
                    StartRange = angle;
                    InFreeRange = true;
                }
            }
            else {
                if (InFreeRange) {
                    AvailableRanges.emplace_back(StartRange, angle);
                    InFreeRange = false;
                }
            }
        }
    
        if (InFreeRange) {
            AvailableRanges.emplace_back(StartRange, baseAngle + M_PI_2);
        }
    
        return AvailableRanges;
 }

bool CollisionField::LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold) {

    int printEvery = 1000; // co który punkt drukowaæ
    int counter = 0;

    for (float i = 0.5f; i < range + 0.25f; i += cellSize * 0.5f) {
        Vec2 PointOnCollisionLine = pos + dir * i;
        markRadar(PointOnCollisionLine, 1.0f);

        //if (counter % printEvery == 0) {
        //    std::cout << "Sprawdzam punkt: x=" << PointOnCollisionLine.getX()
        //        << ", y=" << PointOnCollisionLine.getY() << std::endl;
        //}
        //counter++;

        if (getDensity(PointOnCollisionLine) > CollisionThreshold) { 
            std::cout << "blok"; 
            return true; 
        }
    }
    return false;
}

float CollisionField::getDensity(const Vec2& pos) const {
    Vec2i gridPos = toGridCoords(pos);
    if (isInside(gridPos)) {
        return getCellVal(gridPos);
    }
    return 0.f;
}

float CollisionField::getCellVal(const Vec2i& gridPos) const {
    int blockSize = 8;
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    auto it = sparseGrid.find({ blockX, blockY });
    if (it == sparseGrid.end()) return 0.f;
    return it->second.values[localX][localY];
}

float& CollisionField::getCellRef(const Vec2i& gridPos) {
    int blockSize = 8;
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    return sparseGrid[{blockX, blockY}].values[localX][localY];
}

float CollisionField::getCellValRadar(const Vec2i& gridPos) const {
    int blockSize = 8;
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    auto it = RadarGrid.find({ blockX, blockY });
    if (it == RadarGrid.end()) return 0.f;
    return it->second.values[localX][localY];
}

float& CollisionField::getCellRefRadar(const Vec2i& gridPos) {
    int blockSize = 8;
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    return RadarGrid[{blockX, blockY}].values[localX][localY];
}


void CollisionField::markTube(const Vec2& a, const Vec2& b) {
    float length = (b - a).length();

    // Ka¿dy krok to pó³ komórki – gwarantuje przejœcie przez ka¿d¹
    int steps = std::max(1, static_cast<int>(length / (cellSize * 0.5f)));

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        Vec2 point = a + (b - a) * t;
        mark(point,1.0f);
    }
}

void CollisionField::markNode(const Vec2& pos, float value) {
    Vec2i gridPos = toGridCoords(pos);

    int gX = gridPos.getXi();
    int gY = gridPos.getYi();
    const int radius = 3;

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy <= radius * radius) { // warunek okrêgu
                int nx = gX + dx;
                int ny = gY + dy;
                if (nx >= 0 && ny >= 0) {
                    getCellRef(Vec2i(nx, ny)) = value;
                }
            }
        }
    }
}

void CollisionField::mark(const Vec2& pos, float value) {
    Vec2i gridPos = toGridCoords(pos);

    int gX = gridPos.getXi();
    int gY = gridPos.getYi();

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = gX + dx;
            int ny = gY + dy;
            if (nx >= 0 && ny >= 0) {
                getCellRef(Vec2i(nx,ny)) = value;
            }
        }
    }
}

void CollisionField::markRadar(const Vec2& pos, float value) {
    Vec2i gridPos = toGridCoords(pos);

    int gX = gridPos.getXi();
    int gY = gridPos.getYi();

    getCellRefRadar(Vec2i(gX, gY)) = value;
}

float CollisionField::getCellSize() const {
    return cellSize;
}

int CollisionField::getWidth() const {
    return width;
}

int CollisionField::getHeight() const {
    return height;
}

const std::unordered_map<std::pair<int, int>, Block, PairHash>& CollisionField::getSparseGrid() const {
    return sparseGrid;
}

const std::unordered_map<std::pair<int, int>, Block, PairHash>& CollisionField::getRadarGrid() const {
    return RadarGrid;
}

void CollisionField::clearRadarGrid() {
    RadarGrid.clear();
}
