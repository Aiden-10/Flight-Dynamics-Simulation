#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "../Vehicle/state.h"

class Integrator {
public:
    // Constructor
    Integrator(double timeStep);

    // Advances time
    void step(RigidBodyState& currentState, const RigidBodyState& stateDerivative);

    // Change simulation speed
    void setDt(double dt);
    double getDt() const;
private:
    // Time step
    double dt;
};

#endif // INTEGRATOR_H