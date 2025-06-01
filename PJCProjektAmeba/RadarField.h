#pragma once
#include "Vec2.h"
#include "Vec2i.h"
#include "CollisionLogic.h"
#include <vector>
#include <utility>
#include <unordered_map>


class RadarField : public CollisionLogic {
private:

    std::unordered_map<std::pair<int, int>, GridBlock<float>, PairHash> RadarGrid;

public:
    RadarField(int width, int height, float cellSize, float tubeLength);

    std::vector<std::pair<float, float>> getAvailableAnglesRadar(const Vec2& pos, float baseAngle);

    bool LineCollisionCheckRadar(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold);

    virtual float& getCellRef(const Vec2i& gridPos) override;
    virtual float getCellVal(const Vec2i& gridPos) const override;

    virtual void mark(const Vec2& pos, float value) override;

    const std::unordered_map<std::pair<int, int>, GridBlock<float>, PairHash>& getRadarGrid() const;

    virtual void clearGrid() override;
};
