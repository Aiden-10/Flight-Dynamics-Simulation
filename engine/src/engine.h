#ifndef ENGINE_H
#define ENGINE_H

#include "Simulation/simulation.h" 

class PhysicsEngine {
private:
    Simulation sim;

public:
    PhysicsEngine();

    // Setup the rockets
    void initializeScenario();

    // logic func
    void runWorkerLoop(); 
};

#endif // ENGINE_H