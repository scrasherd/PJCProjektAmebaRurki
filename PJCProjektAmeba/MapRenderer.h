#pragma once
#include <SFML/Graphics.hpp>
#include "IPlasmodiumController.h"
#include "FigureMaker.h"

enum class RenderMode {
    Food,
    Vector,
    Pressure,
    Collision,
    Node
};

class MapRenderer {
public:
    MapRenderer() = default;
    virtual ~MapRenderer() = default;
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) = 0;

    static std::unique_ptr<MapRenderer> create(RenderMode mode);
};

