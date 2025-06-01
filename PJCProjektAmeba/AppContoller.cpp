#include "AppController.h"
#include "PlasmodiumController.h"
#include "RenderController.h"
#include "MapRendererRegistry.h"

AppController::AppController(int mapWidth, int mapHeight, float tubeLength)
    : mapWidth(mapWidth), mapHeight(mapHeight),
    window(sf::VideoMode({ static_cast<unsigned int>(mapWidth), static_cast<unsigned int>(mapHeight) }), "Plasmodium App"),
    foodField(mapHeight, mapWidth),
    plasmodium(std::make_unique<PlasmodiumController>(mapWidth, mapHeight, tubeLength, foodField)),
    renderController(std::make_unique<RenderController>(RenderMode::FoodGradientMapRenderer, window))
{
    foodField.addSource(Vec2((mapHeight / 2.0f) + 50.0f, (mapWidth / 2.0f) + 50.0f), 1.0f, 100.0f);
    foodField.updateField();
}

void AppController::run() {
    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            renderController->handleEvent(*event);

            if (event->is<sf::Event::KeyPressed>()) {
                const auto key = event->getIf<sf::Event::KeyPressed>()->code;

                if (key == sf::Keyboard::Key::Space) {
                    simulationController.togglePause();
                }
                else if (key == sf::Keyboard::Key::Enter) {
                    simulationController.stepOnce(*plasmodium);
                }
            }
        }

        simulationController.updateIfRunning(*plasmodium, clock);

        window.clear();
        renderController->draw(window, *plasmodium);
        window.display();
    }
}
