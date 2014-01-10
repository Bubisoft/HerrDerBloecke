#include "Camera.h"
#define ZOOM_SPEED 0.05

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
    Vector3 v = Vector3::Subtract(mLookAt, mPosition);
    Matrix rotY = Matrix::Identity;

    // get the angle between the camera and the z-axis
    float yAngle = Vector3::Dot(v, Vector3::UnitZ) / v.Length();
    yAngle = System::Math::Asin(yAngle);

    // we only want to rotate between 5 and 85 degrees in the y-direction
    if(change.Y < 0 && yAngle < -System::Math::PI / 36 && yAngle ||
        change.Y > 0 && yAngle > -System::Math::PI * 17 / 36)
        rotY = Matrix::RotationAxis(Vector3::Cross(Vector3::UnitZ, v), change.Y * Speed);

    Matrix rot = Matrix::Translation(-mLookAt) * Matrix::RotationZ(change.X * Speed) * rotY * Matrix::Translation(mLookAt);
    mPosition = Vector3::TransformCoordinate(mPosition, rot);
}

void HdB::Camera::Zoom(const int% delta)
{
    Vector3 v = Vector3::Subtract(mPosition, mLookAt);

    if(delta > 0 && v.Length() > 5)
        mPosition = mLookAt + v * (1 - ZOOM_SPEED);
    if(delta < 0 && v.Length() < 40)
        mPosition = mLookAt + v * (1 + ZOOM_SPEED);
}
