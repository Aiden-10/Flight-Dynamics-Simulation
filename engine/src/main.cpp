#include <iostream>
#include <iomanip>
#include <thread>
#include "engine.h"
#include "Networking/TelemetryStreamer.h"
#include "Networking/DataStructures.h"
// ---------------------------------------------------------
// TODO LIST:
// - Get real motor data (ThrustCurve.org)
// - Calculate dynamic inertia (mass-based)
// - Implement look-up tables (Mach vs Drag)
// ---------------------------------------------------------

int main() {

    std::cout << "Starting Flight Dynamics Simulation..." << std::endl;

    PhysicsEngine physics;
    
    physics.initializeScenario();

    physics.runWorkerLoop();

    std::cout << "Simulation completed." << std::endl;

    return 0;
}