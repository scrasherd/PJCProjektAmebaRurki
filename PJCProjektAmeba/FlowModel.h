#pragma once
class Plasmodium;
class Node;
class Tube;

class FlowModel {
private:
    Plasmodium& plasmodium;

    float resistanceFactor = 1.0f; // opór rurki ~ 1 / (œrednica^2)
    float damping = 0.2f;          // jak szybko zmienia siê ciœnienie

    float computeFlow(Node* from, Node* to, float diameter);

public:
    FlowModel(Plasmodium& owner);

    void updateFlow(float dt); // jedna iteracja przep³ywu
};
