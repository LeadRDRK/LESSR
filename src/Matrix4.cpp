// Portions of this file are derived from gameplay3d's source code (http://www.gameplay3d.io)
/*
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include <LESSR/Matrix4.h>
#include <LESSR/Vector2.h>
#include <LESSR/Vector3.h>
#include <LESSR/Vector4.h>
#include <LESSR/MathUtils.h>
#include <cstring>

namespace LESSR
{

static const float MATRIX_IDENTITY[16] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};
#define MATRIX_SIZE (sizeof(float) * 16)

Matrix4::Matrix4()
: Matrix4(MATRIX_IDENTITY)
{}

Matrix4::Matrix4(const float* _m)
{
    set(_m);
}

void Matrix4::set(const float* _m)
{
    memcpy(m, _m, MATRIX_SIZE);
}

void Matrix4::setIdentity()
{
    memcpy(m, MATRIX_IDENTITY, MATRIX_SIZE);
}

void Matrix4::zero()
{
    memset(m, 0, MATRIX_SIZE);
}

void Matrix4::createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix4* dst)
{
    createLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
                 targetPosition.x, targetPosition.y, targetPosition.z,
                 up.x, up.y, up.z, dst);
}

void Matrix4::createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
                           float targetPositionX, float targetPositionY, float targetPositionZ,
                           float upX, float upY, float upZ, Matrix4* dst)
{
    Vector3 eye(eyePositionX, eyePositionY, eyePositionZ);
    Vector3 target(targetPositionX, targetPositionY, targetPositionZ);
    Vector3 up(upX, upY, upZ);
    up.normalize();

    Vector3 zaxis = eye - target;
    zaxis.normalize();

    Vector3 xaxis = up.cross(zaxis);
    xaxis.normalize();

    Vector3 yaxis = zaxis.cross(xaxis);
    yaxis.normalize();

    dst->m[0] = xaxis.x;
    dst->m[1] = yaxis.x;
    dst->m[2] = zaxis.x;
    dst->m[3] = 0.0f;

    dst->m[4] = xaxis.y;
    dst->m[5] = yaxis.y;
    dst->m[6] = zaxis.y;
    dst->m[7] = 0.0f;

    dst->m[8] = xaxis.z;
    dst->m[9] = yaxis.z;
    dst->m[10] = zaxis.z;
    dst->m[11] = 0.0f;

    dst->m[12] = -xaxis.dot(eye);
    dst->m[13] = -yaxis.dot(eye);
    dst->m[14] = -zaxis.dot(eye);
    dst->m[15] = 1.0f;
}

void Matrix4::createPerspective(float fieldOfView, float aspectRatio,
                                float zNearPlane, float zFarPlane, Matrix4* dst)
{
    float f_n = 1.0f / (zFarPlane - zNearPlane);
    float theta = MATH_DEG_TO_RAD(fieldOfView) * 0.5f;
    if (fabs(fmod(theta, MATH_PIOOVER2)) < MATH_EPSILON)
        return;
    float divisor = tan(theta);
    float factor = 1.0f / divisor;

    memset(dst, 0, MATRIX_SIZE);
    dst->m[0] = (1.0f / aspectRatio) * factor;
    dst->m[5] = factor;
    dst->m[10] = (-(zFarPlane + zNearPlane)) * f_n;
    dst->m[11] = -1.0f;
    dst->m[14] = -2.0f * zFarPlane * zNearPlane * f_n;
}

void Matrix4::createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix4* dst)
{
    memset(dst, 0, MATRIX_SIZE);
    dst->m[0] = 2 / width;
    dst->m[5] = -2 / height;
    dst->m[10] = 1 / (zNearPlane - zFarPlane);
    dst->m[12] = -1;
    dst->m[13] = 1;
    dst->m[14] = zNearPlane / (zNearPlane - zFarPlane);
    dst->m[15] = 1;
}

void Matrix4::createOrthographicRect(float left, float right, float bottom, float top,
                                     float zNearPlane, float zFarPlane, Matrix4* dst)
{
    memset(dst, 0, MATRIX_SIZE);
    dst->m[0] = 2 / (right - left);
    dst->m[5] = 2 / (top - bottom);
    dst->m[12] = (left + right) / (left - right);
    dst->m[10] = 1 / (zNearPlane - zFarPlane);
    dst->m[13] = (top + bottom) / (bottom - top);
    dst->m[14] = zNearPlane / (zNearPlane - zFarPlane);
    dst->m[15] = 1;
}

static void createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition,
                                  const Vector3& cameraUpVector, const Vector3* cameraForwardVector,
                                  Matrix4* dst)
{
    Vector3 delta = cameraPosition - objectPosition;
    bool isSufficientDelta = pow(delta.length(), 2) > MATH_EPSILON;

    memcpy(dst->m, MATRIX_IDENTITY, MATRIX_SIZE);
    dst->m[3] = objectPosition.x;
    dst->m[7] = objectPosition.y;
    dst->m[11] = objectPosition.z;

    // As per the contracts for the 2 variants of createBillboard, we need
    // either a safe default or a sufficient distance between object and camera.
    if (cameraForwardVector || isSufficientDelta)
    {
        Vector3 target = isSufficientDelta ? cameraPosition : (objectPosition - *cameraForwardVector);

        // A billboard is the inverse of a lookAt rotation
        Matrix4 lookAt;
        Matrix4::createLookAt(objectPosition, target, cameraUpVector, &lookAt);
        dst->m[0] = lookAt.m[0];
        dst->m[1] = lookAt.m[4];
        dst->m[2] = lookAt.m[8];
        dst->m[4] = lookAt.m[1];
        dst->m[5] = lookAt.m[5];
        dst->m[6] = lookAt.m[9];
        dst->m[8] = lookAt.m[2];
        dst->m[9] = lookAt.m[6];
        dst->m[10] = lookAt.m[10];
    }
}

void Matrix4::createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                             const Vector3& cameraUpVector, Matrix4* dst)
{
    createBillboardHelper(objectPosition, cameraPosition, cameraUpVector, NULL, dst);
}

void Matrix4::createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                             const Vector3& cameraUpVector, const Vector3& cameraForwardVector,
                             Matrix4* dst)
{
    createBillboardHelper(objectPosition, cameraPosition, cameraUpVector, &cameraForwardVector, dst);
}

void Matrix4::createScale(const Vector3& scale, Matrix4* dst)
{
    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[0] = scale.x;
    dst->m[5] = scale.y;
    dst->m[10] = scale.z;
}

void Matrix4::createScale(float xScale, float yScale, float zScale, Matrix4* dst)
{
    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[0] = xScale;
    dst->m[5] = yScale;
    dst->m[10] = zScale;
}

void Matrix4::createRotation(const Vector3& axis, float angle, Matrix4* dst)
{
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    // Make sure the input axis is normalized.
    float n = x*x + y*y + z*z;
    if (n != 1.0f)
    {
        // Not normalized.
        n = sqrt(n);
        // Prevent divide too close to zero.
        if (n > 0.000001f)
        {
            n = 1.0f / n;
            x *= n;
            y *= n;
            z *= n;
        }
    }

    float c = cos(angle);
    float s = sin(angle);

    float t = 1.0f - c;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float txy = tx * y;
    float txz = tx * z;
    float tyz = ty * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;

    dst->m[0] = c + tx*x;
    dst->m[1] = txy + sz;
    dst->m[2] = txz - sy;
    dst->m[3] = 0.0f;

    dst->m[4] = txy - sz;
    dst->m[5] = c + ty*y;
    dst->m[6] = tyz + sx;
    dst->m[7] = 0.0f;

    dst->m[8] = txz + sy;
    dst->m[9] = tyz - sx;
    dst->m[10] = c + tz*z;
    dst->m[11] = 0.0f;

    dst->m[12] = 0.0f;
    dst->m[13] = 0.0f;
    dst->m[14] = 0.0f;
    dst->m[15] = 1.0f;
}

void Matrix4::createRotationX(float angle, Matrix4* dst)
{
    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[5]  = c;
    dst->m[6]  = s;
    dst->m[9]  = -s;
    dst->m[10] = c;
}

void Matrix4::createRotationY(float angle, Matrix4* dst)
{
    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[0]  = c;
    dst->m[2]  = -s;
    dst->m[8]  = s;
    dst->m[10] = c;
}

void Matrix4::createRotationZ(float angle, Matrix4* dst)
{
    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[0] = c;
    dst->m[1] = s;
    dst->m[4] = -s;
    dst->m[5] = c;
}

void Matrix4::createFromEuler(float yaw, float pitch, float roll, Matrix4* dst)
{
	memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);
	
	dst->rotateY(yaw);
	dst->rotateX(pitch);
	dst->rotateZ(roll);
}

void Matrix4::createTranslation(const Vector3& translation, Matrix4* dst)
{
    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[12] = translation.x;
    dst->m[13] = translation.y;
    dst->m[14] = translation.z;
}

void Matrix4::createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix4* dst)
{
    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[12] = xTranslation;
    dst->m[13] = yTranslation;
    dst->m[14] = zTranslation;
}

void Matrix4::add(const Matrix4& m)
{
    add(*this, m, this);
}

void Matrix4::add(const Matrix4& m1, const Matrix4& m2, Matrix4* dst)
{
    for (int i = 0; i < 16; ++i)
        dst->m[i] = m1.m[i] + m2.m[i];
}

float Matrix4::determinant() const
{
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];

    // Calculate the determinant.
    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

void Matrix4::getUpVector(Vector3* dst) const
{
    dst->x = m[4];
    dst->y = m[5];
    dst->z = m[6];
}

void Matrix4::getDownVector(Vector3* dst) const
{
    dst->x = -m[4];
    dst->y = -m[5];
    dst->z = -m[6];
}

void Matrix4::getLeftVector(Vector3* dst) const
{
    dst->x = -m[0];
    dst->y = -m[1];
    dst->z = -m[2];
}

void Matrix4::getRightVector(Vector3* dst) const
{
    dst->x = m[0];
    dst->y = m[1];
    dst->z = m[2];
}

void Matrix4::getForwardVector(Vector3* dst) const
{
    dst->x = -m[8];
    dst->y = -m[9];
    dst->z = -m[10];
}

void Matrix4::getBackVector(Vector3* dst) const
{
    dst->x = m[8];
    dst->y = m[9];
    dst->z = m[10];
}

bool Matrix4::invert()
{
    return invert(this);
}

bool Matrix4::invert(Matrix4* dst) const
{
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];

    // Calculate the determinant.
    float det = (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);

    // Close to zero, can't invert.
    if (fabs(det) <= 2e-37f)
        return false;

    // Support the case where m == dst.
    Matrix4 inverse;
    inverse.m[0]  = m[5] * b5 - m[6] * b4 + m[7] * b3;
    inverse.m[1]  = -m[1] * b5 + m[2] * b4 - m[3] * b3;
    inverse.m[2]  = m[13] * a5 - m[14] * a4 + m[15] * a3;
    inverse.m[3]  = -m[9] * a5 + m[10] * a4 - m[11] * a3;

    inverse.m[4]  = -m[4] * b5 + m[6] * b2 - m[7] * b1;
    inverse.m[5]  = m[0] * b5 - m[2] * b2 + m[3] * b1;
    inverse.m[6]  = -m[12] * a5 + m[14] * a2 - m[15] * a1;
    inverse.m[7]  = m[8] * a5 - m[10] * a2 + m[11] * a1;

    inverse.m[8]  = m[4] * b4 - m[5] * b2 + m[7] * b0;
    inverse.m[9]  = -m[0] * b4 + m[1] * b2 - m[3] * b0;
    inverse.m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
    inverse.m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;

    inverse.m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
    inverse.m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
    inverse.m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
    inverse.m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;

    multiply(inverse, 1.0f / det, dst);

    return true;
}

bool Matrix4::isIdentity() const
{
    return (memcmp(m, MATRIX_IDENTITY, MATRIX_SIZE) == 0);
}

void Matrix4::multiply(float scalar)
{
    multiply(*this, scalar, this);
}

void Matrix4::multiply(float scalar, Matrix4* dst) const
{
    multiply(*this, scalar, dst);
}

void Matrix4::multiply(const Matrix4& m, float scalar, Matrix4* dst)
{
    for (int i = 0; i < 16; ++i)
        dst->m[i] = m.m[i] * scalar;
}

void Matrix4::multiply(const Matrix4& m)
{
    multiply(*this, m, this);
}

void Matrix4::multiply(const Matrix4& m1, const Matrix4& m2, Matrix4* dst)
{
    float res[16];
    memset(res, 0, MATRIX_SIZE);

    for (int i = 0; i < 4; i++)
        for (int k = 0; k < 4; k++)
            for (int j = 0; j < 4; j++)
                res[4*i+j] += m1.m[4*i+k] * m2.m[4*k+j];
    
    memcpy(dst, res, MATRIX_SIZE);
}

void Matrix4::negate()
{
    negate(this);
}

void Matrix4::negate(Matrix4* dst) const
{
    for (int i = 0; i < 16; ++i)
        dst->m[i] = -m[i];
}

void Matrix4::rotate(const Vector3& axis, float angle)
{
    rotate(axis, angle, this);
}

void Matrix4::rotate(const Vector3& axis, float angle, Matrix4* dst) const
{
    Matrix4 r;
    createRotation(axis, angle, &r);
    multiply(*this, r, dst);
}

void Matrix4::rotateX(float angle)
{
    rotateX(angle, this);
}

void Matrix4::rotateX(float angle, Matrix4* dst) const
{
    Matrix4 r;
    createRotationX(angle, &r);
    multiply(*this, r, dst);
}

void Matrix4::rotateY(float angle)
{
    rotateY(angle, this);
}

void Matrix4::rotateY(float angle, Matrix4* dst) const
{
    Matrix4 r;
    createRotationY(angle, &r);
    multiply(*this, r, dst);
}

void Matrix4::rotateZ(float angle)
{
    rotateZ(angle, this);
}

void Matrix4::rotateZ(float angle, Matrix4* dst) const
{
    Matrix4 r;
    createRotationZ(angle, &r);
    multiply(*this, r, dst);
}

void Matrix4::scale(float value)
{
    scale(value, this);
}

void Matrix4::scale(float value, Matrix4* dst) const
{
    scale(value, value, value, dst);
}

void Matrix4::scale(float xScale, float yScale, float zScale)
{
    scale(xScale, yScale, zScale, this);
}

void Matrix4::scale(float xScale, float yScale, float zScale, Matrix4* dst) const
{
    Matrix4 s;
    createScale(xScale, yScale, zScale, &s);
    multiply(*this, s, dst);
}

void Matrix4::scale(const Vector3& s)
{
    scale(s.x, s.y, s.z, this);
}

void Matrix4::scale(const Vector3& s, Matrix4* dst) const
{
    scale(s.x, s.y, s.z, dst);
}

void Matrix4::subtract(const Matrix4& m)
{
    subtract(*this, m, this);
}

void Matrix4::subtract(const Matrix4& m1, const Matrix4& m2, Matrix4* dst)
{
    for (int i = 0; i < 16; ++i)
        dst->m[i] = m1.m[i] - m2.m[i];
}

void Matrix4::transformVector(Vector4* vector) const
{
    transformVector(vector->x, vector->y, vector->z, vector->w, vector);
}

void Matrix4::transformVector(const Vector4& vector, Vector4* dst) const
{
    transformVector(vector.x, vector.y, vector.z, vector.w, dst);
}

void Matrix4::transformVector(float x, float y, float z, float w, Vector4* dst) const
{
    dst->x = x * m[0] + y * m[4] + z * m[8] + w * m[12];
    dst->y = x * m[1] + y * m[5] + z * m[9] + w * m[13];
    dst->z = x * m[2] + y * m[6] + z * m[10] + w * m[14];
    dst->w = x * m[3] + y * m[7] + z * m[11] + w * m[15];
}

void Matrix4::translate(float x, float y, float z)
{
    translate(x, y, z, this);
}

void Matrix4::translate(float x, float y, float z, Matrix4* dst) const
{
    Matrix4 t;
    createTranslation(x, y, z, &t);
    multiply(*this, t, dst);
}

void Matrix4::translate(const Vector3& t)
{
    translate(t.x, t.y, t.z, this);
}

void Matrix4::translate(const Vector3& t, Matrix4* dst) const
{
    translate(t.x, t.y, t.z, dst);
}

void Matrix4::transpose()
{
    transpose(this);
}

void Matrix4::transpose(Matrix4* dst) const
{
    float t[16] = {
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    };
    memcpy(dst, t, MATRIX_SIZE);
}

Matrix4 Matrix4::operator+(const Matrix4& v) const
{
    Matrix4 res(*this);
    res.add(v);
    return res;
}

Matrix4 Matrix4::operator-(const Matrix4& v) const
{
    Matrix4 res(*this);
    res.subtract(v);
    return res;
}

Matrix4 Matrix4::operator*(const Matrix4& v) const
{
    Matrix4 res(*this);
    res.multiply(v);
    return res;
}

Matrix4& Matrix4::operator+=(const Matrix4& v)
{
    add(v);
    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& v)
{
    subtract(v);
    return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& v)
{
    multiply(v);
    return *this;
}

Matrix4 Matrix4::operator*(float f)
{
    Matrix4 res(*this);
    res.multiply(f);
    return res;
}

Matrix4& Matrix4::operator*=(float f)
{
    multiply(f);
    return *this;
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
    Vector4 res;
    transformVector(v, &res);
    return res;
}

}