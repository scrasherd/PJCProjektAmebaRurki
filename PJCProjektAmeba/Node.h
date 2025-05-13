#pragma once
#include"Tube.h"
#include"Vec2.h"
#include<vector>

class Tube;

class Node {

private:
	Vec2 NodePosition;
	std::vector<Tube*> ConnectedTubes;

public:
    Node(const Vec2& pos);

    const Vec2& getPosition() const;

    void connectTube(Tube* tube);
    const std::vector<Tube*>& getConnectedTubes() const;

};
