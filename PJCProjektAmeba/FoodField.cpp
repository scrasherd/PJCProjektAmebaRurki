#include "FoodField.h"
#include <cmath>
#include <algorithm>

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

        // Przechodzimy tylko po obszarze w zasiêgu Ÿród³a
        int minX = std::max(0, static_cast<int>(pos.x - radius));
        int maxX = std::min(width - 1, static_cast<int>(pos.x + radius));
        int minY = std::max(0, static_cast<int>(pos.y - radius));
        int maxY = std::min(height - 1, static_cast<int>(pos.y + radius));

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                float dx = x - pos.x;
                float dy = y - pos.y;
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

float FoodField::getValueAt(int x, int y) const {
    if (isInside(x, y)) {
        return field[y][x];
    }
    return 0.0f;
}

bool FoodField::isInside(int x, int y) const {
    return (x >= 0 && y >= 0 && x < width && y < height);
}