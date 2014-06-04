#include "Score.h"
#include "Player.h"
#include "Unit.h"

HdB::Score::Score(Player^ player) : mPlayer(player)
{
    mScoreLog = gcnew List<UInt32>();
    mLoggingTimer = gcnew Timer();
    mLoggingTimer->Interval = 3000; // Log every 30 seconds
    mLoggingTimer->Tick += gcnew EventHandler(this, &Score::MakeLogEntry);
    mLoggingTimer->Enabled = true;
    ExtraPoints=0;
}

void HdB::Score::MakeLogEntry(Object^ sender, EventArgs^ args)
{
    UInt32 score = 0;
    for each (Unit^ u in mPlayer->Units)
        score += u->Points();
    score+=ExtraPoints;
    mScoreLog->Add(score);
}
