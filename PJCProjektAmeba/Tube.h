#pragma once
#include "Vec2.h"

class Node;

class Tube {

private:
	Node* NodeA;
	Node* NodeB;

	float TubeDiameter;

    float dP;
    float dPnorm; //pole pomocniczne na umieszczenie znormalizowanej wartoœci dP dla ka¿dego wêz³a 

    float CytoplasmAmount;

public:

    Tube(Node* a, Node* b, float diameter, float CytValue);

    const Node* getNodeA() const;
    const Node* getNodeB() const;
    const Node* getOtherNode(const Node* node) const;

    float getDiameter() const;

    Vec2 getDirection() const;
    float getLength() const;

    float getdP();
    void setdP(float value);

    float getdPnorm();
    void setdPnorm(float value);

    float getCytoplasmAmount();
    void setCytoplasmAmount(float value);
};
