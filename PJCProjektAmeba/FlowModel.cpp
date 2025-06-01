#define _USE_MATH_DEFINES
#include "FlowModel.h"
#include "FoodField.h"
#include "Tube.h"
#include "Node.h"
#include <vector>
#include <iostream>

FlowModel::FlowModel(IPlasmodiumController& pController) : pController(pController) {}

void FlowModel::simulate() {
    updatePhasesAndPressures(0.01f);
    computeDPinTubes();           
    computeFlow();               
}

void FlowModel::updatePhasesAndPressures(float dt) {
    auto& foodField = pController.getFoodField();

    auto& nodePtrs = pController.getNodes();
    const float baseOmega = 2.f * M_PI * 0.5f; // 0.5 Hz
    const float coupling = 1.0f;

    for (auto& nodePtr : nodePtrs) {
        Node* thisNode = nodePtr.get();
        float phase = thisNode->getPhase();
        float phaseSum = 0.f;
        int count = 0;

        for (Tube* tube : thisNode->getConnectedTubes()) {
            const Node* neighbor = tube->getOtherNode(thisNode);
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
        float targetVelocity = baseOmega + coupling * std::sin(averageNeighborPhase - phase);
        float newPhase = (phase + targetVelocity * dt + noise);
        nodePtr->setPhase(newPhase);

        // 4. Oblicz ciœnienie z fazy
        float foodAmount = foodField.getValueAt(nodePtr->getPosition());
        float foodEffect = std::exp(foodAmount) - 1.f; // im wiêcej jedzenia, tym mniejsze ciœnienie
        nodePtr->setPressure(0.5f * std::sin(newPhase)+ 2.5f - foodEffect);

        if (nodePtr->getPosition() == Vec2(500, 500)) {
            //std::cout << nodePtr->getPressure() << "\n";
        }
    }
}

void FlowModel::computeDPinTubes() {
    auto& tubePtrs = pController.getTubes();
    //dP = Pa - Pb

    for (auto& tubePtr : tubePtrs) {
        const Node* nodeA = tubePtr->getNodeA();
        const Node* nodeB = tubePtr->getNodeB();

        float Pa = nodeA->getPressure();
        float Pb = nodeB->getPressure();

        tubePtr->setdP(Pa - Pb);
    }

}

void FlowModel::computeFlow() {
    auto& nodePtrs = pController.getNodes();

    float CytIn = 0;

    for (auto& nodePtr : nodePtrs) {
        Node* thisNode = nodePtr.get();
        bool NodeNeedsCorrection = checkNodeForCorrection(thisNode);
        
        if (NodeNeedsCorrection) {
            correctNodePressure(thisNode);
        }
        else {
            normalizedPinTubes(thisNode);
        }

        CytoplasmTransfer(thisNode);
    }
}

bool FlowModel::checkNodeForCorrection(Node* node) {
    bool AllIN = true;
    bool AllOUT = true;
    auto& tubes = node->getConnectedTubes();

    for (std::size_t i = 0; i < tubes.size() && (AllIN || AllOUT); ++i) {
        Tube* tube = tubes[i];
        if (tube->getNodeA() == node && tube->getdP() > 0) {
            AllIN = false;
        }
        else if (tube->getNodeA() == node && tube->getdP() < 0) {
            AllOUT = false;
        }
        else if (tube->getNodeA() != node && tube->getdP() > 0) {
            AllOUT = false;
        }
        else if (tube->getNodeA() != node && tube->getdP() < 0) {
            AllIN = false;
        }
    }
    if (AllIN || AllOUT) {
        return true;
    }
    else {
        return false;
    }
}

void FlowModel::correctNodePressure(Node* node){
    int count = 0;
    float pressureSum = 0;
    std::vector<float> correctdP;

    for (Tube* tube : node->getConnectedTubes()) {//uœrednanie ciœnienia œrodkowego wêz³a
        pressureSum += tube->getOtherNode(node)->getPressure();
        ++count;
    }
    float newPressure = pressureSum / count;
    node->setPressure(newPressure);

    for (Tube* tube : node->getConnectedTubes()) {//obliczanie prpze³ywów na podstawie nowego ciœnienia
        tube->setdPnorm(node->getPressure() - tube->getOtherNode(node)->getPressure());
    }
}

void FlowModel::normalizedPinTubes(Node* node) {

    for (Tube* tube : node->getConnectedTubes()) {
        float dP = tube->getdP();
        if (node == tube->getNodeA()) {
            tube->setdPnorm(dP);
        }
        else if (node == tube->getNodeB()) {
            tube->setdPnorm(-dP);
        }
    }
}

void FlowModel::CytoplasmTransfer(Node* node) {
    float availableSpace = 0;
    float availableCyt = 0;
    float sumOutDPNorm = 0.0f;
    std::vector<float> outdPValues;

    for (Tube* tube : node->getConnectedTubes()) {
        if (tube->getdPnorm() < 0) {
            float flow = std::min(tube->getCytoplasmAmount(), std::abs(tube->getdPnorm()) * 100.f);
            tube->setCytoplasmAmount(tube->getCytoplasmAmount() - flow);
            availableCyt += flow;
        }
        else {
            outdPValues.push_back(tube->getdPnorm());
            sumOutDPNorm += tube->getdPnorm();
        }
    }

    for (Tube* tube : node->getConnectedTubes()) {
        if (tube->getdPnorm() > 0) {
            float share = std::max(0.0f, tube->getdPnorm() / sumOutDPNorm);
            tube->setCytoplasmAmount(tube->getCytoplasmAmount() + availableCyt * share);
        }

    }
}

