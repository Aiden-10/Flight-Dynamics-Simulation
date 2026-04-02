#include "vehicle.h"

Vehicle::Vehicle(const RigidBodyState& initialState, const Propulsion& propulsionSystem) {
    this->state = initialState;
    this->propulsion = propulsionSystem;
    this->centerOfGravity = Vector3(0.0);
    // TODO: aerodynamics
}

RigidBodyState Vehicle::getDerivative(double time) {
    RigidBodyState derivative;

    // 1. PROPULSION
    Propulsion::Data propData = this->propulsion.calculate(time);
    
    // 2. AERODYNAMICS
    Aerodynamics::Data aeroData = this->aerodynamics.calculate(state, this->environment);
    
    // 3. FORCE SUMMATION
    Vector3 globalThrust = state.orientation.rotate(propData.thrustVector);
    Vector3 gravityForce = this->environment.getGravity() * state.mass; 

    Vector3 totalForce = globalThrust + aeroData.force + gravityForce;
    
    derivative.vel = totalForce / state.mass;
    derivative.pos = state.vel;

    // 4. ANGULAR DYNAMICS
    Vector3 bodyTorque = state.orientation.inverse().rotate(aeroData.moment);

    derivative.angularVel.x = bodyTorque.x / state.inertia.m[0][0];
    derivative.angularVel.y = bodyTorque.y / state.inertia.m[1][1];
    derivative.angularVel.z = bodyTorque.z / state.inertia.m[2][2];

    // 5. ORIENTATION
    Quaternion w_quat(0, state.angularVel.x, state.angularVel.y, state.angularVel.z);
    derivative.orientation = (state.orientation * w_quat) * 0.5;
    

    // 6. MASS LOSS
    derivative.mass = propData.massFlowRate; 

    return derivative;
}

RigidBodyState& Vehicle::getRigidBodyState() {
    return this->state;
}

void Vehicle::setState(const RigidBodyState& newState) {
    this->state = newState;
}   