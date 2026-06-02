#include "Vector.h"

#include <cmath>

Vector3::Vector3() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

Vector3::Vector3(double value) {
    this->x = value;
    this->y = value;
    this->z = value;
}

Vector3::Vector3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3::magnitude() const {
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3 Vector3::normalize() const {
    double mag = this->magnitude();
    if (mag == 0.0) {
        return Vector3(0.0, 0.0, 0.0);
    }
    return *this / mag;
}

double Vector3::dot(const Vector3& other) const {
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const {
    double new_x = this->y * other.z - this->z * other.y;
    double new_y = this->z * other.x - this->x * other.z;
    double new_z = this->x * other.y - this->y * other.x;

    return Vector3(new_x, new_y, new_z);
}

Vector3 Vector3::operator+(const Vector3& other) const {
    double new_x = this->x + other.x;
    double new_y = this->y + other.y;
    double new_z = this->z + other.z;
    
    return Vector3(new_x, new_y, new_z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    double new_x = this->x - other.x;
    double new_y = this->y - other.y;
    double new_z = this->z - other.z;

    return Vector3(new_x, new_y, new_z);
}

Vector3& Vector3::operator+=(const Vector3& other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;

    return *this;
}

Vector3 Vector3::operator*(double scalar) const {
    double new_x = this->x * scalar;
    double new_y = this->y * scalar;
    double new_z = this->z * scalar;

    return Vector3(new_x, new_y, new_z);
}

Vector3 Vector3::operator/(double scalar) const {
    double new_x = this->x / scalar;
    double new_y = this->y / scalar;
    double new_z = this->z / scalar;

    return Vector3(new_x, new_y, new_z);
}

Vector3& Vector3::operator*=(double scalar) {
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;

    return *this;
}

Vector3& Vector3::operator/=(double scalar) {
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;

    return *this;
}