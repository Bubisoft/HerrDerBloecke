#include "Score.h"
#include "Player.h"
#include "Unit.h"

HdB::Score::Score(Player^ player) : mPlayer(player)
{
    mScoreLog = gcnew List<UInt32>();
    mLoggingTimer = gcnew Timer();
    mLoggingTimer->Interval = 30000; // Log every 30 seconds
    mLoggingTimer->Tick += gcnew EventHandler(this, &Score::MakeLogEntry);
    mLoggingTimer->Enabled = true;
}

void HdB::Score::MakeLogEntry(Object^ sender, EventArgs^ args)
{
    UInt32 score = 0;
    for each (Unit^ u in mPlayer->Units)
        score += u->Points();
    mScoreLog->Add(score);
}
