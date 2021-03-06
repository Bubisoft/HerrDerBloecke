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

        ~Player();

        /** let the Player get new resources */
        void ProcessResources();

        /** Start a BuildTask for a new unit */
        void BuildUnit(Unit^ unit, UInt16 seconds, Unit^ placeholder);

        event UnitEvent^ UnitBuilt;
        event UnitEvent^ UnitDestroyed;

        /** Manages all of the resources of each player */
        property Resources^ Res;

        /** Accessor for this player's units */
        property List<Unit^>^ Units {
            List<Unit^>^ get() { return mUnits; }
        }
        property Unit^ Headquarters;

        void AddGoldUnit(UInt16 value);
        void AddBlockterieUnit(UInt16 value);
        void AddFoodUnit(UInt16 value);
        
        bool CheckUnitSpace();

        UInt16 CountBlockstatt();

        /** Saves all Data of the Player*/
        virtual void Save(BinaryWriter^ bw);

        virtual void Load(BinaryReader^ br, Renderer^ renderer);

        /** Returns true if the passed Unit ist one of the players */
        bool OwnUnit(Unit^ u);

    protected:
        // Workers on the resources
        UInt16 mGoldUnits;
        UInt16 mBlockterieUnits;
        UInt16 mFoodUnits;

        List<Unit^>^ mUnits;
        List<BuildTask^>^ mBuildTasks;
        Timer^ mBuildTimer;
        void BuildTimerCallback(Object^ source, EventArgs^ e);
        bool IsBlue;
    };
}

