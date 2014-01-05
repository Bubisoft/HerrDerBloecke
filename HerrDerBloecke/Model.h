#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace SlimDX::Direct3D9;

namespace HdB {
    // Forward declarations
    ref class Unit;
    value struct Submesh;

    ref class Model
    {
    public:
        /** Construct a new object to manage a specific model.
         *  The model will be loaded from the file models/<name>.HBM.
         *  The Modelname in the file will be ignored as we want to
         *  handle model creation and usage consistently.
         */
        Model(String^ name, Device^ device);

        /** Destructor */
        ~Model();

        /** Spawn a new model instance */
        void AddInstance(Unit^ unit);

        /** Draw all of our model instances */
        void Draw();

        property String^ Name;

    private:
        void LoadFromHBMFile(String^ filename);

        Device^ mDevice;
        List<Submesh>^ mMeshes;
        List<Unit^>^ mInstances;
    };
}
