#pragma once
#include "Vec2.h"
#include "Vec2i.h"
#include <vector>
#include <utility>
#include <unordered_map>

class Plasmodium;

struct Block {
    float values[8][8]{};
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

class CollisionField {
private:

    Plasmodium& plasmodium;
    float cellSize;
    int width, height;
    std::unordered_map<std::pair<int, int>, Block, PairHash> sparseGrid;
    std::unordered_map<std::pair<int, int>, Block, PairHash> RadarGrid;

public:
    CollisionField(Plasmodium& plasmodium, int width, int height, float cellSize);

    Vec2i toGridCoords(const Vec2& pos) const;

    bool isInside(const Vec2i& gridPos) const;

    std::vector<std::pair<float, float>> getAvailableAngles(Vec2(pos), float baseAngle);

    bool LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold);

    float getDensity(const Vec2& pos) const;

    float& getCellRef(const Vec2i& pos);
    float getCellVal(const Vec2i& posy) const;

    float& getCellRefRadar(const Vec2i& pos);
    float getCellValRadar(const Vec2i& posy) const;

    void mark(const Vec2& pos, float value);
    void markTube(const Vec2& a, const Vec2& b);
    void markNode(const Vec2& pos, float value);

    void markRadar(const Vec2& pos, float value);

    //void decay(float factor);
    //void clear();

    float getCellSize() const;
    int getWidth() const;
    int getHeight() const;
    const std::unordered_map<std::pair<int, int>, Block, PairHash>& getSparseGrid() const;
    const std::unordered_map<std::pair<int, int>, Block, PairHash>& getRadarGrid() const;

    void clearRadarGrid();

    //std::vector<std::pair<float, float>> getAvailableAngles(Vec2(pos), float baseAngle);
    //bool LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold);
    //bool isTubePathFree(const Vec2& from, const Vec2& to);
    //float normalizeAngle(float angle);

    //float& getCellRef(int x, int y);
    //float getCellVal(int x, int y) const;

    //const std::unordered_map<std::pair<int, int>, Block, PairHash>& getSparseGrid() const;
};