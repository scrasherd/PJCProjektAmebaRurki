#pragma once
#include "Vec2.h"
#include "Vec2i.h"
#include "Grid.h"
#include <vector>
#include <utility>
#include <unordered_map>


class CollisionLogic : public Grid {
private:

    float cellSize;
    float tubeLength;

public:
    CollisionLogic(int width, int height, float cellSize, float tubeLength);

    float getCellSize() const override;
    float getTubeLength() const;

    virtual float& getCellRef(const Vec2i& gridPos) = 0;
    virtual float getCellVal(const Vec2i& gridPos) const = 0;

    virtual void mark(const Vec2& pos, float value) = 0;

    float getDensity(const Vec2& pos) const;

    virtual void clearGrid() = 0;
};
