#include "FoodField.h"
#include <cmath>
#include <algorithm>
#include <iostream>

FoodField::FoodField(int width, int height)
    : width(width), height(height), field(height, std::vector<float>(width, 0.0f)) {}

void FoodField::addSource(const Vec2& pos, float intensity, float radius) {
    sources.emplace_back(pos, intensity, radius);
}

void FoodField::clearField() {
    for (auto& row : field) {
        std::fill(row.begin(), row.end(), 0.0f);
    }
}

void FoodField::updateField() {
    clearField();

    for (const auto& source : sources) {
        Vec2 pos = source.getFoodSourcePosition();
        float radius = source.getRadius();
        float intensity = source.getIntensity();

        // przejscie po zasiegu
        int minX = std::max(0, static_cast<int>(pos.getX() - radius));
        int maxX = std::min(width - 1, static_cast<int>(pos.getX() + radius));
        int minY = std::max(0, static_cast<int>(pos.getY() - radius));
        int maxY = std::min(height - 1, static_cast<int>(pos.getY() + radius));

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                float dx = x - pos.getX();
                float dy = y - pos.getY();
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist < radius) {
                    float influence = intensity * (1.0f - dist / radius);
                    field[y][x] = std::min(field[y][x] + influence, 1.0f);
                }
            }
        }
    }
}

const std::vector<FoodSource>& FoodField::getSources() const {
    return sources;
}

float FoodField::getValueAt(const Vec2& pos) const {
    if (isInside(pos)) {
        return field[pos.getY()][pos.getX()];
    }
    return 0.0f;
}

bool FoodField::isInside(const Vec2& pos) const {
    return (pos.getX() >= 0 && pos.getY() >= 0 && pos.getX() < width && pos.getY() < height);
}

 const Vec2 FoodField::getFoodGradient(Vec2 pos) const {
    int x = pos.getX();
    int y = pos.getY();

    float dx = getValueAt(Vec2(x + 1, y)) - getValueAt(Vec2(x - 1, y));
    float dy = getValueAt(Vec2(x, y + 1)) - getValueAt(Vec2(x, y - 1));

    return Vec2(dx, dy);
}