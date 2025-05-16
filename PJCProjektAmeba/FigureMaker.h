#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"


class FigureMaker {
private:

public:
	void drawSquareVertex(sf::VertexArray& array, const Vec2& pos, float size, sf::Color color);
};