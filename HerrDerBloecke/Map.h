#pragma once

#include "IDrawable.h"

using namespace System;
using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Map : IDrawable
    {
    public:
        /** Construct our renderable map. */
        Map(Device^ device);

        /** Render the map. */
        virtual void Draw();

    private:
        Device^ mDevice;
        
    };

    ref class MapField : IDrawable
    {
    public:
        /** Simple MapField that consists of only a ground plane with
         *  a texture.
         */
        MapField(Device^ device, String^ texture);

        virtual ~MapField();
        virtual void Draw();

    private:
        Device^ mDevice;
        Mesh^ mGroundMesh;
        Texture^ mTexture;
    };
}
