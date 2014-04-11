#pragma once

#include "IDrawable.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Unit;

    ref class Map : IDrawable
    {
    public:
        /** Construct our renderable map. Just a simple grass ground for now. */
        Map(Device^ device);

        virtual ~Map();

        /** Render the map. */
        virtual void Draw();

        /** Convert a position on the ground plane to a map field coordinate. */
        static Point GetFieldCoordinate(const Vector3% posOnGround);

    private:
        Device^ mDevice;
        Mesh^ mGroundMesh;
        Texture^ mTexture;
        Material mMaterial;
    };

    ref class MapOccupation
    {
    public:
        MapOccupation(Unit^ unit);
        bool Contains(Point pos);

        property Point MinField {
            Point get() { return mMinField; }
        }
        property Point MaxField {
            Point get() { return mMaxField; }
        }

    protected:
        void Update(const Vector3% pos);
        Unit^ mUnit;
        Point mMinField;
        Point mMaxField;
    };
}
