#include "AppController.h"
#include "VectorMapRenderer.h"

AppController::AppController(int mapWidth, int mapHeight, float tubeLength): 
    window(sf::VideoMode({ static_cast<unsigned int>(mapWidth), static_cast<unsigned int>(mapHeight) }), "Plasmodium App"),
    plasmodium(Vec2(mapHeight/2.0f, mapHeight/2.0f), tubeLength),
    renderer(std::make_unique<VectorMapRenderer>()) 
{
    view = window.getDefaultView();
}

void AppController::run() {
    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            //Zoom
            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheelScrolled->delta > 0)
                    view.zoom(0.9f); // przybli¿enie
                else if (mouseWheelScrolled->delta < 0)
                    view.zoom(1.1f); // oddalenie

                window.setView(view);
            }

            //Przeci¹ganie
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    isDragging = true;
                    lastMousePos = sf::Mouse::getPosition(window);
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>()) {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>();
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    isDragging = false;
                }
            }

            if (event->is<sf::Event::MouseMoved>() && isDragging) {
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                sf::Vector2f delta = window.mapPixelToCoords(lastMousePos) - window.mapPixelToCoords(currentMousePos);
                view.move(delta);
                window.setView(view);
                lastMousePos = currentMousePos;
            }


        }

        if (clock.getElapsedTime().asMilliseconds() > 1) {
            plasmodium.growOneStep();
            clock.restart();
        }

        window.clear();
        renderer->draw(window, plasmodium);
        window.display();
    }
}
