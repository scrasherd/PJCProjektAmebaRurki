#pragma once
#include "Vec2.h"
#include "Vec2i.h"
#include "Grid.h"
#include <vector>
#include <utility>
#include <unordered_map>


class CollisionLogic : public Grid {
private:

    struct Block {
        float values[8][8]{};
    };

    float cellSize;

public:
    CollisionLogic(Plasmodium& plasmodium, int width, int height, float cellSize);

    float getCellSize() const override;

    virtual std::vector<std::pair<float, float>> getAvailableAngles(const Vec2& pos, float baseAngle) = 0;

    virtual bool LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold) = 0;

    virtual float& getCellRef(const Vec2i& gridPos) = 0;
    virtual float getCellVal(const Vec2i& gridPos) const = 0;

    virtual void mark(const Vec2& pos, float value) = 0;

    float getDensity(const Vec2& pos) const;

    virtual void clearGrid() = 0;
};
