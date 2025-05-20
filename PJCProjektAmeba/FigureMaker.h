#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"
#include <functional>


class FigureMaker {
private:

public:
	void drawSquareVertex(sf::VertexArray& array, const Vec2& pos, float size, sf::Color color);

	void drawCircleGradientVertexPart(sf::VertexArray& array, const Vec2& center, float innerRadius, float outerRadius, sf::Color centerColor, sf::Color outerColor, int segments);
	void drawCircleGradientVertexFull(sf::VertexArray& array, const Vec2& center, float radius, int resolution,
										std::function<float(const Vec2&)> getValueAt, std::function<sf::Color(float)> colorFromValue);


};