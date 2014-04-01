#pragma once

using namespace System;
using namespace SlimDX;

namespace HdB {
    ref class Unit abstract
    {
    public:
        Unit(String^ model, const Vector3% pos);
        Matrix GetTransform();
        void Damage(int dmg);
        property String^ ModelName;
        property Vector3 Position;
        property Vector3 LookAt;
    protected:
        int mHP;
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
