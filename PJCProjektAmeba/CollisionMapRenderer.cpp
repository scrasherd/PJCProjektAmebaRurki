#include "CollisionMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "CollisionField.h"
#include <iostream>

void CollisionMapRenderer::draw(sf::RenderWindow& window, const Plasmodium& plasmodium, const FoodField& foodField) {
    FigureMaker maker;
    sf::VertexArray CollisionArray(sf::PrimitiveType::Triangles);

    sf::VertexArray RadarArray(sf::PrimitiveType::Triangles);

    const CollisionField& colField = plasmodium.getStaticCollisionField();
    const RadarField& radarField = plasmodium.getStaticRadarField();

    float cellSize = colField.getCellSize();

    const auto& sparse = colField.getSparseGrid();
    const int blockSize = 8;

    for (const auto& [blockCoord, gridBlock] : sparse) {
        int blockX = blockCoord.first;
        int blockY = blockCoord.second;
        
        for (int by = 0; by < blockSize; ++by) {
            for (int bx = 0; bx < blockSize; ++bx) {
                float density = gridBlock.get(bx,by);
                if (density > 0.1f) {
                    float px = (blockX * blockSize + bx + 0.5f) * cellSize;
                    float py = (blockY * blockSize + by + 0.5f) * cellSize;

                    float clamped = std::min(density / 5.f, 1.f);
                    sf::Color color = sf::Color(255, 0, 0, static_cast<unsigned int>(clamped * 255));

                    maker.drawSquareVertex(CollisionArray, Vec2(px, py), cellSize, color);
                }
            }
        }
    }

    const auto& radar = radarField.getRadarGrid();

    for (const auto& [blockCoord, gridBlock] : radar) {
        int blockX = blockCoord.first;
        int blockY = blockCoord.second;

        for (int by = 0; by < blockSize; ++by) {
            for (int bx = 0; bx < blockSize; ++bx) {
                float density = gridBlock.get(bx,by);
                if (density > 0.1f) {
                    float px = (blockX * blockSize + bx + 0.5f) * cellSize;
                    float py = (blockY * blockSize + by + 0.5f) * cellSize;

                    float clamped = std::min(density / 5.f, 1.f);
                    sf::Color color = sf::Color(0, 0, 255, static_cast<unsigned int>(clamped * 255));

                    maker.drawSquareVertex(RadarArray, Vec2(px, py), cellSize, color);
                }
            }
        }
    }
    window.draw(CollisionArray);
    window.draw(RadarArray);
}
