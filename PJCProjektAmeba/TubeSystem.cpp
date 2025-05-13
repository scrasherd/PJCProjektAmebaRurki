#include "TubeSystem.h"
#include <memory>

TubeSystem::TubeSystem(int width, int height)
    : width(width), height(height),
    TubeMap(height, std::vector<Tube*>(width, nullptr)) {}

Tube* TubeSystem::getTube(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height)
        return nullptr;

    return TubeMap[y][x];
}

void TubeSystem::setTube(int x, int y, Tube* tube) {
    if (x < 0 || y < 0 || x >= width || y >= height)
        return;

    TubeMap[y][x] = tube;
}