#define _USE_MATH_DEFINES
#include "RadarField.h"
#include "Plasmodium.h"
#include "Vec2.h"
#include "Vec2i.h"
#include <cmath>
#include <algorithm>
#include <iostream>


RadarField::RadarField(Plasmodium& plasmodium, int width, int height, float cellSize)
    : CollisionLogic(plasmodium, width, height, cellSize)
{}

std::vector<std::pair<float, float>> RadarField::getAvailableAngles(const Vec2& pos, float baseAngle) {
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

bool RadarField::LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold) {
    float cellSize = getCellSize();
    //int printEvery = 1000; 
    //int counter = 0;

    for (float i = 0.5f; i < range + 0.25f; i += cellSize * 0.5f) {
        Vec2 PointOnCollisionLine = pos + dir * i;
        mark(PointOnCollisionLine, 1.0f);
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

float RadarField::getCellVal(const Vec2i& gridPos) const {
    static constexpr int blockSize = GridBlock<Node*>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    auto it = RadarGrid.find({ blockX, blockY });
    if (it == RadarGrid.end()) return 0.f;
    return it->second.get(localX, localY);
}

float& RadarField::getCellRef(const Vec2i& gridPos) {
    static constexpr int blockSize = GridBlock<Node*>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    return RadarGrid[{blockX, blockY}].ref(localX, localY);
}

void RadarField::mark(const Vec2& pos, float value) {
    Vec2i gridPos = toGridCoords(pos);

    int gX = gridPos.getXi();
    int gY = gridPos.getYi();

    getCellRef(Vec2i(gX, gY)) = value;
}


const std::unordered_map<std::pair<int, int>, GridBlock<float>, PairHash>& RadarField::getRadarGrid() const {
    return RadarGrid;
}

void RadarField::clearGrid() {
    RadarGrid.clear();
}
