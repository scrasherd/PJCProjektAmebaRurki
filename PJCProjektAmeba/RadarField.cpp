#define _USE_MATH_DEFINES
#include "RadarField.h"
#include "Node.h"
#include "Tube.h"
#include "Vec2.h"
#include "Vec2i.h"
#include <cmath>
#include <algorithm>
#include <iostream>


RadarField::RadarField(int width, int height, float cellSize, float tubeLength)
    : CollisionLogic(width, height, cellSize, tubeLength)
{}

std::vector<std::pair<float, float>> RadarField::getAvailableAnglesRadar(const Vec2& pos, float baseAngle) {
    const float length = getTubeLength();
    const float AngleStep = 1 / 40.f;
    const float CollisionThreshold = 0.1f;

    std::vector<std::pair<float, float>> AvailableRanges;
    bool InFreeRange = false;
    float StartRange = 0;

    for (float angle = baseAngle - M_PI_2; angle <= baseAngle + M_PI_2; angle += AngleStep) {

        Vec2 dir(std::cos(angle), std::sin(angle));
        bool Collision = LineCollisionCheckRadar(pos, dir, length, CollisionThreshold);

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

bool RadarField::LineCollisionCheckRadar(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold) {
    float cellSize = getCellSize();

    for (float i = 1.f; i < range + 0.25f; i += cellSize * 0.5f) {
        Vec2 PointOnCollisionLine = pos + dir * i;
        

        if (getDensity(PointOnCollisionLine) < CollisionThreshold) {
            mark(PointOnCollisionLine, 1.0f);
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
