#include "VectorMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "FoodField.h"
#include "MapRendererRegistry.h"

namespace {
    const bool registered = [] {
        MapRendererRegistry::registerRenderer(RenderMode::VectorMapRenderer, [] {
            return std::make_unique<VectorMapRenderer>();
            });
        return true;
        }();
}

void VectorMapRenderer::draw(sf::RenderWindow& window, const IPlasmodiumController& pController) {
    FigureMaker maker;

    sf::VertexArray lineArray(sf::PrimitiveType::Lines);
    sf::VertexArray nodeArray(sf::PrimitiveType::Triangles);
    float halfSizeNode = 2.0f;

    sf::VertexArray FoodSourceArray(sf::PrimitiveType::Triangles);
    float halfSize = 5.0f;

    auto& foodField = pController.getFoodField();

    for (const auto& FoodSource : foodField.getSources()) {
        Vec2 c = FoodSource.getFoodSourcePosition();
        sf::Color color = sf::Color::Red;
        float Size = 10.f;

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
        float alpha = node->getRankingValue();
        sf::Color color(255, 0, 0, alpha);
        if (node->getConnectedTubes().size() != 1) {
            color = sf::Color::Green;
        }
        //else if (node->getAttemptedGrowthFlag()) {
        //    color = sf::Color::Blue;
        //}
        float s = 1.f;
        maker.drawSquareVertex(nodeArray, p, s, color);
    }

    //Vec2 topLeft(0.f, 0.f);
    //Vec2 size(1000.f, 1000.f);

    //sf::VertexArray border(sf::PrimitiveType::LineStrip, 5);
    //border[0].position = sf::Vector2f(topLeft.getX(), topLeft.getY());
    //border[1].position = sf::Vector2f(topLeft.getX() + size.getX(), topLeft.getY());
    //border[2].position = sf::Vector2f(topLeft.getX() + size.getX(), topLeft.getY() + size.getY());
    //border[3].position = sf::Vector2f(topLeft.getX(), topLeft.getY() + size.getY());
    //border[4].position = border[0].position;

    //for (int i = 0; i < 5; ++i) {
    //    border[i].color = sf::Color::Yellow;
    //}
    //window.draw(border);

    window.draw(lineArray);
    window.draw(nodeArray);
    //window.draw(FoodSourceArray);
}

