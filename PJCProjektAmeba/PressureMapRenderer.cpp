#include "PressureMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "FoodField.h"
#include <algorithm>
#include <iostream>

void PressureMapRenderer::draw(sf::RenderWindow& window, const Plasmodium& plasmodium, const FoodField& foodField) {
    FigureMaker maker;

    sf::VertexArray lineArray(sf::PrimitiveType::Lines);
    sf::VertexArray nodeArray(sf::PrimitiveType::Triangles);

    for (const auto& tube : plasmodium.getStaticTubes()) {
        Vec2 a = tube->getNodeA()->getPosition();
        Vec2 b = tube->getNodeB()->getPosition();

        float cytoplasm = tube->getCytoplasmAmount();
        float normCytoplasm = std::clamp(cytoplasm / 20.0f, 0.f, 1.f);

        sf::Color color = sf::Color(
            static_cast<unsigned char>(0),
            static_cast<unsigned char>(255 * normCytoplasm),
            static_cast<unsigned char>(255 * (1.0f - normCytoplasm)),
            255
        );

        lineArray.append(sf::Vertex(sf::Vector2f(a.getX(), a.getY()), color));
        lineArray.append(sf::Vertex(sf::Vector2f(b.getX(), b.getY()), color));
    }

    for (const auto& node : plasmodium.getStaticNodes()) {
        Vec2 p = node->getPosition();
        float pressure = node->getPressure();
        float normP = std::clamp((pressure - 0.3f) / (3.0f - 0.3f), 0.0f, 1.0f);

        sf::Color color = sf::Color(255 * normP, 0, 0);

        float s = 1.f;
        maker.drawSquareVertex(nodeArray, p, s, color);
    }

    window.draw(lineArray);
    window.draw(nodeArray);
}

