#include "simulation.h"
#include <iostream>

Simulation::Simulation(double dt, double maxDuration) : integrator(dt){
    this->dt = dt;
    this->maxDuration = maxDuration;
}

void Simulation::log(double time) {
    if (checkCollisions()) {
        return;
    }
    for (auto& vehicle : vehicles) {
        RigidBodyState state = vehicle.getRigidBodyState();
        static double lastLogTime = 0.0;
        if (currentTime - lastLogTime >= 0.1) {
            Vector3 euler = state.orientation.toEuler(); 
            Aerodynamics::Data aeroData = vehicle.aerodynamics.calculate(state, environment);
            logger.log(
                currentTime, 
                state.pos.x, state.pos.y, state.pos.z,
                state.vel.x, state.vel.y, state.vel.z,
                euler.x * 57.2958,  // Roll (deg)
                euler.y * 57.2958,  // Pitch (deg)
                euler.z * 57.2958,  // Yaw (deg)
                state.angularVel.x, 
                state.angularVel.y, 
                state.angularVel.z,
                aeroData.alpha * 57.2958, // Angle of Attack (deg)
                state.mass
            );
            lastLogTime = currentTime;
        }
    }
}

SimulationState Simulation::step() {
    
    for (auto& vehicle : vehicles) {

        RigidBodyState currentState = vehicle.getRigidBodyState();
        RigidBodyState derivative = vehicle.getDerivative(currentTime);

        // Calculate the new state
        integrator.step(currentState, derivative);

        // Update the vehicle's state
        vehicle.setState(currentState);

        // Log the current state
        log(currentTime);

        // Prepare the SimulationState to send to the UI
        SimulationState simState;
        simState.currentTime = currentTime;
        simState.px = currentState.pos.x;
        simState.py = currentState.pos.y;
        simState.pz = currentState.pos.z;
        simState.qw = currentState.orientation.w;
        simState.qx = currentState.orientation.x;
        simState.qy = currentState.orientation.y;
        simState.qz = currentState.orientation.z;
        simState.vx = currentState.vel.x;
        simState.vy = currentState.vel.y;
        simState.vz = currentState.vel.z;
        simState.fuel_mass = currentState.mass;

        currentTime += dt;

        return simState;
    }   
}
void Simulation::stop() {
    running = false;
}

void Simulation::start() {
    running = true;
}

void Simulation::addVehicle(RigidBodyState& initialState, Propulsion& propulsionSystem) {

    Vehicle new_vehicle = Vehicle(initialState, propulsionSystem);

    vehicles.push_back(new_vehicle);

    return;
}

bool Simulation::checkCollisions() {
    for (auto& vehicle : vehicles) {
        if (vehicle.getRigidBodyState().pos.z < 0) {
            return true;
        }
    }
    return false;
}