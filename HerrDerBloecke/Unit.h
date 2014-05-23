#pragma once
#include "Resources.h"
#include "ISaveable.h"

using namespace System;
using namespace SlimDX;

namespace HdB {
    ref class Model;
    ref class Renderer;
    ref class Unit;
    delegate void PositionEvent(const Vector3% pos);
    delegate void UnitDestroyedEvent(Unit^ u);

    ref class Unit abstract
    {
    public:
        // General
        Unit(HdB::Model^ model, const Vector3% pos);
        void Spawn();
        void Despawn();
        Matrix GetTransform();
        void Damage(int dmg);
        float PercentHP() { return (float)mHP / MaxHP(); }
        
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
        event UnitDestroyedEvent^ UnitDestroyed;

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

    /*********************
     * UNIT BASE CLASSES *
     *********************/
    ref class Building abstract : Unit
    {
    public:
        Building(HdB::Model^ model, const Vector3% pos);
    };

    ref class Soldier abstract : Unit
    {
    public:
        Soldier(HdB::Model^ model, const Vector3% pos);
        void StartAttack(Unit^ target);
        void AttackCallback(Object^ sender, EventArgs^ e);
        property Unit^ mAttackTarget;
    private:
        System::Windows::Forms::Timer^ mAttackTimer;
    public:
        // Abstract Unit Attributes
        virtual const int Attack() = 0;
        virtual const int AttackSpeed() = 0;
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

    /*******************
     * OVERRIDE MACROS *
     *******************/
    #define UNIT_MAXHP(x) virtual const int MaxHP() override { return x; }
    #define UNIT_BUILDTIME(x) virtual const UInt16 BuildTime() override { return x; }
    #define UNIT_COSTS(gold, blockt, food) virtual const Costs GetCosts() override { return Costs(gold, blockt, food); }
    #define SOLDIER_ATTACK(x) virtual const int Attack() override { return x; }
    #define SOLDIER_ATTACKSPEED(x) virtual const int AttackSpeed() override {return x; }
    #define SOLDIER_DEFENSE(x) virtual const int Defense() override { return x; }
    #define SOLDIER_SPEED(x) virtual const int Speed() override { return x; }
    #define SOLDIER_RANGE(x) virtual const int Range() override { return x; }
    #define PRODUCTION_TYPE(x) virtual const ProductionType GetProductionType() override { return x; }

    /********
     * TEMP *
     ********/
    ref class TestUnit : Soldier
    {
    public:
        TestUnit(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(100);
        UNIT_BUILDTIME(5);
        UNIT_COSTS(3, 6, 8);

        SOLDIER_ATTACK(10);
        SOLDIER_ATTACKSPEED(3);
        SOLDIER_DEFENSE(10);
        SOLDIER_SPEED(10);
        SOLDIER_RANGE(1);
    };

    /************
     * SOLDIERS *
     ************/
    ref class ZuseZ3 : Soldier
    {
    public:
        ZuseZ3(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(50);
        UNIT_BUILDTIME(5);
        UNIT_COSTS(5, 10, 10);

        SOLDIER_ATTACK(10);
        SOLDIER_ATTACKSPEED(6);
        SOLDIER_DEFENSE(10);
        SOLDIER_SPEED(10);
        SOLDIER_RANGE(10);
    };

    ref class Wirth : Soldier
    {
    public:
        Wirth(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(30);
        UNIT_BUILDTIME(7);
        UNIT_COSTS(15, 10, 5);

        SOLDIER_ATTACK(10);
        SOLDIER_ATTACKSPEED(3);
        SOLDIER_DEFENSE(20);
        SOLDIER_SPEED(100);
        SOLDIER_RANGE(5);
    };

    /*************
     * BUILDINGS *
     *************/
    ref class Hauptgebaeude: Building
    {
    public:
        Hauptgebaeude(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(500);
        UNIT_BUILDTIME(0);
        UNIT_COSTS(0, 0, 0);
    };

    ref class Blockstatt : Building
    {
    public:
        Blockstatt(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(100);
        UNIT_BUILDTIME(8);
        UNIT_COSTS(3, 3, 3);
    };

    ref class Blockhuette : ProductionBuilding
    {
    public:
        Blockhuette(HdB::Model^ model, const Vector3% pos);

        virtual void Save(BinaryWriter^ br) override;
        property bool Enabled;

        UNIT_MAXHP(100);
        UNIT_BUILDTIME(10);
        UNIT_COSTS(3, 6, 8);
        PRODUCTION_TYPE(ProductionType::eGold);
    };

    ref class Blockwerk : ProductionBuilding
    {
    public:
        Blockwerk(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(100);
        UNIT_BUILDTIME(8);
        UNIT_COSTS(1, 2, 3);
        PRODUCTION_TYPE(ProductionType::eBlockterie);
    };

    ref class Blockfarm : ProductionBuilding
    {
    public:
        Blockfarm(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(100);
        UNIT_BUILDTIME(11);
        UNIT_COSTS(4, 5, 6);
        PRODUCTION_TYPE(ProductionType::eFood);
    };
}
