#include "CollisionMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "CollisionField.h"
#include <iostream>

void CollisionMapRenderer::draw(sf::RenderWindow& window, const Plasmodium& plasmodium, const FoodField& foodField) {
    FigureMaker maker;

    sf::VertexArray CollisionArray(sf::PrimitiveType::Triangles);

    const CollisionField& colField = plasmodium.getCollisionField();

    float cellSize = colField.getCellSize();

    int zakres = colField.getHeight() * (0.35);
    //int zakres = 4500;

    for (int x = zakres; x < colField.getHeight() - zakres; ++x) {
        for (int y = zakres; y < colField.getWidth() - zakres; ++y) {
            float px = (x + 0.5f) * cellSize;
            float py = (y + 0.5f) * cellSize;
            float density = colField.getDensity(Vec2(py, px));

            if (density > 0.0f) {
                //std::cout << "Density at (0,0): " << colField.getDensity(Vec2(px, py)) << std::endl;
                // Im wiêksza gêstoœæ, tym ciemniejszy kolor
                float clamped = std::min(density / 5.f, 1.f);
                sf::Color color = sf::Color(255, 0, 0, 255);

                maker.drawSquareVertex(CollisionArray, Vec2(px, py), cellSize, color);
            }
        }
    }




    window.draw(CollisionArray);
}