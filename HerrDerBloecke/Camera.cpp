#include "Camera.h"

/** Default reduction of CameraSpeed */
#define STD_SPEED 30.0f

HdB::Camera::Camera(const Vector3& pos, const Vector3& lookAt)
    : mPosition(pos), mLookAt(lookAt)
{
    Speed = 1.f;
}

Matrix HdB::Camera::ViewMatrix()
{
    return Matrix::LookAtRH(mPosition, mLookAt, Vector3::UnitZ);
}

void HdB::Camera::Move(const Vector3& change)
{
    mLookAt.X += change.X * (1/STD_SPEED) * Speed;
    mLookAt.Y += change.Y * (1/STD_SPEED) * Speed;
    mPosition.X += change.X * (1/STD_SPEED) * Speed;
    mPosition.Y += change.Y * (1/STD_SPEED) * Speed;
}
