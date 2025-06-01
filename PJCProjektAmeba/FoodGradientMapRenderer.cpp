#include "FoodGradientMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "FoodField.h"

void FoodGradientMapRenderer::draw(sf::RenderWindow& window, const IPlasmodiumController& pController) {
    FigureMaker maker;

    sf::VertexArray lineArray(sf::PrimitiveType::Lines);
    sf::VertexArray nodeArray(sf::PrimitiveType::Triangles);

    sf::VertexArray FoodSourceArray(sf::PrimitiveType::Triangles);

    sf::VertexArray gradient(sf::PrimitiveType::Triangles);

    auto& foodField = pController.getFoodField();

    auto getValueAt = [&](const Vec2& pos) {
        return foodField.getValueAt(pos);
    };

    auto colorFromValue = [](float value) -> sf::Color {
        float t = std::clamp(value, 0.f, 1.f);
        int alpha = static_cast<int>(t * 255.0f);
        return sf::Color(255, 140, 0, alpha); // pomarañczowy z przejrzystoœci¹
    };

    const float radius = 100.0f;
    const int resolution = 5;

    for (const auto& src : foodField.getSources()) {
        maker.drawCircleGradientVertexFull(
            gradient,
            src.getFoodSourcePosition(),
            radius,
            resolution,
            [&](const Vec2& pos) {
                return foodField.getValueAt(pos);
            },
            colorFromValue
        );
    }

    for (const auto& FoodSource : foodField.getSources()) {
        Vec2 c = FoodSource.getFoodSourcePosition();
        sf::Color color = sf::Color::Red;
        float Size = 3.f;

        maker.drawSquareVertex(FoodSourceArray, c, Size, color);
    }

    for (const auto& tube : pController.getTubes()) {
        Vec2 a = tube->getNodeA()->getPosition();
        Vec2 b = tube->getNodeB()->getPosition();

        lineArray.append(sf::Vertex(sf::Vector2f(a.getX(), a.getY()), sf::Color::White));
        lineArray.append(sf::Vertex(sf::Vector2f(b.getX(), b.getY()), sf::Color::White));
    }

    for (const auto& node : pController.getNodes()) {
        Vec2 p = node->getPosition();
        sf::Color color = (node->getConnectedTubes().size() == 1) ?
            sf::Color::Green : sf::Color(100, 100, 100);

        float s = 1.f;
        maker.drawSquareVertex(nodeArray, p, s, color);
    }

    window.draw(gradient);
    window.draw(lineArray);
    //window.draw(nodeArray);
    window.draw(FoodSourceArray);
}
