#pragma once

#include "IDrawable.h"
#include "ISaveable.h"
using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Unit;
    ref class Renderer;

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
        Map(Renderer^ renderer);

        virtual ~Map();

        /** Render the map. */
        virtual void Draw(long long timeSinceLastFrame);

        /** Add a unit that will occupy its "fields" (for collision detection). */
        void AddUnit(Unit^ unit);

        /** Removing a unit will free its occupied "fields". */
        void RemoveUnit(Unit^ unit);

        /** Check whether a unit is occupying the given position's field. Returns a
         *  nullptr if nothing is found.
         */
        Unit^ CheckOccupation(const Vector3% posOnGround);

        /** Check whether units are occupying the rectangle of the given two positions. Returns a
        *  list of the units in that area.
        */
        List<Unit^>^ CheckOccupation(const Vector3% a, const Vector3% b);

        /** Check whether units are occupying the area of the given four polygon points.
         *  Returns a list of the units in that area.
         */
        List<Unit^>^ CheckOccupation(const Vector3% a, const Vector3% b, const Vector3% c, const Vector3% d);

        /** Check whether we can build a unit or if it would collide with other units. */
        bool CanBuild(Unit^ unit);

        /** Check whether we can build a unit at the given position or if it would collide with other units. */
        bool CanBuild(Unit^ unit, const Vector3% position);

        /** Recursively calculate the direction a unit has to go from its position to reach the given field.
         *  This caclulates a path without collisions and returns the direction vector to the next waypoint.
         */
        Vector3 NextMoveDirection(Unit^ unit, Point endField);

        /** Check whether a given position is on our map. */
        bool OnMap(const Vector3% pos);

        /** Convert a position on the ground plane to a map field coordinate. */
        static Point GetFieldCoordinate(const Vector3% posOnGround);

        /** Convert a map field coordinate to the vector pointing at its center. */
        static Vector3 GetFieldCenter(Point field);

    private:
        Vector3 NextMoveDirection(Unit^ unit, Point endField, size_t it);
        Renderer^ mRenderer;
        Mesh^ mGroundMesh;
        Texture^ mTexture;
        Material mMaterial;
        List<MapOccupation^>^ mOccupations;
    };
}
