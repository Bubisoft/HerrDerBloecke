#include "Camera.h"

/** Default reduction of CameraSpeed */

HdB::Camera::Camera(const Vector3% pos, const Vector3% lookAt)
    : mPosition(pos), mLookAt(lookAt)
{
    Speed = 0.005;
}

Matrix HdB::Camera::ViewMatrix()
{
    return Matrix::LookAtRH(mPosition, mLookAt, Vector3::UnitZ);
}

void HdB::Camera::Move(const Vector2% oldPos, const Vector2% newPos, const Vector2% window)
{
    Vector3 lookAtV = Vector3::Subtract(mLookAt, mPosition);
    Vector3 upVector = Vector3::Cross(Vector3::Cross(Vector3::UnitZ,lookAtV),lookAtV);

    float oldAngX = (oldPos.X/window.X - 0.5) * 0.78540;
    float oldAngY = (oldPos.Y/window.Y - 0.5) * 0.78540;
    float newAngX = (newPos.X/window.X - 0.5) * 0.78540;
    float newAngY = (newPos.Y/window.Y - 0.5) * 0.78540;

    Matrix m00 = Matrix::Translation(mLookAt) * Matrix::RotationAxis(upVector, -oldAngX) * Matrix::RotationAxis(Vector3::Cross(Vector3::UnitZ, lookAtV), -oldAngY) * Matrix::Translation(-mLookAt);
    Matrix m11 = Matrix::Translation(mLookAt) * Matrix::RotationAxis(upVector, -newAngX) * Matrix::RotationAxis(Vector3::Cross(Vector3::UnitZ, lookAtV), -newAngY) * Matrix::Translation(-mLookAt);

    Vector3 ray00 = Vector3::TransformCoordinate(lookAtV, m00);
    Vector3 ray11 = Vector3::TransformCoordinate(lookAtV, m11);

    Vector3 p00 = Vector3(-(mPosition.Z/ray00.Z)*ray00.X+mPosition.X, -(mPosition.Z/ray00.Z)*ray00.Y+mPosition.Y, 0);
    Vector3 p11 = Vector3(-(mPosition.Z/ray11.Z)*ray11.X+mPosition.X, -(mPosition.Z/ray11.Z)*ray11.Y+mPosition.Y, 0);

    Vector3 move = Vector3(p11.X-p00.X, p11.Y-p00.Y, 0);

    mPosition += move;
    mLookAt += move;
}

void HdB::Camera::Rotate(const Vector2% change)
{
    Matrix rot = Matrix::Translation(mLookAt) * Matrix::RotationZ(change.X*Speed) * Matrix::Translation(-mLookAt);
    mPosition = Vector3::TransformCoordinate(mPosition, rot);
}
