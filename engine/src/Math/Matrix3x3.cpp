#include "Matrix3x3.h"

Matrix3x3::Matrix3x3() {
    m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0;
    m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0;
}

Matrix3x3::Matrix3x3(double m00, double m01, double m02,
                     double m10, double m11, double m12,
                     double m20, double m21, double m22) {
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

Matrix3x3 Matrix3x3::Zero() const {
    return Matrix3x3(0.0, 0.0, 0.0,
                     0.0, 0.0, 0.0,
                     0.0, 0.0, 0.0);
}

Matrix3x3 Matrix3x3::Identity() const {
    return Matrix3x3(1.0, 0.0, 0.0,
                     0.0, 1.0, 0.0,
                     0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& other) const {
    return Matrix3x3(m[0][0] + other.m[0][0], m[0][1] + other.m[0][1], m[0][2] + other.m[0][2],
                     m[1][0] + other.m[1][0], m[1][1] + other.m[1][1], m[1][2] + other.m[1][2],
                     m[2][0] + other.m[2][0], m[2][1] + other.m[2][1], m[2][2] + other.m[2][2]);
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& o) const {
    return Matrix3x3(
        m[0][0]*o.m[0][0] + m[0][1]*o.m[1][0] + m[0][2]*o.m[2][0],
        m[0][0]*o.m[0][1] + m[0][1]*o.m[1][1] + m[0][2]*o.m[2][1],
        m[0][0]*o.m[0][2] + m[0][1]*o.m[1][2] + m[0][2]*o.m[2][2],

        m[1][0]*o.m[0][0] + m[1][1]*o.m[1][0] + m[1][2]*o.m[2][0],
        m[1][0]*o.m[0][1] + m[1][1]*o.m[1][1] + m[1][2]*o.m[2][1],
        m[1][0]*o.m[0][2] + m[1][1]*o.m[1][2] + m[1][2]*o.m[2][2],

        m[2][0]*o.m[0][0] + m[2][1]*o.m[1][0] + m[2][2]*o.m[2][0],
        m[2][0]*o.m[0][1] + m[2][1]*o.m[1][1] + m[2][2]*o.m[2][1],
        m[2][0]*o.m[0][2] + m[2][1]*o.m[1][2] + m[2][2]*o.m[2][2]
    );
}

double Matrix3x3::determinant() const {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}
Matrix3x3 Matrix3x3::transpose() const {
    return Matrix3x3(m[0][0], m[1][0], m[2][0],
                      m[0][1], m[1][1], m[2][1],
                      m[0][2], m[1][2], m[2][2]);
}
Matrix3x3 Matrix3x3::inverse() const {
    double det = determinant();
    if (det == 0.0) return Zero();
    double invDet = 1.0 / det;
    return Matrix3x3(
        (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet,
        (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet,
        (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet,
        (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet,
        (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet,
        (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet,
        (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet,
        (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet,
        (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet
    );
}
Matrix3x3 Matrix3x3::rotate(const Matrix3x3& rotationMatrix) const {
    return rotationMatrix * (*this);
}