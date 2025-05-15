#pragma once
#include "FoodField.h"
#include "FlowModel.h"
#include <vector>
#include <memory>
#include <random>
#include "Node.h"
#include "Tube.h"

class Plasmodium {

private:
	std::vector<std::unique_ptr<Node>> nodes;
	std::vector<Node*> EndingNodes;

	std::vector<std::unique_ptr<Tube>> tubes;

	FlowModel flowModel;

	std::mt19937 rng;

	float tubeLength;


public:

	Plasmodium(const Vec2& startPosition, float tubeLength);

	//Rozrost
	void growOneStep(const FoodField& foodField);

	void sortEndings(const FoodField& foodField);

	float computeAngle(Node* parent);
	float generateAngle(float baseAngle, int generated, int direction, const FoodField& foodField, const Vec2& position);

	//Przep³yw
	void simulateFlow(float dt);



	const std::vector<std::unique_ptr<Node>>& getNodes() const;
	const std::vector<std::unique_ptr<Tube>>& getTubes() const;

};
