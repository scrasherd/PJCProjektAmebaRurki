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
    RadarField(Plasmodium& plasmodium, int width, int height, float cellSize);

    virtual std::vector<std::pair<float, float>> getAvailableAngles(const Vec2& pos, float baseAngle) override;

    virtual bool LineCollisionCheck(const Vec2& pos, const Vec2& dir, float range, float CollisionThreshold) override;

    virtual float& getCellRef(const Vec2i& gridPos) override;
    virtual float getCellVal(const Vec2i& gridPos) const override;

    virtual void mark(const Vec2& pos, float value) override;

    const std::unordered_map<std::pair<int, int>, GridBlock<float>, PairHash>& getRadarGrid() const;

    virtual void clearGrid() override;
};
