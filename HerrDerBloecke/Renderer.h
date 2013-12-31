#pragma once

#include "Camera.h"

using namespace System::Windows::Forms;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;
using namespace SlimDX::Windows;

namespace HdB {
    ref class Renderer
    {
    public:
        /** Construct a Renderer that will render into the supplied Form Control Handle. */
        Renderer(Control^ target);

        /** Resize the BackBuffer of our Render Device to match the supplied size. */
        void Resize(const int& w, const int& h);

        /** Draw the next Frame. This is the render function that should be called in our
         *  main loop.
         */
        void Draw();

        /** Scrolls the Map / Moves the Camera */
        void MoveCamera(const Vector3& change);

        /** Sets the Speed of the Camera */
        void SetCameraSpeed(const float& speed);

    private:
        void ResetDevice();

        PresentParameters^ mParams;
        Direct3D^ m3D;
        Device^ mDevice;

        Camera^ mCamera;
    
	    Vector3 mScrollVector;
        Mesh^ mTeapot;
    };
}
