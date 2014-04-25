#pragma once
#include "Resources.h"
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

        virtual UInt16 BuildTime() { return 1; }

        virtual Cost GetCost(){Cost ret={-1,-1,-1}; return ret;}

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

        virtual UInt16 BuildTime() override { return 5; }
        virtual Cost GetCost() override {Cost ret={3,6,8}; return ret;}
    };

    ref class Soldier abstract : Unit
    {
    public:
        Soldier(HdB::Model^ model, const Vector3% pos);
        static const int Attack = 0;
    };

    ref class ProductionBuilding : Building
    {
    public:
        ProductionBuilding(HdB::Model^ model, const Vector3% pos);

        virtual ProductionType GetProductionType()=0;
    };

    /********
     * TEMP *
     ********/
    ref class TestUnit : Unit
    {
    public:
        TestUnit(HdB::Model^ model, const Vector3% pos);

        virtual UInt16 BuildTime() override { return 5; }
        virtual Cost GetCost() override {Cost ret={3,6,8}; return ret;}
    };

    ref class Blockhuette : ProductionBuilding
    {
    public:
        Blockhuette(HdB::Model^ model, const Vector3% pos);

        virtual UInt16 BuildTime() override { return 10; }
        virtual Cost GetCost() override {Cost ret={3,6,8}; return ret;}
        virtual ProductionType GetProductionType() override {return ProductionType::eGold;}
    };


}
