#pragma once
#include <SFML/Graphics.hpp>
#include "Plasmodium.h"
#include "MapRenderer.h"
#include <memory>

class AppController {
private:
    int mapWidth;
    int mapHeight;

    sf::RenderWindow window;
    sf::View view;

    Plasmodium plasmodium;
    std::unique_ptr<MapRenderer> renderer;

    sf::Clock clock;

public:
    AppController(int mapWidth, int mapHeight, float tubeLength);
    void run();

    bool isDragging = false;
    sf::Vector2i lastMousePos;

};
