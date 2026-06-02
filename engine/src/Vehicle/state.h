#ifndef STATE_H
#define STATE_H

#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix3x3.h"

struct RigidBodyState {
    // Physical properties
    Vector3 pos;    // Position [x, y, z]
    Vector3 vel;    // Velocity [x, y, z]
    double mass;    // Mass (changes as fuel burns)

    // Rotational properties
    Vector3 angularVel;     // (rad/s)
    Quaternion orientation; // Orientation

    // Inertia properties
    Matrix3x3 inertia;     // Inertia tensor 

    // Constructors
    RigidBodyState();
    RigidBodyState(Vector3 position, Vector3 velocity, Vector3 angularVelocity, Quaternion orientation, Matrix3x3 inertia, double mass);

    // Operators
    RigidBodyState operator+(const RigidBodyState& other) const;
    RigidBodyState operator*(double scalar) const;
};  

#endif // STATE_H