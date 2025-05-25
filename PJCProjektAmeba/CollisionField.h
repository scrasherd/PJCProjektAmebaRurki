#pragma once
#include "Vec2.h"
#include <vector>
#include <utility>

class Plasmodium;

class CollisionField {
private:
    Plasmodium& plasmodium;
    float cellSize;
    int width, height;
    std::vector<std::vector<float>> ColField;

public:
    CollisionField(Plasmodium& plasmodium, int width, int height, float cellSize);

    std::pair<int, int> toGridCoords(const Vec2& pos) const;

    void mark(const Vec2& pos, float value);
    //void markNode(const Vec2& pos, float value = 1.0f);
    void markTube(const Vec2& a, const Vec2& b);
    float getDensity(const Vec2& pos) const;

    void decay(float factor);
    void clear();

    float getCellSize() const;
    int getWidth() const;
    int getHeight() const;

    std::vector<std::pair<float, float>> getAvailableAngles(Vec2(pos), float baseAngle);
    bool LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold);
    bool isTubePathFree(const Vec2& from, const Vec2& to);
    float normalizeAngle(float angle);
};