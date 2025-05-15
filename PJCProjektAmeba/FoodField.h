#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include "FoodSource.h"
#include <vector>

class FoodField {
private:
    std::vector<std::vector<float>> field;
    std::vector<FoodSource> sources;
    int width, height;

public:
    FoodField(int width, int height);

    void addSource(const Vec2& pos, float intensity, float radius);
    void clearField();
    void updateField();

    const std::vector<FoodSource>& getSources() const;

 
    bool isInside(int x, int y) const;
    float getValueAt(int x, int y) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};