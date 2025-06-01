#pragma once
#include "IMapRenderer.h"

class PressureMapRenderer : public IMapRenderer {
public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};