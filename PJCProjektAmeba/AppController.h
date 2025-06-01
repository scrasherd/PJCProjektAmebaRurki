#pragma once
#include <SFML/Graphics.hpp>
#include "IMapRenderer.h"
#include "FoodField.h"
#include "RenderController.h"
#include "SimulationController.h"
#include <memory>

struct MapSize {
    int mapWidth;
    int mapHeight;
};

//Kontroler aplikacji
class AppController {
private:
    int mapWidth;
    int mapHeight;

    sf::RenderWindow window;

    FoodField foodField;

    std::unique_ptr<IPlasmodiumController> plasmodium;

    std::unique_ptr<RenderController> renderController;
    SimulationController simulationController;


    sf::Clock clock;
public:
    AppController(int mapWidth, int mapHeight, float tubeLength);
    void run();



};
