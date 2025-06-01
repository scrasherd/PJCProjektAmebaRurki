#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "Grid.h"
#include "FoodSource.h"
#include <vector>

class FoodField : public Grid {
private:
    std::unordered_map<std::pair<int, int>, GridBlock<float>, PairHash> FoodGrid;
    std::vector<FoodSource> sources;
    float cellSize = 1.0f;

public:
    FoodField(int width, int height);

    float getCellSize() const override;

    void addSource(const Vec2& pos, float intensity, float radius);
    void updateField();

    const std::vector<FoodSource>& getSources() const;


    float& refCell(const Vec2i& gridPos);
    float getValueAt(const Vec2& pos) const;

    const Vec2 getFoodGradient(Vec2 pos) const;

    void clearGrid() override;
};