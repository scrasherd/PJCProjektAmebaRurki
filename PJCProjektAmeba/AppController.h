#pragma once
#include <SFML/Graphics.hpp>
#include "IMapRenderer.h"
#include "FoodField.h"
#include "RenderController.h"
#include "SimulationController.h"
#include <memory>

//Kontroler aplikacji
class AppController {
private:
    int mapWidth;
    int mapHeight;

    sf::RenderWindow window;

    FoodField foodField;

    std::unique_ptr<IPlasmodiumController> plasmodium;

    RenderMode currentMode = RenderMode::FoodGradientMapRenderer;
    std::unique_ptr<IMapRenderer> renderer;

    std::unique_ptr<RenderController> renderController;
    SimulationController simulationController;


    sf::Clock clock;
    bool isDragging = false;
    sf::Vector2i lastMousePos;

public:
    AppController(int mapWidth, int mapHeight, float tubeLength);
    void run();



};
