#include "Camera.h"

Camera::Camera(const Vector3& pos, const Vector3& lookAt)
    : mPosition(pos), mLookAt(lookAt)
{
}

Matrix Camera::ViewMatrix()
{
    return Matrix::LookAtRH(mPosition, mLookAt, Vector3::UnitY);
}
