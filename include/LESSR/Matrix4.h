#pragma once
#include <string>

namespace LESSR
{
struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix4
{
    float m[16];

    Matrix4();
    Matrix4(const float* m);
    void set(const float* m);
    void setIdentity();
    void zero();

    static void createLookAt(const Vector3& eyePosition, const Vector3& targetPosition,
                             const Vector3& up, Matrix4* dst);
                      
    static void createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
                             float targetPositionX, float targetPositionY, float targetPositionZ,
                             float upX, float upY, float upZ, Matrix4* dst);

    static void createPerspective(float fieldOfView, float aspectRatio,
                                  float zNearPlane, float zFarPlane, Matrix4* dst);

    static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix4* dst);

    static void createOrthographicRect(float left, float right, float bottom, float top,
                                            float zNearPlane, float zFarPlane, Matrix4* dst);

    static void createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                                const Vector3& cameraUpVector, Matrix4* dst);

    static void createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                                const Vector3& cameraUpVector, const Vector3& cameraForwardVector,
                                Matrix4* dst);

    static void createScale(const Vector3& scale, Matrix4* dst);
    static void createScale(float xScale, float yScale, float zScale, Matrix4* dst);

    static void createRotation(const Vector3& axis, float angle, Matrix4* dst);
    static void createRotationX(float angle, Matrix4* dst);
    static void createRotationY(float angle, Matrix4* dst);
    static void createRotationZ(float angle, Matrix4* dst);

    static void createFromEuler(float yaw, float pitch, float roll, Matrix4* dst);
    static void createTranslation(const Vector3& translation, Matrix4* dst);
    static void createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix4* dst);

    void add(const Matrix4& m);
    static void add(const Matrix4& m1, const Matrix4& m2, Matrix4* dst);

    void multiply(float scalar);
    void multiply(float scalar, Matrix4* dst) const;
    static void multiply(const Matrix4& m, float scalar, Matrix4* dst);

    void multiply(const Matrix4& m);
    static void multiply(const Matrix4& m1, const Matrix4& m2, Matrix4* dst);

    void subtract(const Matrix4& m);
    void subtract(const Matrix4& m1, const Matrix4& m2, Matrix4* dst);

    float determinant() const;

    void getUpVector(Vector3* dst) const;
    void getDownVector(Vector3* dst) const;
    void getLeftVector(Vector3* dst) const;
    void getRightVector(Vector3* dst) const;
    void getForwardVector(Vector3* dst) const;
    void getBackVector(Vector3* dst) const;

    bool invert();
    bool invert(Matrix4* dst) const;
    bool isIdentity() const;

    void negate();
    void negate(Matrix4* dst) const;

    void rotate(const Vector3& axis, float angle);
    void rotate(const Vector3& axis, float angle, Matrix4* dst) const;
    void rotateX(float angle);
    void rotateX(float angle, Matrix4* dst) const;
    void rotateY(float angle);
    void rotateY(float angle, Matrix4* dst) const;
    void rotateZ(float angle);
    void rotateZ(float angle, Matrix4* dst) const;

    void scale(float value);
    void scale(float value, Matrix4* dst) const;
    void scale(float xScale, float yScale, float zScale);
    void scale(float xScale, float yScale, float zScale, Matrix4* dst) const;
    void scale(const Vector3& s);
    void scale(const Vector3& s, Matrix4* dst) const;

    void transformVector(Vector4* vector) const;
    void transformVector(const Vector4& vector, Vector4* dst) const;
    void transformVector(float x, float y, float z, float w, Vector4* dst) const;

    void translate(float x, float y, float z);
    void translate(float x, float y, float z, Matrix4* dst) const;
    void translate(const Vector3& t);
    void translate(const Vector3& t, Matrix4* dst) const;
    void transpose();
    void transpose(Matrix4* dst) const;

    Matrix4 operator+(const Matrix4& v) const;
    Matrix4 operator-(const Matrix4& v) const;
    Matrix4 operator*(const Matrix4& v) const;
    Matrix4& operator+=(const Matrix4& v);
    Matrix4& operator-=(const Matrix4& v);
    Matrix4& operator*=(const Matrix4& v);

    Matrix4 operator*(float f);
    Matrix4& operator*=(float f);

    Vector4 operator*(const Vector4& v) const;

    float& operator()(uint8_t row, uint8_t col);
    float operator()(uint8_t row, uint8_t col) const;

};

inline float& Matrix4::operator()(uint8_t row, uint8_t col)
{
    return m[4*row + col];
}

inline float Matrix4::operator()(uint8_t row, uint8_t col) const
{
    return m[4*row + col];
}

}