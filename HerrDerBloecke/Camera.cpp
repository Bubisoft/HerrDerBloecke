#include "Camera.h"

HdB::Camera::Camera(Device^ device, const Vector3% pos, const Vector3% lookAt)
: mDevice(device), mPosition(pos), mLookAt(lookAt)
{
    Speed = 0.005f;
}

Matrix HdB::Camera::ViewMatrix()
{
    return Matrix::LookAtRH(mPosition, mLookAt, Vector3::UnitZ);
}

Matrix HdB::Camera::ProjectionMatrix()
{
    return Matrix::PerspectiveFovRH(System::Math::PI / 4.f, mDevice->Viewport.Width * 1.f / mDevice->Viewport.Height,
        1.0f, 100.0f);
}

Vector3 HdB::Camera::Unproject2D(System::Drawing::Point pos)
{
    Matrix viewProj = ViewMatrix() * ProjectionMatrix();

    // Get our 3D cursor position in both near and far Camera plane
    Vector3 near = Vector3::Unproject(Vector3(pos.X, pos.Y, 0.f), mDevice->Viewport.X, mDevice->Viewport.Y,
        mDevice->Viewport.Width, mDevice->Viewport.Height, mDevice->Viewport.MinZ, mDevice->Viewport.MaxZ, viewProj);
    Vector3 far = Vector3::Unproject(Vector3(pos.X, pos.Y, 1.f), mDevice->Viewport.X, mDevice->Viewport.Y,
        mDevice->Viewport.Width, mDevice->Viewport.Height, mDevice->Viewport.MinZ, mDevice->Viewport.MaxZ, viewProj);

    // Create a X-Y-Plane that emulates our ground
    Plane xy(Vector3::Zero, Vector3::UnitZ);

    // Get the intersection of our cursor ray and the plane
    Vector3 result;
    Plane::Intersects(xy, near, far, result);
    return result;
}

void HdB::Camera::Move(const Vector3% change)
{
    mPosition += change;
    mLookAt += change;
}

void HdB::Camera::Rotate(const Vector2% change)
{
    Matrix rot = Matrix::Translation(-mLookAt) * Matrix::RotationZ(change.X * Speed) * Matrix::Translation(mLookAt);
    mPosition = Vector3::TransformCoordinate(mPosition, rot);
}
