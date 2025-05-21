#define _USE_MATH_DEFINES
#include "CollisionField.h"
#include "Plasmodium.h"
#include "Vec2.h"
#include <cmath>
#include <algorithm>
#include <iostream>

CollisionField::CollisionField(Plasmodium& plasmodium, int width, int height, float cellSize)
    : plasmodium(plasmodium), cellSize(cellSize), width(width), height(height),
    ColField(height, std::vector<float>(width, 0.0f)) {
}

std::pair<int, int> CollisionField::toGridCoords(const Vec2& pos) const {
    int gx = static_cast<int>(std::floor(pos.getX() / cellSize));
    int gy = static_cast<int>(std::floor(pos.getY() / cellSize));
    //std::cout << "toGridCoords: pos=(" << pos.getX() << "," << pos.getY()
    //    << ") -> grid=(" << gx << "," << gy << ")\n";
    return { gy, gx };
}


void CollisionField::mark(const Vec2& pos) {
    auto [gy, gx] = toGridCoords(pos);
    //std::cout << "toGridCoords: pos=(" << pos.getX() << "," << pos.getY()
    //    << ") -> grid=(" << gx << "," << gy << ")\n";
    if (gx >= 0 && gy >= 0 && gx < width && gy < height) {
        ColField[gy][gx] = 1.0f;
        std::cout << "Marking cell (" << gx << ", " << gy << ") = " << ColField[gy][gx] << std::endl;
    }
}

void CollisionField::markTube(const Vec2& a, const Vec2& b) {
    float length = (b - a).length();

    // Ka¿dy krok to pó³ komórki – gwarantuje przejœcie przez ka¿d¹
    int steps = std::max(1, static_cast<int>(length / (cellSize * 0.5f)));

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        Vec2 point = a + (b - a) * t;
        mark(point);
    }
}


float CollisionField::getDensity(const Vec2& pos) const {
    //std::cout << "siema";
    auto [gy, gx] = toGridCoords(pos);
    if (gx >= 0 && gy >= 0 && gx < (width) && gy < (height)) {
        return ColField[gy][gx];
    }
    return 0.f;
}

void CollisionField::decay(float factor) {
    for (auto& column : ColField) {
        for (float& val : column) {
            val *= factor;
        }
    }
}

void CollisionField::clear() {
    for (auto& column : ColField) {
        std::fill(column.begin(), column.end(), 0.0f);
    }
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

std::vector<std::pair<float, float>> CollisionField::getAvailableAngles(Vec2(pos), float baseAngle) const {
    const float length = plasmodium.getTubeLength() / cellSize;
    const float AngleStep = 1 / length;
    const float CollisionThreshold = 0.9f;

    std::vector<std::pair<float, float>> AvailableRenges;
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
                AvailableRenges.emplace_back(StartRange, angle);
                InFreeRange = false;
            }
        }
    }

    return AvailableRenges;
}

bool CollisionField::LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold) const {

    for (float i = 0; i < range; i += cellSize * 0.5f) {
        Vec2 PointOnCollisionLine = pos + dir * i;
        if (getDensity(PointOnCollisionLine) > CollisionThreshold) { return true;}
    }
    return false;
}
