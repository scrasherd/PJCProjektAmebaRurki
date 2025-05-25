#include "AppController.h"

AppController::AppController(int mapWidth, int mapHeight, float tubeLength) :
    window(sf::VideoMode({ static_cast<unsigned int>(mapWidth), static_cast<unsigned int>(mapHeight) }), "Plasmodium App"),
    foodField(mapHeight, mapWidth),
    plasmodium(Vec2(mapHeight / 2.0f, mapWidth / 2.0f), mapWidth, mapHeight, tubeLength, foodField),
    renderer(MapRenderer::create(RenderMode::Food))
{
    view = window.getDefaultView();

    foodField.addSource(Vec2((mapHeight / 2.0f) + 50.0f, (mapWidth / 2.0f) - 50.0f), 1.0f, 100.0f);
    //foodField.addSource(Vec2((mapHeight / 2.0f) - 50.0f, (mapWidth / 2.0f) - 50.0f), 1.0f, 100.0f);
    foodField.updateField();
}

void AppController::run() {
    bool Pause = true;

    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                window.close();


            //Zmiana trybu mapy

            if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>()->code;

                if (key == sf::Keyboard::Key::Numpad2) {
                    currentMode = RenderMode::Food;
                    renderer = MapRenderer::create(currentMode);
                }
                else if (key == sf::Keyboard::Key::Numpad1) {
                    currentMode = RenderMode::Vector;
                    renderer = MapRenderer::create(currentMode);
                }
                else if (key == sf::Keyboard::Key::Numpad3) {
                    currentMode = RenderMode::Pressure;
                    renderer = MapRenderer::create(currentMode);
                }
                else if (key == sf::Keyboard::Key::Numpad4) {
                    currentMode = RenderMode::Collision;
                    renderer = MapRenderer::create(currentMode);
                }
                else if (key == sf::Keyboard::Key::Space) {
                    Pause = !Pause;
                }
                else if (key == sf::Keyboard::Key::Enter) {
                    if (Pause) {
                        plasmodium.growOneStep(foodField);
                        plasmodium.simulateFlow(0.01f);
                    }
                }

            }

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

            if (!Pause && clock.getElapsedTime().asMilliseconds() > 1) {
                plasmodium.growOneStep(foodField);
                plasmodium.simulateFlow(0.01f);
                clock.restart();
            }

            window.clear();
            renderer->draw(window, plasmodium, foodField);
            window.display();
    }
}
