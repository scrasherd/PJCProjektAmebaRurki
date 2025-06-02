#include "RenderController.h"
#include "SFML/System.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>


RenderController::RenderController(RenderMode initialMode, sf::RenderWindow& window)
    : currentMode(initialMode), window(window)
{
    renderer = MapRendererRegistry::create(initialMode);
    view = window.getDefaultView();
    window.setView(view);
}

void RenderController::setMode(RenderMode mode) {
    currentMode = mode;
    renderer = MapRendererRegistry::create(mode);
}

void RenderController::handleEvent(const sf::Event& event) {
    // Mapowanie klawiszy na rodzaj mapy
    static const std::unordered_map<sf::Keyboard::Key, RenderMode> keyToMode = {
        { sf::Keyboard::Key::Num1, RenderMode::VectorMapRenderer },
        { sf::Keyboard::Key::Num2, RenderMode::FoodGradientMapRenderer },
        { sf::Keyboard::Key::Num3, RenderMode::PressureMapRenderer },
        { sf::Keyboard::Key::Num4, RenderMode::CollisionMapRenderer },
        { sf::Keyboard::Key::Num5, RenderMode::NodeGridMapRenderer },
    };

    if (event.is<sf::Event::KeyPressed>()) {
        auto key = event.getIf<sf::Event::KeyPressed>()->code;
        auto it = keyToMode.find(key);
        if (it != keyToMode.end()) {
            setMode(it->second);
            return;
        }
    }

    if (const auto& mouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouseWheelScrolled->delta > 0)
            view.zoom(0.9f);
        else if (mouseWheelScrolled->delta < 0)
            view.zoom(1.1f);
        window.setView(view);
    }

    if (event.is<sf::Event::MouseButtonPressed>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left) {

            isDragging = true;
            lastMousePos = sf::Mouse::getPosition(window);
        }
    }

    if (event.is<sf::Event::MouseButtonReleased>()) {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        if (mouse->button == sf::Mouse::Button::Left) {
            isDragging = false;

        }
    }


    if (event.is<sf::Event::MouseMoved>() && isDragging) {
        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(lastMousePos) - window.mapPixelToCoords(currentMousePos);
        view.move(delta);
        window.setView(view);
        lastMousePos = currentMousePos;
    }
}

void RenderController::draw(sf::RenderWindow& window, const IPlasmodiumController& pController) {
    if (renderer) {
        renderer->draw(window, pController);
    }
}
