#pragma once
#include "MapRenderer.h"

class PressureMapRenderer : public MapRenderer {
public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};