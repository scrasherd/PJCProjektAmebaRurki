#define _USE_MATH_DEFINES
#include "CollisionField.h"
#include "Vec2.h"
#include "Vec2i.h"
#include <cmath>
#include <algorithm>
#include <iostream>


CollisionField::CollisionField(int width, int height, float cellSize, float tubeLength)
    : CollisionLogic(width, height, cellSize, tubeLength)
{}


const std::vector<std::pair<float, float>> CollisionField::getAvailableAngles(const Vec2& pos, float baseAngle) const {
        const float length = getTubeLength();
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

const bool CollisionField::LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold) const{
    float cellSize = getCellSize();
    //int printEvery = 1000; 
    //int counter = 0;

    for (float i = 0.5f; i < range + 0.25f; i += cellSize * 0.5f) {
        Vec2 PointOnCollisionLine = pos + dir * i;

        //if (counter % printEvery == 0) {
        //    std::cout << "Sprawdzam punkt: x=" << PointOnCollisionLine.getX()
        //        << ", y=" << PointOnCollisionLine.getY() << std::endl;
        //}
        //counter++;

        if (getDensity(PointOnCollisionLine) > CollisionThreshold) { 
            return true; 
        }
    }
    return false;
}

float CollisionField::getCellVal(const Vec2i& gridPos) const {
    int blockSize = GridBlock<float>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    auto it = CollisionGrid.find({ blockX, blockY });
    if (it == CollisionGrid.end()) return 0.f;
    return it->second.get(localX,localY);
}

float& CollisionField::getCellRef(const Vec2i& gridPos) {
    int blockSize = GridBlock<float>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    return CollisionGrid[{blockX, blockY}].ref(localX,localY);
}

void CollisionField::markTube(const Vec2& a, const Vec2& b) {
    float cellSize = getCellSize();
    float length = (b - a).length();

    int steps = std::max(1, static_cast<int>(length / (cellSize * 0.5f)));

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        Vec2 point = a + (b - a) * t;
        mark(point,1.0f);
    }
}

void CollisionField::markNode(const Vec2& pos) {
    Vec2i gridPos = toGridCoords(pos);

    int gX = gridPos.getXi();
    int gY = gridPos.getYi();
    const int radius = 3;

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy <= radius * radius) {
                int nx = gX + dx;
                int ny = gY + dy;
                if (nx >= 0 && ny >= 0) {
                    getCellRef(Vec2i(nx, ny)) = 1.0f;
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

const std::unordered_map<std::pair<int, int>, GridBlock<float>, PairHash>& CollisionField::getSparseGrid() const {
    return CollisionGrid;
}

void CollisionField::clearGrid() {
    CollisionGrid.clear();
}
