#include "aerodynamics.h"

#include <cmath>
#include <algorithm>

Aerodynamics::Data Aerodynamics::calculate(const RigidBodyState& state, Environment environment) {
    Data data;

    Vector3 windVelocity = environment.getWind(state.pos.z, 0.0);
    // windVelocity = Vector3(0.0, 0.0, 0.0);

    Vector3 airspeedVector = state.vel - windVelocity;

    double speed = airspeedVector.magnitude();
    if (speed < 0.1) return data;

    // 1. PHYSICAL CONFIG (Move to a struct later)
    const double area = 0.045;         // S
    const double length = 3.0;       // d (Reference length for damping)
    const double Cl_alpha = 4.0;     // Lift-slope
    const double Cd_base = 0.3;      // Zero-lift drag
    const double staticMargin = -0.4; // CG to CP distance
    const double Cmq = -2.5;         // Damping derivative (Negative = stable)

    // 2. ATMOSPHERE
    double density = environment.getAtmosphereDensity(state.pos.z);
    double q = 0.5 * density * speed * speed; // Dynamic pressure

    // 3. ORIENTATION
    Vector3 airspeedDir = airspeedVector.normalize();
    Vector3 bodyDir = state.orientation.getForwardVector();

    // 4. ANGLE OF ATTACK
    double dot = airspeedDir.dot(bodyDir);
    data.alpha = std::acos(std::clamp(dot, -1.0, 1.0));

    // 5. DRAG (Including Induced Drag)
    // Drag increases as Alpha increases: Cd = Cd0 + k*alpha^2
    double inducedDragCoeff = 0.1 * (data.alpha * data.alpha);
    double totalCd = Cd_base + inducedDragCoeff;
    data.force = airspeedDir * (-q * area * totalCd);

    // 6. LIFT & MOMENTS
    if (data.alpha > 0.001) {
        double liftMag = q * area * Cl_alpha * data.alpha;
        
        // Perpendicular direction toward the nose
        Vector3 liftDir = (bodyDir - (airspeedDir * dot)).normalize();
        
        // Static Stability Moment (The "Spring")
        data.force = data.force + (liftDir * liftMag);
        Vector3 leverArm = bodyDir * staticMargin;
        data.moment = leverArm.cross(liftDir * liftMag);
    }

    // 7. AERODYNAMIC DAMPING (The "Shock Absorber")
    // M_damping = q * S * d * (d / 2V) * Cmq * omega
    // This opposes rotation and prevents the "Death Tumble"
    if (speed > 1.0) {
        double dampingFactor = q * area * length * (length / (2.0 * speed)) * Cmq;
        Vector3 worldDampingTorque = state.orientation.rotate(state.angularVel * dampingFactor);
        data.moment = data.moment + worldDampingTorque;
    }

    return data;
}