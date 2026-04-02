#include <iostream>
#include <iomanip>
#include <thread>
#include "engine.h"

// ---------------------------------------------------------
// TODO LIST:
// - Get real motor data (ThrustCurve.org)
// - Calculate dynamic inertia (mass-based)
// - Implement look-up tables (Mach vs Drag)
// ---------------------------------------------------------

int main() {

    PhysicsEngine physics;
    
    physics.initializeScenario();

    physics.runWorkerLoop();

    return 0;
}