#include "AppController.h"
#include "PlasmodiumController.h"
#include <iostream>

AppController::AppController(int mapWidth, int mapHeight, float tubeLength) :
    window(sf::VideoMode({ static_cast<unsigned int>(mapWidth), static_cast<unsigned int>(mapHeight) }), "Plasmodium App"),
    foodField(mapHeight, mapWidth),
    plasmodium(std::make_unique<PlasmodiumController>(mapWidth, mapHeight, tubeLength, foodField)),
    renderer(MapRenderer::create(RenderMode::Food))
{
    view = window.getDefaultView();

    foodField.addSource(Vec2((mapHeight / 2.0f) + 50.0f, (mapWidth / 2.0f) + 50.0f), 1.0f, 100.0f);
    //foodField.addSource(Vec2((mapHeight / 2.0f) + 100.0f, (mapWidth / 2.0f) + 50.0f), 1.0f, 100.0f);

    //foodField.addSource(Vec2((mapHeight / 2.0f) - 50.0f, (mapWidth / 2.0f) - 50.0f), 1.0f, 100.0f);
    foodField.updateField();
 
}

void AppController::run() {

    bool Pause = true;

    static sf::Clock phaseClock;
    static bool growPhase = false;

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
                else if (key == sf::Keyboard::Key::Numpad5) {
                    currentMode = RenderMode::Node;
                    renderer = MapRenderer::create(currentMode);
                }
                else if (key == sf::Keyboard::Key::Space) {
                    Pause = !Pause;
                }
                else if (key == sf::Keyboard::Key::Enter) {
                    if (Pause) {
                        plasmodium->update();
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
            float elapsed = phaseClock.getElapsedTime().asSeconds();
            plasmodium->update();

            //if (!growPhase) {
            //    plasmodium.simulateFlow();

            //    if (elapsed >= 0.5f) {
            //        growPhase = true; // przechodzimy do fazy wzrostu
            //    }
            //}
            //else {
            //    
            //    plasmodium.simulateGrowth();
            //    

            //    // reset fazy
            //    phaseClock.restart();
            //    growPhase = false;
            //}

            //clock.restart();
        }

            window.clear();
            renderer->draw(window, *plasmodium);
            window.display();
    }
}
