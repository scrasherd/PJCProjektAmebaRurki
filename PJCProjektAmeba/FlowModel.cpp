#include "FlowModel.h"
#include "Plasmodium.h"
#include "Tube.h"
#include "Node.h"

FlowModel::FlowModel(Plasmodium& owner) : plasmodium(owner) {}

float FlowModel::computeFlow(Node* from, Node* to, float diameter) {
    float deltaP = from->getPressure() - to->getPressure();
    float resistance = resistanceFactor / (diameter * diameter); // uproszczony model oporu
    return deltaP / resistance;
}

void FlowModel::updateFlow(float dt) {
    for (const auto& tube : plasmodium.getTubes()) {
        Node* A = tube->getNodeA();
        Node* B = tube->getNodeB();

        float flow = computeFlow(A, B, tube->getDiameter());

        // Aktualizacja ciœnieñ – symetryczna
        A->setPressure(A->getPressure() - flow * damping * dt);
        B->setPressure(B->getPressure() + flow * damping * dt);
    }
}
