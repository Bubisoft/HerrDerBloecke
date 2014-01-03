#pragma once

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    // Forward Declarations
    ref class Camera;
    ref class Model;
    ref class Unit;

    ref class Renderer
    {
    public:
        /** Construct a Renderer that will render into the supplied Form Control Handle. */
        Renderer(Control^ target);

        /** Destroy the whole world! */
        ~Renderer();

        /** Resize the BackBuffer of our Render Device to match the supplied size. */
        void Resize(const int& w, const int& h);

        /** Draw the next Frame. This is the render function that should be called in our
         *  main loop.
         */
        void Draw();

        /** Assign a unit to its model and spawn the instance */
        void SpawnUnit(Unit^ unit);

        property Camera^ Camera;

    private:
        void ResetDevice();

        PresentParameters^ mParams;
        Direct3D^ m3D;
        Device^ mDevice;
        List<Model^>^ mModels;
    };
}
