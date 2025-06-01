#pragma once
#include <SFML/Graphics.hpp>
#include "IPlasmodiumController.h"
#include "IMapRenderer.h"
#include "FoodField.h"
#include "CollisionField.h"
#include <memory>

class AppController {
private:
    int mapWidth;
    int mapHeight;

    sf::RenderWindow window;
    sf::View view;

    FoodField foodField;
    std::unique_ptr<IPlasmodiumController> plasmodium;

    RenderMode currentMode = RenderMode::Food;
    std::unique_ptr<MapRenderer> renderer;




    sf::Clock clock;

public:
    AppController(int mapWidth, int mapHeight, float tubeLength);
    void run();

    bool isDragging = false;
    sf::Vector2i lastMousePos;

};
