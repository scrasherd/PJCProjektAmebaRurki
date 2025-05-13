#include "VectorMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"

void VectorMapRenderer::draw(sf::RenderWindow& window, const Plasmodium& plasmodium) {
    sf::VertexArray lineArray(sf::PrimitiveType::Lines);
    sf::VertexArray nodeArray(sf::PrimitiveType::Lines);

    for (const auto& tube : plasmodium.getTubes()) {
        Vec2 a = tube->getNodeA()->getPosition();
        Vec2 b = tube->getNodeB()->getPosition();

        lineArray.append(sf::Vertex(sf::Vector2f(a.x, a.y), sf::Color::White));
        lineArray.append(sf::Vertex(sf::Vector2f(b.x, b.y), sf::Color::White));
    }

    for (const auto& node : plasmodium.getNodes()) {
        Vec2 p = node->getPosition();
        sf::Color color = (node->getConnectedTubes().size() == 1) ?
            sf::Color::Green : sf::Color(100, 100, 100);

        float s = 0.1f;
        nodeArray.append(sf::Vertex({ p.x - s, p.y - s }, color));
        nodeArray.append(sf::Vertex({ p.x + s, p.y + s }, color));
        nodeArray.append(sf::Vertex({ p.x + s, p.y - s }, color));
        nodeArray.append(sf::Vertex({ p.x - s, p.y + s }, color));
    }

    window.draw(lineArray);
    window.draw(nodeArray);
}
