#pragma once
#include "MapRenderer.h"

class VectorMapRenderer : public MapRenderer {
public:
    void draw(sf::RenderWindow& window, const Plasmodium& plasmodium) override;
};

