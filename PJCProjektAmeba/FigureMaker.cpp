#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include "FigureMaker.h"
#include <functional>
#include <cmath>

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

void FigureMaker::drawCircleGradientVertexPart(sf::VertexArray& array, const Vec2& center, float innerRadius, float outerRadius, sf::Color centerColor, sf::Color outerColor, int segments) {
    sf::Vector2f c(center.getX(), center.getY());

    for (int i = 0; i < segments; ++i) {
        float angle1 = i * 2 * M_PI / segments;
        float angle2 = (i + 1) * 2 * M_PI / segments;

        sf::Vector2f p1_in = c + sf::Vector2f(std::cos(angle1), std::sin(angle1)) * innerRadius;
        sf::Vector2f p2_in = c + sf::Vector2f(std::cos(angle2), std::sin(angle2)) * innerRadius;

        sf::Vector2f p1_out = c + sf::Vector2f(std::cos(angle1), std::sin(angle1)) * outerRadius;
        sf::Vector2f p2_out = c + sf::Vector2f(std::cos(angle2), std::sin(angle2)) * outerRadius;

        // Trójk¹t 1
        array.append(sf::Vertex(p1_in, centerColor));
        array.append(sf::Vertex(p1_out, outerColor));
        array.append(sf::Vertex(p2_out, outerColor));

        // Trójk¹t 2
        array.append(sf::Vertex(p1_in, centerColor));
        array.append(sf::Vertex(p2_out, outerColor));
        array.append(sf::Vertex(p2_in, centerColor));
    }
}

void FigureMaker::drawCircleGradientVertexFull(sf::VertexArray& array, const Vec2& center, float radius, int resolution, 
                                                std::function<float(const Vec2&)> getValueAt, std::function<sf::Color(float)> colorFromValue) {

    float step = radius / resolution;
    int x = center.getX();
    int y = center.getY();

    for (int i = resolution - 1; i >= 0; i--) {
        float innerRadius = i * step;
        float outerRadius = (i + 1) * step;

        float centerValue = getValueAt(center + Vec2(0, -innerRadius));
        float outerValue = getValueAt(center + Vec2(0, -outerRadius));

        sf::Color centerColor = colorFromValue(centerValue);
        sf::Color outerColor = colorFromValue(outerValue);

        drawCircleGradientVertexPart(array, center, innerRadius, outerRadius, centerColor, outerColor, 20);
    }
}