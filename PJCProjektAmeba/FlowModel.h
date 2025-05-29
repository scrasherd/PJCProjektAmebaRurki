#pragma once
#include <vector>
class Plasmodium;
class FoodField;
class Node;
class Tube;

class FlowModel {
private:
    Plasmodium& plasmodium;
    FoodField& foodField;

    float resistanceFactor = 1.0f; 
    float damping = 0.2f;

public:
    FlowModel(Plasmodium& owner, FoodField& foodField);

    //void updateFlow(float dt);

    void updatePhasesAndPressures(float dt);

    void computeDPinTubes();

    void computeFlow();
    void CytoplasmTransfer(Node* node);

    bool checkNodeForCorrection(Node* node);
    void correctNodePressure(Node* node);

    void normalizedPinTubes(Node* node);



    //bool isNodeA(Node* node, Tube* tube);
};
