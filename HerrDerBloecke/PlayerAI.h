#pragma once
#include "Player.h"

using namespace SlimDX;

namespace HdB {
    ref class PlayerAI : public Player
    {
    public:
        /** Construct a new computer player that will behave in a pre-defined
         *  manner. Initialize with Renderer so the AI can be completely self-
         *  contained.
         */
        PlayerAI(Renderer^ renderer, const Vector3% posHQ);

    private:
        void CheckSchedule(Object^ source, EventArgs^ e);
        void OnNewUnit(Unit^ unit);

        Renderer^ mRenderer;
        Vector3 mPositionHQ;
        Timer^ mTimer;
        UInt64 mSeconds;
    };
}
