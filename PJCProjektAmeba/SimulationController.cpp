#include "SimulationController.h"

void SimulationController::togglePause() {
    paused = !paused;
}

bool SimulationController::isPaused() const {
    return paused;
}

void SimulationController::stepOnce(IPlasmodiumController& controller) {
    if (paused) {
        controller.update();
    }
}

void SimulationController::updateIfRunning(IPlasmodiumController& controller, sf::Clock& clock) {
    if (!paused && clock.getElapsedTime().asMilliseconds() > 1) {
        controller.update();
    }
}
