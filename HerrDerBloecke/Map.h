#pragma once

#include "IDrawable.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Unit;

    ref class MapOccupation
    {
    public:
        MapOccupation(HdB::Unit^ unit);

        property HdB::Unit^ Unit {
            HdB::Unit^ get() { return mUnit; }
        }
        property Rectangle Area {
            Rectangle get() { return mArea; }
        }

    protected:
        void Update(const Vector3% pos);
        HdB::Unit^ mUnit;
        Rectangle mArea;
    };

    ref class Map : IDrawable
    {
    public:
        /** Construct our renderable map. Just a simple grass ground for now. */
        Map(Device^ device);

        virtual ~Map();

        /** Render the map. */
        virtual void Draw();

        /** Add a unit that will occupy its "fields" (for collision detection). */
        void AddUnit(Unit^ unit);

        /** Removing a unit will free its occupied "fields". */
        void RemoveUnit(Unit^ unit);

        /** Check whether a unit is occupying the given position's field. Returns a
         *  nullptr if nothing is found.
         */
        Unit^ CheckOccupation(const Vector3% posOnGround);

        /** Check whether units are occupying the given positions area. Returns a
         *  list of the units in that area.
         */
        List<Unit^>^ CheckOccupation(const Vector3% minPos, const Vector3% maxPos);

        /** Check whether we can build a unit or if it would collide with other units. */
        bool CanBuild(Unit^ unit);

        /** Convert a position on the ground plane to a map field coordinate. */
        static Point GetFieldCoordinate(const Vector3% posOnGround);

    private:
        Device^ mDevice;
        Mesh^ mGroundMesh;
        Texture^ mTexture;
        Material mMaterial;
        List<MapOccupation^>^ mOccupations;
    };
}
