#include "FoodGradientMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "FoodField.h"

void FoodGradientMapRenderer::draw(sf::RenderWindow& window, const Plasmodium& plasmodium, const FoodField& foodField) {
    sf::VertexArray lineArray(sf::PrimitiveType::Lines);
    sf::VertexArray nodeArray(sf::PrimitiveType::Lines);

    sf::VertexArray FoodSourceArray(sf::PrimitiveType::Triangles);
    float halfSize = 5.0f;

    sf::VertexArray gradient(sf::PrimitiveType::Points);

    for (int y = 0; y < foodField.getHeight(); ++y) {
        for (int x = 0; x < foodField.getWidth(); ++x) {
            float value = foodField.getValueAt(x, y);
            if (value > 0.0f) {
                // jasnoœæ skaluje siê z wartoœci¹ po¿ywienia
                int alpha = static_cast<int>(std::clamp(value * 255.0f, 0.0f, 255.0f));
                sf::Color color(255, 140, 0, alpha); // pomarañczowy odcieñ

                gradient.append(sf::Vertex(sf::Vector2f(x, y), color));
            }
        }
    }

    for (const auto& FoodSource : foodField.getSources()) {
        Vec2 c = FoodSource.getFoodSourcePosition();
        float cx = c.x;
        float cy = c.y;
        sf::Color color = sf::Color::Red;

        // Wierzcho³ki kwadratu
        sf::Vector2f A(cx - halfSize, cy - halfSize);
        sf::Vector2f B(cx + halfSize, cy - halfSize);
        sf::Vector2f C(cx + halfSize, cy + halfSize);
        sf::Vector2f D(cx - halfSize, cy + halfSize);

        // Trójk¹t ABC
        FoodSourceArray.append(sf::Vertex(A, color));
        FoodSourceArray.append(sf::Vertex(B, color));
        FoodSourceArray.append(sf::Vertex(C, color));

        // Trójk¹t ACD
        FoodSourceArray.append(sf::Vertex(A, color));
        FoodSourceArray.append(sf::Vertex(C, color));
        FoodSourceArray.append(sf::Vertex(D, color));
    }

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
    window.draw(gradient);
    window.draw(lineArray);
    window.draw(nodeArray);
    window.draw(FoodSourceArray);
}
