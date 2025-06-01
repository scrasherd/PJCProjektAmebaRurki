#pragma once
#include "IMapRenderer.h"


class VectorMapRenderer : public IMapRenderer {
public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};

