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
        PlayerAI(Renderer^ renderer, const Vector3% posHQ, List<Unit^>^ enemyUnits);
        virtual void Save(BinaryWriter^ bw) override;
        virtual void Load(BinaryReader^ br,Renderer^ renderer)override;

        void OnUnitDestroyed(Unit^ unit);
    private:
        void CheckSchedule(Object^ source, EventArgs^ e);

        void CheckEvents(Object^ sourde, EventArgs^ e);

        /** Returns true if the AI has at least one soldier
        */
        int CheckSoldiers();

        /** Checks for missing buildings and tries to rebuild them 
        */
        void CheckMissingBuilding();

        Unit^ IsAttacked();

        /** Returns a random subclass object of the Soldier class
        */
        Unit^ GetRandomSoldier();

        /** Starts a attack with all soldiers on the given Unit without interrupting except isDefense is true
        */
        void Attack(Unit^ u, bool isDefense);

        /** Moves all Soldiers to the passed position
        */
        void MoveUnits(const Vector3% pos);

        void OnNewUnit(Unit^ unit);

        void PositionUnit(Unit^ unit, Unit^ placeholder);

        Renderer^ mRenderer;
        Random^ mRandom;
        List<Unit^>^ mEnemyUnits;
        List<AIEvent^>^ mEvents;
        Vector3 mPositionHQ;
        Timer^ mTimer;
        Timer^ mEventTimer;
        UInt64 mSeconds;

        //decision parameter
        bool CanBuildSoldier;
        bool IsMissingBuilding;
        bool IsWaitingForSoldier;
        bool IsBeingAttacked;

        bool IsBuildingFarm;
        bool IsBuildingWerk;
        bool IsBuildingStatt;
        bool IsBuildingHaus;
        bool IsBuildingSoldier;
    };
}
