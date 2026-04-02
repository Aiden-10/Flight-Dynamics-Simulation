#ifndef AERODYNAMICS_H
#define AERODYNAMICS_H

#include "../Math/Vector.h"
#include "../Vehicle/state.h"
#include "../Environment/environment.h"


class Aerodynamics {
public:
    // Aerodynamics.Data
    struct Data {
        Vector3 force;     // {Fx, Fy, Fz}
        Vector3 moment;    // {Mx, My, Mz}
        double alpha;      // Angle of Attack (radians)
    };

    // Compute aerodynamic forces and moments
    Aerodynamics::Data calculate(const RigidBodyState& state, Environment environment);

private:
};

#endif // AERODYNAMICS_H