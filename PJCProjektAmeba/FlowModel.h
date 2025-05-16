#pragma once
class Plasmodium;
class FoodField;
class Node;
class Tube;

class FlowModel {
private:
    Plasmodium& plasmodium;
    FoodField& foodField;

    float resistanceFactor = 1.0f; // opór rurki ~ 1 / (œrednica^2)
    float damping = 0.2f;          // jak szybko zmienia siê ciœnienie

public:
    FlowModel(Plasmodium& owner, FoodField& foodField);

    //void updateFlow(float dt); // jedna iteracja przep³ywu

    void updatePhasesAndPressures(float dt);
};
