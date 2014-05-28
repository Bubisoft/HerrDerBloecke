#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

namespace HdB {
    ref class Player;

    ref class Score {
    public:
        Score(Player^ player);
        property List<UInt32>^ Log {
            List<UInt32>^ get() { return mScoreLog; }
        }

    private:
        void MakeLogEntry(Object^ sender, EventArgs^ args);

        Player^ mPlayer;
        Timer^ mLoggingTimer;
        List<UInt32>^ mScoreLog;
    };
}
