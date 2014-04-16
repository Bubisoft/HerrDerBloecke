#pragma once

#include "IDrawable.h"

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    // Forward Declarations
    ref class Camera;
    ref class Unit;
    ref class Map;

    ref class Renderer
    {
    public:
        /** Constructor */
        Renderer();

        /** Destroy the whole world! */
        virtual ~Renderer();

        /** Initialize a Renderer that will render into the supplied Form Control Handle. */
        bool Init(Control^ target);

        /** Resize the BackBuffer of our Render Device to match the supplied size. */
        void Resize(const int% w, const int% h);

        /** Draw the next Frame. This is the render function that should be called in our
         *  main loop.
         */
        void Draw();

        /** Add a drawable object to our list of things we have to draw. */
        void AddDrawable(IDrawable^ drawable);

        /** Assign a unit to its model and spawn the instance. */
        void SpawnUnit(Unit^ unit);

        property HdB::Camera^ Camera {
            HdB::Camera^ get() { return mCamera; }
        }
        property HdB::Map^ Map {
            HdB::Map^ get() { return mMap; }
        }
        property bool Paused;

    private:
        void ResetDevice();

        PresentParameters^ mParams;
        Direct3D^ m3D;
        Device^ mDevice;
        List<IDrawable^>^ mDrawables;
        HdB::Camera^ mCamera;
        HdB::Map^ mMap;
    };
}
