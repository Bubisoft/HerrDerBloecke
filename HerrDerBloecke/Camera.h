#pragma once

using namespace SlimDX;
using namespace SlimDX::Direct3D9;
using namespace System::Windows::Forms;

namespace HdB {
    ref class Camera
    {
    public:
        /** Create a new camera at the supplied position which will look at the
         *  given lookAt position.
         */
        Camera(Device^ device, const Vector3% pos, const Vector3% lookAt);

        /** Calculate the View Matrix for our Transform State in the Renderer. */
        Matrix ViewMatrix();

        /** Calculate the Projection Matrix. */
        Matrix ProjectionMatrix();

        /** Project the 2D mouse pointer location onto our ground plane. */
        Vector3 Unproject2D(System::Drawing::Point pos);

        /** Move the Camera by the given direction. */
        void Move(const Vector3% change);

        /** Rotate the Camera according to the change of the cursor position */
        void Rotate(const Vector2% change);

        /** Zoom the Camera. */
        void Zoom(const int% delta);

        /** Register when movement keys are pressed or released. */
        void SetMovementKey(Keys key, bool pressed);

        /** Move the camera according to the arrow keys that are pressed. */
        void MoveByKeys();

        /** Defines the camera rotation speed. */
        property float Speed;

        property Vector3 LookAt{
            Vector3 get(){ return mLookAt; }
        };
    
    private:
        Device^ mDevice;
        Vector3 mPosition;
        Vector3 mLookAt;

        bool mKeyUp;
        bool mKeyDown;
        bool mKeyLeft;
        bool mKeyRight;
    };
}
