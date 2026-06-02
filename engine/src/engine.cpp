#include "engine.h"

#include <thread>

PhysicsEngine::PhysicsEngine() : sim(0.01, 300) {
    // The Simulation is initialized with a timestep of 0.01s and a max duration of 300s (5 minutes)
}


void PhysicsEngine::initializeScenario() {
    
    Matrix3x3 inertia(
        1.2,  0.0,  0.0,  // X (Roll) 
        0.0, 45.0,  0.0,  // Y (Pitch)
        0.0,  0.0, 45.0   // Z (Yaw)  
    );

    RigidBodyState state(
        Vector3(0, 0, 0),                       // Position: Launch Pad A (Origin)
        Vector3(0, 0, 0),                       // Velocity
        Vector3(0, 0, 0),                       // Angular Velocity
        Quaternion().getLaunchOrientation(80),  // Aimed 80 degrees up
        inertia,                                // inertia from earlier
        200.0                                   // Initial Mass (kg)
    );

    // Thrust: 2500N, Burn Time: 6.0s, Fuel Mass: 40.0kg
    Propulsion prop(5500.0, 10.0, 100.0, Vector3(0.0, 0.0, 0.0), 0.0);

    // Add to the physics engine's simulation
    this->sim.addVehicle(state, prop);

}

void PhysicsEngine::runWorkerLoop() {
    sim.start();

    while (sim.isRunning()) {
        
        if (sim.isRunning() && sim.getCurrentTime() < sim.getMaxDuration()) {
            // Step the sim forward
            sim.step();

            // Check for collisions or ground impact
            bool hasCollided = sim.checkCollisions();
            if (hasCollided) {
                sim.stop();
            }
        } else {
            sim.stop();
        }

        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Physics Thread Finished" << std::endl;
}