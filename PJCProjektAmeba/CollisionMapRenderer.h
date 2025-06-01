#pragma once
#include "IMapRenderer.h"

class CollisionMapRenderer : public IMapRenderer {
private:

public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};

