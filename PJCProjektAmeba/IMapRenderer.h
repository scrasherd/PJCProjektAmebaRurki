#pragma once
#include <SFML/Graphics.hpp>
#include "IPlasmodiumController.h"
#include "FigureMaker.h"

enum class RenderMode {
    FoodGradientMapRenderer,
    VectorMapRenderer,
    PressureMapRenderer,
    CollisionMapRenderer,
    NodeGridMapRenderer
};

class IMapRenderer {
public:
    IMapRenderer() = default;
    virtual ~IMapRenderer() = default;
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) = 0;
};

