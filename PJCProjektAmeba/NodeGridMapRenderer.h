#pragma once
#include "MapRenderer.h"

class NodeGridMapRenderer : public MapRenderer {
private:
    sf::RenderTexture cachedTexture;
    bool textureReady = false;

public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};

