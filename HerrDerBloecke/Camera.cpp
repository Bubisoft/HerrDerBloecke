#include "Camera.h"

#define ZOOM_DISTANCE_MIN 5.f
#define ZOOM_DISTANCE_MAX 100.f
#define ZOOM_SPEED 0.05f
#define KEYMOVE_SPEED 10.f

HdB::Camera::Camera(Device^ device, const Vector3% pos, const Vector3% lookAt)
: mDevice(device), mPosition(pos), mLookAt(lookAt), mKeyDown(false), mKeyUp(false),
  mKeyLeft(false), mKeyRight(false)
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
        1.0f, 5000.0f);
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

    if(delta > 0 && v.Length() > ZOOM_DISTANCE_MIN)
        mPosition = mLookAt + v * (1 - ZOOM_SPEED);
    if(delta < 0 && v.Length() < ZOOM_DISTANCE_MAX)
        mPosition = mLookAt + v * (1 + ZOOM_SPEED);
}

void HdB::Camera::SetMovementKey(Keys key, bool pressed)
{
    if (key == Keys::Up)
        mKeyUp = pressed;
    else if (key == Keys::Down)
        mKeyDown = pressed;
    else if (key == Keys::Left)
        mKeyLeft = pressed;
    else if (key == Keys::Right)
        mKeyRight = pressed;
}

void HdB::Camera::MoveByKeys(float speedMultiplier)
{
    Vector3 dir = Vector3::Zero;
    Vector3 forward = Vector3::Subtract(mLookAt, mPosition);
    forward.Z = 0.f;
    forward.Normalize();
    Vector3 left = Vector3::Cross(Vector3::UnitZ, forward);

    if (mKeyUp)
        dir += forward;
    if (mKeyDown)
        dir -= forward;
    if (mKeyLeft)
        dir += left;
    if (mKeyRight)
        dir -= left;

    if (dir != Vector3::Zero)
        Move(Vector3::Normalize(dir) * speedMultiplier * KEYMOVE_SPEED);
}
