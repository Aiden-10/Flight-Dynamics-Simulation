#ifndef VECTOR_H
#define VECTOR_H

struct Vector3 {
    // Data
    double x, y, z;

    // Constructors
    Vector3();
    Vector3(double value);
    Vector3(double x, double y, double z);

    // Methods
    double magnitude() const;
    Vector3 normalize() const;
    double dot(const Vector3& other) const;
    Vector3 cross(const Vector3& other) const;

    // Vector Math
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);

    // Scalar Math
    Vector3 operator*(double scalar) const;
    Vector3 operator/(double scalar) const;
    Vector3& operator*=(double scalar);
    Vector3& operator/=(double scalar);
};

#endif // VECTOR_H