#pragma once
#include <SFML/Graphics.hpp>
#include "Plasmodium.h"
#include "FoodField.h"
#include "FigureMaker.h"

enum class RenderMode {
    Food,
    Vector,
    Pressure,
};

class MapRenderer {
public:
    virtual ~MapRenderer() = default;
    virtual void draw(sf::RenderWindow& window, const Plasmodium& plasmodium, const FoodField& foodField) = 0;

    static std::unique_ptr<MapRenderer> create(RenderMode mode);
};

