#pragma once

#include "IDrawable.h"

using namespace System;
using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    // Forward Declarations
    ref class Camera;
    ref class Unit;
    ref class Map;
    ref class Model;
    ref class HealthBar;

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

        /** Returns a Model with the given name, try to load it from a file if we
         *  don't have it in our list of loaded drawables (load-on-demand).
         */
        //Model^ GetModel(String^ name);
        Model^ GetBlueModel(String^ name);
        Model^ GetRedModel(String^ name);

        /** Returns an Alphamodel with the given name, try to load it from a file if we
         *  don't have it in our list of loaded drawables (load-on-demand).
         */
        Model^ GetAlphaModel(String^ name);

        property HdB::Camera^ Camera {
            HdB::Camera^ get() { return mCamera; }
        }
        property HdB::Map^ Map {
            HdB::Map^ get() { return mMap; }
        }
        property List<Unit^>^ SelectedUnits {
            List<Unit^>^ get() { return mSelectedUnits; }
        }
        property Rectangle^ SelectionFrame;

    private:
        void ResetDevice();
        void DrawSelectionFrame();

        PresentParameters^ mParams;
        Direct3D^ m3D;
        Device^ mDevice;
        List<IDrawable^>^ mBlueDrawables;
        List<IDrawable^>^ mRedDrawables;
        List<IDrawable^>^ mAlphaDrawables;
        List<IDrawable^>^ mFonts;
        HdB::Camera^ mCamera;
        HdB::Map^ mMap;
        HealthBar^ mHealthBar;
        List<Unit^>^ mSelectedUnits;
        long long mFrameTime;
    };
}
