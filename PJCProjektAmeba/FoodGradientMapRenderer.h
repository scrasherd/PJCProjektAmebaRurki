#pragma once
#include "MapRenderer.h"

class FoodGradientMapRenderer : public MapRenderer {
public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};