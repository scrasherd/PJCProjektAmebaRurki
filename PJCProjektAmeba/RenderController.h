#pragma once
#include <SFML/Graphics.hpp>
#include "IMapRenderer.h"
#include "MapRendererRegistry.h"
#include "IMapRenderer.h"

class RenderController {
private:
    RenderMode currentMode;
    std::unique_ptr<IMapRenderer> renderer;

    sf::View view;
    sf::RenderWindow& window;

    bool isDragging = false;
    sf::Vector2i lastMousePos;

    void setMode(RenderMode mode);

    //void setMode(RenderMode mode);

public:
    RenderController(RenderMode initialMode, sf::RenderWindow& window);


    //RenderMode getCurrentMode() const;

    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window, const IPlasmodiumController& pController);


};

