#ifndef APPLICATION_H
#define APPLICATION_H

#include "Simulation/simulation.h" 
#include "Helpers/ThreadSafeQueue.h"
#include "Networking/DataStructures.h"

class Application {
private:
    // Core Simulation Engine
    Simulation sim;

    // Thread-safe queue for commands from the UI
    TSQueue<Command> commandQueue;

public:
    // Constructor
    Application();

    // Adds vehicles and sets up the initial scenario
    void initializeScenario();

    // Main loop for running the simulation
    void runWorkerLoop(); 
};

#endif // APPLICATION_H