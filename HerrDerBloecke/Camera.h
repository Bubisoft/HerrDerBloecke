#pragma once

using namespace SlimDX;

ref class Camera
{
public:
    /** Create a new camera at the supplied position which will look at the
     *  given lookAt position.
     */
    Camera(const Vector3& pos, const Vector3& lookAt);

    /** Calculate the View Matrix for our Transform State in the Renderer. */
    Matrix ViewMatrix();

private:
    Vector3 mPosition;
    Vector3 mLookAt;
};
