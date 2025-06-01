#include "FoodField.h"
#include <cmath>
#include <algorithm>
#include <iostream>

FoodField::FoodField(int width, int height)
    : Grid(width, height) {}


void FoodField::addSource(const Vec2& pos, float intensity, float radius) {
    sources.emplace_back(pos, intensity, radius);
}

void FoodField::clearGrid() {
    FoodGrid.clear();
}

float FoodField::getCellSize() const {
    return cellSize;
}

void FoodField::updateField() {
    FoodGrid.clear();

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
                    Vec2i gridPos(x, y);
                    float& cell = refCell(gridPos);
                    cell = std::min(cell + influence, 1.0f);
                }
            }
        }
    }
}

const std::vector<FoodSource>& FoodField::getSources() const {
    return sources;
}

float& FoodField::refCell(const Vec2i& gridPos) {

    constexpr int blockSize = GridBlock<float>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    return FoodGrid[{blockX, blockY}].ref(localX, localY);
}

float FoodField::getValueAt(const Vec2& pos) const {
    Vec2i gridPos = toGridCoords(pos);
    if (!isInside(gridPos)) return 0.0f;

    constexpr int blockSize = GridBlock<float>::getBlockSize();
    int blockX = gridPos.getXi() / blockSize;
    int blockY = gridPos.getYi() / blockSize;
    int localX = gridPos.getXi() % blockSize;
    int localY = gridPos.getYi() % blockSize;

    auto it = FoodGrid.find({ blockX, blockY });
    if (it == FoodGrid.end()) return 0.0f;

    return it->second.get(localX, localY);
}



 const Vec2 FoodField::getFoodGradient(Vec2 pos) const {
    int x = pos.getX();
    int y = pos.getY();

    float dx = getValueAt(Vec2(x + 1, y)) - getValueAt(Vec2(x - 1, y));
    float dy = getValueAt(Vec2(x, y + 1)) - getValueAt(Vec2(x, y - 1));

    return Vec2(dx, dy);
}