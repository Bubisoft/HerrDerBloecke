#pragma once

/** Default reduction of CameraSpeed */
#define STD_SPEED 30.0f

using namespace SlimDX;

namespace HdB {
    ref class Camera
    {
    public:
        /** Create a new camera at the supplied position which will look at the
         *  given lookAt position.
         */
        Camera(const Vector3& pos, const Vector3& lookAt);

        /** Moves the Camera */
        void MoveCamera(const Vector3& change);

        /** Calculate the View Matrix for our Transform State in the Renderer. */
        Matrix ViewMatrix();

        /** Defines the camera movement speed. */
        property float Speed;

    private:
        // Position of the Camera
        Vector3 mPosition;

        // Position the Camera is looking at
        Vector3 mLookAt;
    };
}
