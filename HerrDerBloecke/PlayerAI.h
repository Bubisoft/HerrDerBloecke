#pragma once
#include "Player.h"
#include "AIEvent.h"
using namespace SlimDX;

namespace HdB {

    ref class PlayerAI : public Player
    {
    public:
        /** Construct a new computer player that will behave in a pre-defined
         *  manner. Initialize with Renderer so the AI can be completely self-
         *  contained.
         */
        PlayerAI(Renderer^ renderer, List<Unit^>^ enemyUnits);
        
        /** Called if the AI is to start anew at the given position and not from a loaded game. */
        void NewGame(const Vector3% pos);

        virtual void Save(BinaryWriter^ bw) override;
        virtual void Load(BinaryReader^ br,Renderer^ renderer)override;

        void OnUnitDestroyed(Unit^ unit);
    private:
        void CheckSchedule(Object^ source, EventArgs^ e);

        /** Returns true if the AI has at least one soldier
        */
        int CheckSoldiers();

        /** Checks for missing buildings and tries to rebuild them 
        */
        void CheckMissingBuilding();

        /** Returns a Soldier wich is not defending */
        Soldier^ GetDefender(Soldier^ attacker);

        /** Checks if we are being attacked and adds Events to defend
        */
        void IsAttacked();

        /** Returns a random subclass object of the Soldier class
        */
        Unit^ GetRandomSoldier();

        /** Returns a Building based on a simple logic
        */
        void CheckBuilding();

        /** Moves all Soldiers to the passed position
        */
        void MoveUnits(const Vector3% pos);

        void OnNewUnit(Unit^ unit);

        void PositionUnit(Unit^ unit, Unit^ placeholder);

        Renderer^ mRenderer;
        Random^ mRandom;
        List<Unit^>^ mEnemyUnits;
        List<Soldier^>^ mSoldiers;
        List<Soldier^>^ mDefendingSoldier;
        List<AIEvent^>^ mEvents;
        Vector3 mPositionHQ;
        Timer^ mTimer;
        UInt64 mSeconds;
        UInt16 mLevel;
        UInt16 mCountSoldier;
        AIEvent^ mToDo;

        //decision parameter
        bool CanBuildSoldier;
        bool IsMissingBuilding;
        bool IsBeingAttacked;
        bool IsAttacking;
        bool IsBuilding;
        bool IsWaitingForBuilding;


        bool IsBuildingFarm;
        bool IsBuildingWerk;
        bool IsBuildingStatt;
        bool IsBuildingHaus;
        bool IsBuildingSoldier;
    };
}
