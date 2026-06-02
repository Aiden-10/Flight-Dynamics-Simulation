#ifndef VEHICLE_H
#define VEHICLE_H

#include "../Environment/environment.h"
#include "aerodynamics.h"
#include "propulsion.h"
#include "state.h"

class Vehicle {
public:
    // Constructor
    Vehicle(const RigidBodyState& initialState, const Propulsion& propulsionSystem);

    RigidBodyState getDerivative(double currentTime);

    RigidBodyState& getRigidBodyState();
    void setState(const RigidBodyState& newState);

    // should be private but needed for logging
    Aerodynamics aerodynamics;
private:
    // RigidBodyState
    RigidBodyState state;

    // Physical Constants
    Vector3 centerOfGravity;

    Environment environment;
    Propulsion propulsion;
    Gravity gravity;
    // Guidance guidance;
    // Control control;
};

#endif // VEHICLE_H