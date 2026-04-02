#include "integrator.h"


Integrator::Integrator(double timeStep) {
    this->dt = timeStep;
}

void Integrator::step(RigidBodyState& currentState, const RigidBodyState& stateDerivative) {
    // Calculate the new pos, vel, and mass at a given timestep
    currentState = currentState + (stateDerivative * this->dt);

    // Normalize the orientation quaternion to prevent drift
    currentState.orientation = currentState.orientation.normalize();
    
    return;
}

void Integrator::setDt(double dt) {
    this->dt = dt;
}

double Integrator::getDt() const {
    return this->dt;
}
