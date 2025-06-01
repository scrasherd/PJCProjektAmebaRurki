#pragma once
#include "IMapRenderer.h"

class NodeGridMapRenderer : public IMapRenderer {
private:

public:
    virtual void draw(sf::RenderWindow& window, const IPlasmodiumController& pController) override;
};

