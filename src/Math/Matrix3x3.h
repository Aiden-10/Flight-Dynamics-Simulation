#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "Vector.h"

struct Matrix3x3 {
    double m[3][3];

    // Constructors
    Matrix3x3();
    Matrix3x3(double m00, double m01, double m02,
              double m10, double m11, double m12,
              double m20, double m21, double m22);

    
    Matrix3x3 Zero() const;
    Matrix3x3 Identity() const;

    // Basic Operations
    Matrix3x3 operator+(const Matrix3x3& other) const;
    Matrix3x3 operator*(const Matrix3x3& other) const;
    Matrix3x3 operator*(double scalar) const;
    
    // Matrix-Vector Multiplication
    Vector3 operator*(const Vector3& v) const;

    
    double determinant() const;
    Matrix3x3 transpose() const;
    Matrix3x3 inverse() const;

    Matrix3x3 rotate(const Matrix3x3& rotationMatrix) const;
};

#endif // MATRIX3X3_H