#pragma once
#include "IPlasmodiumController.h"
#include "SFML/System.hpp"

class SimulationController {
private:
    bool paused = true;

public:
    void togglePause();
    void stepOnce(IPlasmodiumController& controller);
    void updateIfRunning(IPlasmodiumController& controller, sf::Clock& clock);

    bool isPaused() const;


};
