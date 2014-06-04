#pragma once
#include "ISaveable.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

namespace HdB {
    ref class Player;

    ref class Score : ISaveable {
    public:
        Score(Player^ player);
        property List<UInt32>^ Log {
            List<UInt32>^ get() { return mScoreLog; }
        }
        property Int32 ExtraPoints;

        virtual void Load(BinaryReader^ br, Renderer^ renderer);
        virtual void Save(BinaryWriter^ bw);
    private:
        void MakeLogEntry(Object^ sender, EventArgs^ args);

        Player^ mPlayer;
        Timer^ mLoggingTimer;
        List<UInt32>^ mScoreLog;
    };
}
