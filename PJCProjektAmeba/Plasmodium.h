#pragma once
#include "TubeSystem.h"
#include <vector>
#include <memory>
#include <random>
#include "Node.h"
#include "Tube.h"

class Plasmodium {

private:
	std::vector<std::unique_ptr<Node>> nodes;
	std::vector<std::unique_ptr<Tube>> tubes;

	std::mt19937 rng;

	float tubeLength;


public:

	Plasmodium(const Vec2& startPosition, float tubeLength);

	void growOneStep();

	const std::vector<std::unique_ptr<Node>>& getNodes() const;
	const std::vector<std::unique_ptr<Tube>>& getTubes() const;

};
