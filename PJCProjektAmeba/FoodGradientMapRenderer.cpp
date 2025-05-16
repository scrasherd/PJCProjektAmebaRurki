#include "FoodGradientMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "FoodField.h"

void FoodGradientMapRenderer::draw(sf::RenderWindow& window, const Plasmodium& plasmodium, const FoodField& foodField) {
    FigureMaker maker;

    sf::VertexArray lineArray(sf::PrimitiveType::Lines);
    sf::VertexArray nodeArray(sf::PrimitiveType::Triangles);

    sf::VertexArray FoodSourceArray(sf::PrimitiveType::Triangles);

    sf::VertexArray gradient(sf::PrimitiveType::Points);

    for (int y = 0; y < foodField.getHeight(); ++y) {
        for (int x = 0; x < foodField.getWidth(); ++x) {
            float value = foodField.getValueAt(Vec2(y,x));
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
        sf::Color color = sf::Color::Red;
        float Size = 10.f;

        maker.drawSquareVertex(FoodSourceArray, c, Size, color);
    }

    for (const auto& tube : plasmodium.getTubes()) {
        Vec2 a = tube->getNodeA()->getPosition();
        Vec2 b = tube->getNodeB()->getPosition();

        lineArray.append(sf::Vertex(sf::Vector2f(a.getX(), a.getY()), sf::Color::White));
        lineArray.append(sf::Vertex(sf::Vector2f(b.getX(), b.getY()), sf::Color::White));
    }

    for (const auto& node : plasmodium.getNodes()) {
        Vec2 p = node->getPosition();
        sf::Color color = (node->getConnectedTubes().size() == 1) ?
            sf::Color::Green : sf::Color(100, 100, 100);

        float s = 1.f;
        maker.drawSquareVertex(nodeArray, p, s, color);
    }

    window.draw(gradient);
    window.draw(lineArray);
    window.draw(nodeArray);
    window.draw(FoodSourceArray);
}
