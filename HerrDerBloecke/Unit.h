#pragma once

using namespace System;
using namespace SlimDX;

namespace HdB {
    ref class Model;

    delegate void PositionEvent(const Vector3% pos);

    ref class Unit abstract
    {
    public:
        Unit(HdB::Model^ model, const Vector3% pos);
        Matrix GetTransform();
        void Damage(int dmg);

        property HdB::Model^ Model {
            HdB::Model^ get() { return mModel; }
        }
        property Vector3 Position {
            Vector3 get() { return mPosition; }
            void set(Vector3 pos) {
                mPosition = pos;
                PositionChanged(pos);
            }
        }
        property Vector3 LookAt;

        event PositionEvent^ PositionChanged;

    protected:
        HdB::Model^ mModel;
        int mHP;
        Vector3 mPosition;
    };

    ref class Building abstract : Unit
    {
    public:
        Building(HdB::Model^ model, const Vector3% pos);
    };

    ref class Soldier abstract : Unit
    {
    public:
        Soldier(HdB::Model^ model, const Vector3% pos);
        static const int Attack = 0;
    };

    /********
     * TEMP *
     ********/
    ref class TestUnit : Unit
    {
    public:
        TestUnit(HdB::Model^ model, const Vector3% pos);
    };
}
