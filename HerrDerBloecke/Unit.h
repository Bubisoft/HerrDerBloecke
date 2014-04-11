#pragma once

using namespace System;
using namespace SlimDX;

namespace HdB {
    delegate void PositionEvent(const Vector3% pos);

    ref class Unit abstract
    {
    public:
        Unit(String^ model, const Vector3% pos);
        Matrix GetTransform();
        void Damage(int dmg);

        property String^ ModelName;
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
        int mHP;
        Vector3 mPosition;
    };

    ref class Building abstract : Unit
    {
    public:
        Building(String^ model, const Vector3% pos);
    };

    ref class Soldier abstract : Unit
    {
    public:
        Soldier(String^ model, const Vector3% pos);
        static const int Attack = 0;
    };

    /********
     * TEMP *
     ********/
    ref class TestUnit : Unit
    {
    public:
        TestUnit(String^ model, const Vector3% pos);
    };
}
