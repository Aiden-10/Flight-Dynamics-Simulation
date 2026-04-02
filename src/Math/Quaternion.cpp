#include "Quaternion.h"
#include <cmath>

Quaternion::Quaternion() {
    this->w = 1.0;
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

Quaternion::Quaternion(double w, double x, double y, double z) {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

double Quaternion::magnitude() const {
    return std::sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::normalize() const {
    double mag = magnitude();
    if (mag > 0.000001) {
        return *this / mag;
    }
    return Quaternion(1, 0, 0, 0); // Return identity if zero-length
}

Quaternion Quaternion::conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::inverse() const {
    double magSq = w * w + x * x + y * y + z * z;
    if (magSq > 0.000001) {
        return conjugate() / magSq;
    }
    return Quaternion(1, 0, 0, 0);
}

Vector3 Quaternion::getForwardVector() const {
    // Standard: X is Forward, Z is Up
    return rotate(Vector3(1.0, 0.0, 0.0));
}

Quaternion Quaternion::getLaunchOrientation(double angle) const {
    double rad = -angle * (3.14159265 / 180.0);
    Vector3 pitchAxis(0.0, 1.0, 0.0); 
    
    double w = cos(rad / 2.0);
    double s = sin(rad / 2.0);
    
    return Quaternion(w, pitchAxis.x * s, pitchAxis.y * s, pitchAxis.z * s).normalize();
}

Vector3 Quaternion::toEuler() const {
    Vector3 euler;

    double sinr_cosp = 2.0 * (w * x + y * z);
    double cosr_cosp = 1.0 - 2.0 * (x * x + y * y);
    euler.z = std::atan2(sinr_cosp, cosr_cosp); 

    double sinp = 2.0 * (w * y - z * x);
    if (std::abs(sinp) >= 1)
        euler.x = std::copysign(3.14159 / 2, sinp);
    else
        euler.x = std::asin(sinp); 

    double siny_cosp = 2.0 * (w * z + x * y);
    double cosy_cosp = 1.0 - 2.0 * (y * y + z * z);
    euler.y = std::atan2(siny_cosp, cosy_cosp);

    return euler;
}

Vector3 Quaternion::rotate(const Vector3& v) const {
    Vector3 vector(x, y, z);
    
    Vector3 t = vector.cross(v) * 2.0;
    
    return v + (t * w) + vector.cross(t);
}

// Used to add the orientation derivative (dq/dt) to the current orientation
Quaternion Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
}

// This assumes w is the scalar part and (x,y,z) is the vector part
Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w * q.w - x * q.x - y * q.y - z * q.z,
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w
    );
}

Quaternion Quaternion::operator*(double scalar) const {
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
}

Quaternion Quaternion::operator/(double scalar) const {
    return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
}