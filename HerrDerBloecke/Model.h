#pragma once

#include "IDrawable.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    // Forward declarations
    ref class Renderer;
    ref class Unit;
    ref struct Submesh;

    ref class Model : IDrawable
    {
    public:
        /** Construct a new object to manage a specific model.
         *  The model will be loaded from the file models/<name>.HBM.
         *  The Modelname in the file will be ignored as we want to
         *  handle model creation and usage consistently.
         */
        Model(String^ name, Renderer^ device);

        /** Destructor */
        virtual ~Model();

        /** Spawn a new model instance. */
        void AddInstance(Unit^ unit);

        /** Despawn an existing model instance. */
        void RemoveInstance(Unit^ unit);

        /** Draw all of our model instances. */
        virtual void Draw(long long timeSinceLastFrame);

        /** Set alpha blending (opacity) of the model. */
        void SetAlpha(float alpha);

        /** Overwrite diffuse color, used to display a player's units
         *  in a specific color. */
        void SetTeamColor(Color4 color);

        property String^ Name;
        property BoundingBox Bounds {
            BoundingBox get() { return mBounds; }
        }

        /** Is this model used for placeholder units? */
        property bool IsPlaceholder;

        virtual String^ ToString() override { return Name; }

    private:
        void LoadFromHBMFile(String^ filename);

        Renderer^ mRenderer;
        List<Submesh^>^ mMeshes;
        List<Unit^>^ mInstances;
        BoundingBox mBounds;
    };
}
