#pragma once

using namespace System;
using namespace SlimDX;

namespace HdB {
    ref class Unit abstract
    {
    public:
        Unit(String^ model, const Vector3% pos);
        property String^ ModelName;
        property Vector3 Position;
    private:
        int mHP;
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
