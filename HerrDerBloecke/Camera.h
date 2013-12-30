#pragma once

/** Default reduction of CameraSpeed */
#define STD_SPEED 30.0f

using namespace SlimDX;

ref class Camera
{
public:
    /** Create a new camera at the supplied position which will look at the
     *  given lookAt position.
     */
    Camera(const Vector3& pos, const Vector3& lookAt);

	/** Moves the Camera */
	void MoveCamera(Vector3& change);

    /** Calculate the View Matrix for our Transform State in the Renderer. */
    Matrix ViewMatrix();

	property float mCameraSpeed{
		void set(float _speed)
		{
			Speed=_speed;
		}
		float get()
		{
			return Speed;
		}
	};

private:
	float Speed; //Speed with wich the camera is moved
    Vector3 mPosition;	//Position of the Camera
    Vector3 mLookAt;    //Position the Camera is looking at
};
