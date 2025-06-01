#pragma once
#include <vector>
#include "IPlasmodiumController.h"
#include "IPlasmodiumModel.h"


class Node;
class Tube;

class FlowModel : public IPlasmodiumModel {
private:
    IPlasmodiumController& pController;

    float resistanceFactor = 1.0f; 
    float damping = 0.2f;

public:
    FlowModel(IPlasmodiumController& pController);

    void simulate() override;

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
