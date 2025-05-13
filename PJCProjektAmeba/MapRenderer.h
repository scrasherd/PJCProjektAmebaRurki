#pragma once
#include <SFML/Graphics.hpp>
#include "Plasmodium.h"

class MapRenderer {
public:
    virtual ~MapRenderer() = default;
    virtual void draw(sf::RenderWindow& window, const Plasmodium& plasmodium) = 0;
};

