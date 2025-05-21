#pragma once
#include "MapRenderer.h"

class CollisionMapRenderer : public MapRenderer {
private:
    sf::RenderTexture cachedTexture;
    bool textureReady = false;

public:
    virtual void draw(sf::RenderWindow& window, const Plasmodium& plasmodium, const FoodField& foodField) override;
};

