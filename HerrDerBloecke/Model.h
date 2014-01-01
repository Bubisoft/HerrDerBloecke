#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace SlimDX::Direct3D9;

namespace HdB {
    // Forward declarations
    ref class Unit;

    ref class Model
    {
    public:
        /** Construct a new object to manage a specific model */
        Model(String^ name, Mesh^ mesh);

        /** Destructor */
        ~Model();

        /** Spawn a new model instance */
        void AddInstance(Unit^ unit);

        /** Draw all of our model instances */
        void Draw(Device^ device);

        property String^ Name;

    private:
        List<Unit^>^ mInstances;
        Material mMaterial;

        // Probably just for testing and to be replaced later
        Mesh^ mMesh;
    };
}
