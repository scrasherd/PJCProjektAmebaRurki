#pragma once
#include"Tube.h"
#include"Vec2.h"
#include<vector>

class Tube;

class Node {

private:
	Vec2 NodePosition;
	std::vector<Tube*> ConnectedTubes;

    bool isEnding;

    float pressure;
    float phase;
    float amplitude;

    bool FailedGrowthFlag;
    float RankingValue;

public:
    Node(const Vec2& pos);

    const Vec2& getPosition() const;

    float getPressure() const;
    void setPressure(float p);

    void connectTube(Tube* tube);
    const std::vector<Tube*>& getConnectedTubes() const;

    float getPhase();
    void setPhase(float value);

    int getFailedGrowthFlag();
    void setFailedGrowthFlag(int value);

    float getRankingValue();
    void setRankingValue(float value);
};
