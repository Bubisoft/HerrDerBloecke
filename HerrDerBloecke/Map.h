#pragma once

#include "IDrawable.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class MapField : IDrawable
    {
    public:
        /** Simple MapField that consists of only a ground plane with
         *  a texture.
         */
        MapField(Device^ device, String^ texture);

        virtual ~MapField();

        virtual void Draw();

        /** Add a new map field at tha abstract position x, y.
         *  X and Y are not world coordinates but grid coordinates.
         */
        virtual void AddField(int x, int y);

    protected:
        Device^ mDevice;
        Mesh^ mGroundMesh;
        Texture^ mTexture;
        List<Vector3>^ mInstances;
        Material mMaterial;
    };

    ref class Map : IDrawable
    {
    public:
        /** Construct our renderable map. */
        Map(Device^ device);

        virtual ~Map();

        /** Render the map. */
        virtual void Draw();

    private:
        Device^ mDevice;
        MapField^ mGrass;
    };
}
