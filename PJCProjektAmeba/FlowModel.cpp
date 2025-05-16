#include "FlowModel.h"
#include "Plasmodium.h"
#include "Tube.h"
#include "Node.h"

FlowModel::FlowModel(Plasmodium& owner, FoodField& food) : plasmodium(owner), foodField(food) {}

void FlowModel::updatePhasesAndPressures(float dt) {
    auto& nodePtrs = plasmodium.getNodes(); // zak³adamy tak¹ metodê

    for (auto& nodePtr : nodePtrs) {
        Node* thisNode = nodePtr.get();
        float phaseSum = 0.f;
        int count = 0;

        for (Tube* tube : thisNode->getConnectedTubes()) {
            Node* neighbor = tube->getNodeA();
            if (neighbor == thisNode) {
                neighbor = tube->getNodeB();
            }
            phaseSum += neighbor->getPhase();
            ++count;
        }

        float averageNeighborPhase = thisNode->getPhase();
        if (count > 0) {
            averageNeighborPhase = phaseSum / count;
        }

        // 2. Dodanie szumu (losowa niewielka fluktuacja)
        float noise = 0.1f * ((rand() / (float)RAND_MAX) - 0.5f); // zakres [-0.05, 0.05]

        // 3. Aktualizacja fazy – dostrajanie siê do s¹siadów (damping = jak szybko siê synchronizuje)
        float phaseDelta = (damping * (averageNeighborPhase - nodePtr->getPhase()) + noise)*dt;
        float newPhase = nodePtr->getPhase() + phaseDelta;
        nodePtr->setPhase(newPhase);

        // 4. Oblicz ciœnienie z fazy
        float foodAmount = foodField.getValueAt(nodePtr->getPosition());
        float foodEffect = std::exp(foodAmount) - 1; // im wiêcej jedzenia, tym mniejsze ciœnienie
        nodePtr->setPressure(0.5f * std::sin(newPhase)+ 2.5f - foodEffect);
    }
}

