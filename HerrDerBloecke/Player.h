#pragma once
#include "ISaveable.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
namespace HdB {
    ref struct BuildTask;
    ref class Unit;
    ref class Resources;

    delegate void UnitEvent(Unit^ unit);
    ref class Player : ISaveable
    {
    public:
        Player();

        /** let the Player get new resources */
        void ProcessResources();

        /** Start a BuildTask for a new unit */
        void BuildUnit(Unit^ unit, UInt16 seconds, Unit^ placeholder);
        event UnitEvent^ UnitBuilt;

        /** Manages all of the resources of each player */
        property Resources^ Res;

        /** Accessor for this player's units */
        property List<Unit^>^ Units {
            List<Unit^>^ get() { return mUnits; }
        }

        property List<UInt32>^ PointsOverTime {
            List<UInt32>^ get() { return mPointsOverTime; }
        }

        void AddGoldUnit(UInt16 value);
        void AddBlockterieUnit(UInt16 value);
        void AddFoodUnit(UInt16 value);

        /** Saves all Data of the Player*/
        virtual void Save(BinaryWriter^ bw);

        virtual void Load(BinaryReader^ br,Renderer^ renderer);

        /** Returns true if the passed Unit ist one of the players */
        bool OwnUnit(Unit^ u);

    private:
        // Workers on the resources
        UInt16 mGoldUnits;
        UInt16 mBlockterieUnits;
        UInt16 mFoodUnits;
        UInt32 mPoints;

        List<UInt32>^ mPointsOverTime;
        List<Unit^>^ mUnits;
        List<BuildTask^>^ mBuildTasks;
        Timer^ mBuildTimer;
        void BuildTimerCallback(Object^ source, EventArgs^ e);
    };
}

