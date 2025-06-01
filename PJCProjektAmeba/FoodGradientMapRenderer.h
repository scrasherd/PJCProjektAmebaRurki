#pragma once
#include "IMapRenderer.h"

class FoodGradientMapRenderer : public IMapRenderer {
public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};