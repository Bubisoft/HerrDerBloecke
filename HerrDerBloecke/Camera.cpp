#include "Camera.h"

HdB::Camera::Camera(const Vector3& pos, const Vector3& lookAt)
    : mPosition(pos), mLookAt(lookAt)
{
    Speed = 1.f;
}

Matrix HdB::Camera::ViewMatrix()
{
    return Matrix::LookAtRH(mPosition, mLookAt, Vector3::UnitY);
}

void HdB::Camera::MoveCamera(const Vector3& change)
{
    mLookAt.X += change.X * (1/STD_SPEED) * Speed;
    mLookAt.Z += change.Z * (1/STD_SPEED) * Speed;
    mPosition.X += change.X * (1/STD_SPEED) * Speed;
    mPosition.Z += change.Z * (1/STD_SPEED) * Speed;
}
