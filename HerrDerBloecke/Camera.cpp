#include "Camera.h"

Camera::Camera(const Vector3& pos, const Vector3& lookAt)
    : mPosition(pos), mLookAt(lookAt)
{
	Speed=1.f;
}

Matrix Camera::ViewMatrix()
{
    return Matrix::LookAtRH(mPosition, mLookAt, Vector3::UnitY);
}

void Camera::MoveCamera(Vector3& change)
{
	change.X*=(1/STD_SPEED)*Speed;
	change.Z*=(1/STD_SPEED)*Speed;

	mLookAt=mLookAt+change;
	mPosition=mPosition+change;
}
