#include "NodeGridMapRenderer.h"
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Tube.h"
#include "NodeField.h"
#include <iostream>
#include "MapRendererRegistry.h"

namespace {
    const bool registered = [] {
        MapRendererRegistry::registerRenderer(RenderMode::NodeGridMapRenderer, [] {
            return std::make_unique<NodeGridMapRenderer>();
            });
        return true;
        }();
}

void NodeGridMapRenderer::draw(sf::RenderWindow& window, const IPlasmodiumController& pController) {
    FigureMaker maker;
    sf::VertexArray NodeGridArray(sf::PrimitiveType::Triangles);

    const NodeField& nodeField = pController.getNodeField();

    float cellSize = nodeField.getCellSize();

    const auto& sparse = nodeField.getNodeGrid();
    const int blockSize = 8;

    for (const auto& [blockCoord, gridBlock] : sparse) {
        int blockX = blockCoord.first;
        int blockY = blockCoord.second;

        for (int by = 0; by < blockSize; ++by) {
            for (int bx = 0; bx < blockSize; ++bx) {
                Node* node = gridBlock.get(bx, by);
                if (node) {
                    float px = (blockX * blockSize + bx + 0.5f) * cellSize;
                    float py = (blockY * blockSize + by + 0.5f) * cellSize;

                    sf::Color color = sf::Color(150, 255, 0, 255); 

                    maker.drawSquareVertex(NodeGridArray, Vec2(px, py), cellSize, color);
                }
            }
        }
    }

    window.draw(NodeGridArray);
}
