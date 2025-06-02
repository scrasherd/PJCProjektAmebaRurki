#pragma once
#include "Vec2.h"

class Node;

class IPlasmodiumObserver {
public:
    virtual void onNodeAdded(const Vec2& pos, Node* node) = 0;
    virtual void onTubeAdded(const Vec2& posA, const Vec2& posB) = 0;
    virtual void onNodeAddedAttempt(const Vec2& pos, float angle) = 0;
    virtual void onStepMade() = 0;
    virtual ~IPlasmodiumObserver() = default;
};
