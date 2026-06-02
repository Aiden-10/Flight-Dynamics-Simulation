#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"

struct Quaternion {
    double w, x, y, z;

    Quaternion();
    Quaternion(double w, double x, double y, double z);

    // Core Math
    double magnitude() const;
    Quaternion normalize() const;
    Quaternion conjugate() const;
    Quaternion inverse() const;

    Vector3 getForwardVector() const;

    Quaternion getLaunchOrientation(double angle) const;

    // To Euler angles
    Vector3 toEuler() const;

    // Rotate a vector by this quaternion
    Vector3 rotate(const Vector3& v) const;

    // Operators
    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(double scalar) const;
    Quaternion operator/(double scalar) const;
};

#endif