#pragma once
#include "Vec2.h"

class Node;

class Tube {

private:
	Node* NodeA;
	Node* NodeB;

	float TubeDiameter;

	bool isEnding;

public:

    Tube(Node* a, Node* b, float diameter, bool isEnding = true);

    Node* getNodeA() const;
    Node* getNodeB() const;

    float getDiameter() const;

    bool getIsEnding() const;
    void setIsEnding(bool value);

    Vec2 direction() const;
    float length() const;

};
