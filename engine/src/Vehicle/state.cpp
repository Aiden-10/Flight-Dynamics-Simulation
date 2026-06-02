#include "state.h"


RigidBodyState::RigidBodyState() {
    this->pos = Vector3();
    this->vel = Vector3();
    this->angularVel = Vector3();
    this->orientation = Quaternion();
    this->inertia = Matrix3x3();
    this->mass = 0.0;
}

RigidBodyState::RigidBodyState(Vector3 position, Vector3 velocity, Vector3 angularVelocity, Quaternion orientation, Matrix3x3 inertia, double mass) {
    this->pos = position;
    this->vel = velocity;
    this->angularVel = angularVelocity;
    this->orientation = orientation;
    this->inertia = inertia;
    this->mass = mass;
}

RigidBodyState RigidBodyState::operator+(const RigidBodyState& other) const {
    RigidBodyState result;
    result.pos = this->pos + other.pos;
    result.vel = this->vel + other.vel;
    result.angularVel = this->angularVel + other.angularVel;
    result.mass = this->mass + other.mass;

    // Add the orientation derivatives (quaternions) and normalize the result
    result.orientation = (this->orientation + other.orientation).normalize();

    result.inertia = this->inertia;
    
    return result;
}

RigidBodyState RigidBodyState::operator*(double scalar) const {
    RigidBodyState result;
    result.pos = this->pos * scalar;
    result.vel = this->vel * scalar;
    result.angularVel = this->angularVel * scalar;
    result.orientation = this->orientation * scalar; 
    result.mass = this->mass * scalar;
    return result;
}