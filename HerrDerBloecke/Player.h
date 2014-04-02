#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

namespace HdB {
    ref struct BuildTask;
    ref class Unit;
    ref class Resources;

    delegate void UnitEvent(Unit^ unit);

    ref class Player
    {
    public:
        Player();

        /** let the Player get new resources */
        void ProcessResources();

        /** Start a BuildTask for a new unit */
        void BuildUnit(Unit^ unit, UInt16 seconds);
        event UnitEvent^ UnitBuilt;

        /** Manages all of the resources of each player */
        property Resources^ Res;

        /** Accessor for this player's units */
        property List<Unit^>^ Units {
            List<Unit^>^ get() { return mUnits; }
        }

    private:
        // Workers on the resources
        UInt16 mGoldUnits;
        UInt16 mBlockterieUnits;
        UInt16 mFoodUnits;

        List<Unit^>^ mUnits;
        List<BuildTask^>^ mBuildTasks;
        Timer^ mBuildTimer;
        void BuildTimerCallback(Object^ source, EventArgs^ e);
    };
}

