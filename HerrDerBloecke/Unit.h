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
        virtual void Damage(int dmg);
        float PercentHP() { return (float)mHP / MaxHP(); }
        void ResetLookAt() { LookAt = -Vector3::UnitY + Position; }

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
        static Unit^ Load(BinaryReader^ br, Renderer^ renderer,bool isblue);

        // Abstract Unit Attributes
        virtual const int MaxHP() = 0;
        virtual const UInt16 BuildTime() = 0;
        virtual const Costs GetCosts() = 0;
        virtual const Int32 Points() = 0;
        virtual const String^ Description() = 0;

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
        property float BuildProgress {
            float get() { return mProgress; }
            void set(float f) { mProgress = f; }
        }
    private:
        float mProgress;
    };

    ref class Soldier abstract : Unit
    {
    public:
        Soldier(HdB::Model^ model, const Vector3% pos);
        void StartAttack(Unit^ target);
        void StopAttack();
        bool IsInRange();
        property Unit^ AttackTarget;

        virtual void Damage(int damage)override;

        // Abstract Unit Attributes
        virtual const int Attack() = 0;
        virtual const int AttackSpeed() = 0;
        virtual const int Defense() = 0;
        virtual const int Speed() = 0;
        virtual const int Range() = 0;

    private:
        void AttackCallback(Object^ sender, EventArgs^ e);
        System::Windows::Forms::Timer^ mAttackTimer;
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
    #define UNIT_POINTS(x) virtual const Int32 Points() override { return x; }
    #define UNIT_DESCRIPTION(x) virtual const String^ Description() override { return x; }
    #define SOLDIER_ATTACK(x) virtual const int Attack() override { return x; }
    #define SOLDIER_ATTACKSPEED(x) virtual const int AttackSpeed() override {return x; }
    #define SOLDIER_DEFENSE(x) virtual const int Defense() override { return x; }
    #define SOLDIER_SPEED(x) virtual const int Speed() override { return x; }
    #define SOLDIER_RANGE(x) virtual const int Range() override { return x; }
    #define PRODUCTION_TYPE(x) virtual const ProductionType GetProductionType() override { return x; }

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
        UNIT_POINTS(5);
        UNIT_DESCRIPTION("Der Allrounder: Billig mit moderater St�rke.");

        SOLDIER_ATTACK(20);
        SOLDIER_ATTACKSPEED(4);
        SOLDIER_DEFENSE(50);
        SOLDIER_SPEED(10);
        SOLDIER_RANGE(50);
    };

    ref class Wirth : Soldier
    {
    public:
        Wirth(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(30);
        UNIT_BUILDTIME(7);
        UNIT_COSTS(15, 10, 5);
        UNIT_POINTS(5);
        UNIT_DESCRIPTION("Schnelle und billige, aber nicht so starke Einheit.");

        SOLDIER_ATTACK(5);
        SOLDIER_ATTACKSPEED(1);
        SOLDIER_DEFENSE(10);
        SOLDIER_SPEED(100);
        SOLDIER_RANGE(25);
    };

    ref class Gates : Soldier
    {
    public:
        Gates(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(40);
        UNIT_BUILDTIME(8);
        UNIT_COSTS(60, 20, 80);
        UNIT_POINTS(10);
        UNIT_DESCRIPTION("Schnellere und st�rkere, aber teurere Einheit.");

        SOLDIER_ATTACK(15);
        SOLDIER_ATTACKSPEED(2);
        SOLDIER_DEFENSE(15);
        SOLDIER_SPEED(20);
        SOLDIER_RANGE(15);
    };

    ref class Turing : Soldier
    {
    public:
        Turing(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(400);
        UNIT_BUILDTIME(120);
        UNIT_COSTS(5000, 5000, 5000);
        UNIT_POINTS(100);
        UNIT_DESCRIPTION("Extrem starke Einheit, aber langsam und sehr teuer.");

        SOLDIER_ATTACK(400);
        SOLDIER_ATTACKSPEED(20);
        SOLDIER_DEFENSE(10);
        SOLDIER_SPEED(17);
        SOLDIER_RANGE(50);
    };

    /*************
     * BUILDINGS *
     *************/
    ref class Hauptgebaeude: Building
    {
    public:
        Hauptgebaeude(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(5000);
        UNIT_BUILDTIME(0);
        UNIT_COSTS(0, 0, 0);
        UNIT_POINTS(0);
        UNIT_DESCRIPTION("Das Hauptgeb�ude. Wird es zerst�rt, verliert der Spieler.");
    };

    ref class Blockstatt : Building
    {
    public:
        Blockstatt(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(1000);
        UNIT_BUILDTIME(8);
        UNIT_COSTS(25, 60, 15);
        UNIT_POINTS(5);
        UNIT_DESCRIPTION("Geb�ude zum Ausbilden von Soldaten.");
    };

    ref class Blockhuette : ProductionBuilding
    {
    public:
        Blockhuette(HdB::Model^ model, const Vector3% pos);

        virtual void Save(BinaryWriter^ br) override;
        property bool Enabled;

        UNIT_MAXHP(500);
        UNIT_BUILDTIME(10);
        UNIT_COSTS(10, 40, 40);
        UNIT_POINTS(5);
        UNIT_DESCRIPTION("Produziert Goldbarren, verbraucht Kastenbrot.");
        PRODUCTION_TYPE(ProductionType::eGold);
    };

    ref class Blockwerk : ProductionBuilding
    {
    public:
        Blockwerk(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(700);
        UNIT_BUILDTIME(8);
        UNIT_COSTS(30, 50, 20);
        UNIT_POINTS(5);
        UNIT_DESCRIPTION("Produziert Blockterie.");
        PRODUCTION_TYPE(ProductionType::eBlockterie);
    };

    ref class Blockfarm : ProductionBuilding
    {
    public:
        Blockfarm(HdB::Model^ model, const Vector3% pos);

        UNIT_MAXHP(400);
        UNIT_BUILDTIME(11);
        UNIT_COSTS(20, 30, 10);
        UNIT_POINTS(5);
        UNIT_DESCRIPTION("Produziert Kastenbrot.");
        PRODUCTION_TYPE(ProductionType::eFood);
    };
}
