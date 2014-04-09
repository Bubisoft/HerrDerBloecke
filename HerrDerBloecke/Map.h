#pragma once

#include "IDrawable.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Map : IDrawable
    {
    public:
        /** Construct our renderable map. Just a simple grass ground for now. */
        Map(Device^ device);

        virtual ~Map();

        /** Render the map. */
        virtual void Draw();

    private:
        Device^ mDevice;
        Mesh^ mGroundMesh;
        Texture^ mTexture;
        Material mMaterial;
    };
}
