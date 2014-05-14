#pragma once
#include "Resources.h"
#include "ISaveable.h"
using namespace System;
using namespace SlimDX;
namespace HdB {
    ref class Model;
    ref class Renderer;
    delegate void PositionEvent(const Vector3% pos);

    ref class Unit abstract
    {
    public:
        // General
        Unit(HdB::Model^ model, const Vector3% pos);
        void Spawn();
        void Despawn();
        Matrix GetTransform();
        void Damage(int dmg);
        float PercentHP() { return mHP / MaxHP(); }
        
        // Properties
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
        property Vector3 MoveTo;
        property bool Spawned {
            bool get() { return mSpawned; }
        }

        // Events
        event PositionEvent^ PositionChanged;

        //ISaveable
        virtual void Save(BinaryWriter^ bw);
        static Unit^ Load(BinaryReader^ br, Renderer^ renderer);

        // Abstract Unit Attributes
        virtual const int MaxHP() = 0;
        virtual const UInt16 BuildTime() = 0;
        virtual const Costs GetCosts() = 0;

    protected:
        HdB::Model^ mModel;
        int mHP;
        Vector3 mPosition;
        bool mSpawned;
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

        // Abstract Unit Attributes
        virtual const int Attack() = 0;
        virtual const int Defense() = 0;
        virtual const int Speed() = 0;
        virtual const int Range() = 0;
    };

    ref class ProductionBuilding abstract : Building
    {
    public:
        ProductionBuilding(HdB::Model^ model, const Vector3% pos);

        // Abstract Unit Attributes
        virtual const ProductionType GetProductionType() = 0;
    };

    /********
     * TEMP *
     ********/
    ref class TestUnit : Soldier
    {
    public:
        TestUnit(HdB::Model^ model, const Vector3% pos);

        virtual const int MaxHP() override { return 100; }
        virtual const UInt16 BuildTime() override { return 5; }
        virtual const Costs GetCosts() override { return Costs(3, 6, 8); }

        virtual const int Attack() override { return 10; }
        virtual const int Defense() override { return 10; }
        virtual const int Speed() override { return 10; }
        virtual const int Range() override { return 1; }
    };

    /************
     * SOLDIERS *
     ************/
    ref class ZuseZ3 : Soldier
    {
    public:
        ZuseZ3(HdB::Model^ model, const Vector3% pos);

        virtual const int MaxHP() override { return 50; }
        virtual const UInt16 BuildTime() override { return 5; }
        virtual const Costs GetCosts() override { return Costs(5, 10, 10); }

        virtual const int Attack() override { return 10; }
        virtual const int Defense() override { return 10; }
        virtual const int Speed() override { return 10; }
        virtual const int Range() override { return 10; }
    };

    ref class Wirth : Soldier
    {
    public:
        Wirth(HdB::Model^ model, const Vector3% pos);

        virtual const int MaxHP() override { return 30; }
        virtual const UInt16 BuildTime() override { return 7; }
        virtual const Costs GetCosts() override { return Costs(15, 10, 5); }

        virtual const int Attack() override { return 10; }
        virtual const int Defense() override { return 20; }
        virtual const int Speed() override { return 100; }
        virtual const int Range() override { return 5; }
    };

    /*************
     * BUILDINGS *
     *************/
    ref class Hauptgebaeude: Building
    {
    public:
        Hauptgebaeude(HdB::Model^ model, const Vector3% pos);
        virtual const int MaxHP() override { return 500; }
        virtual const UInt16 BuildTime() override { return 0; }
        virtual const Costs GetCosts() override { return Costs(0, 0, 0); }
    };

    ref class Blockhuette : ProductionBuilding
    {
    public:
        Blockhuette(HdB::Model^ model, const Vector3% pos);

        virtual const int MaxHP() override { return 100; }
        virtual const UInt16 BuildTime() override { return 10; }
        virtual const Costs GetCosts() override { return Costs(3, 6, 8); }
        virtual const ProductionType GetProductionType() override { return ProductionType::eGold; }
        virtual void Save(BinaryWriter^ br) override;

    public:
        bool enabled;
    };

    ref class Blockstatt : Building
    {
    public:
        Blockstatt(HdB::Model^ model, const Vector3% pos);

        virtual const int MaxHP() override { return 100; }
        virtual const UInt16 BuildTime() override { return 8; }
        virtual const Costs GetCosts() override { return Costs(3, 3, 3); }
    };

    ref class Blockwerk : ProductionBuilding
    {
    public:
        Blockwerk(HdB::Model^ model, const Vector3% pos);

        virtual const int MaxHP() override { return 100; }
        virtual const UInt16 BuildTime() override { return 8; }
        virtual const Costs GetCosts() override { return Costs(1,2,3); }
        virtual const ProductionType GetProductionType() override { return ProductionType::eBlockterie; }
    };

    ref class Blockfarm : ProductionBuilding
    {
    public:
        Blockfarm(HdB::Model^ model, const Vector3% pos);

        virtual const int MaxHP() override { return 100; }
        virtual const UInt16 BuildTime() override { return 11; }
        virtual const Costs GetCosts() override { return Costs(4,5,6); }
        virtual const ProductionType GetProductionType() override { return ProductionType::eFood; }
    };
}
