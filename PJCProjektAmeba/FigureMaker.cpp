#include <SFML/Graphics.hpp>
#include "FigureMaker.h"

void FigureMaker::drawSquareVertex( sf::VertexArray& array, const Vec2& pos, float size, sf::Color color){
    float half = size / 2.0f;
    float cx = pos.getX();
    float cy = pos.getY();

    sf::Vector2f A(cx - half, cy - half);
    sf::Vector2f B(cx + half, cy - half);
    sf::Vector2f C(cx + half, cy + half);
    sf::Vector2f D(cx - half, cy + half);

    array.append(sf::Vertex(A, color));
    array.append(sf::Vertex(B, color));
    array.append(sf::Vertex(C, color));

    array.append(sf::Vertex(A, color));
    array.append(sf::Vertex(C, color));
    array.append(sf::Vertex(D, color));
}