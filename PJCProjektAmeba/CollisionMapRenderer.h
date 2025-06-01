#pragma once
#include "IMapRenderer.h"

class CollisionMapRenderer : public IMapRenderer {
private:
    sf::RenderTexture cachedTexture;
    bool textureReady = false;

public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};

