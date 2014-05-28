#include "Score.h"
#include "Player.h"
#include "Unit.h"

HdB::Score::Score(Player^ player) : mPlayer(player)
{
    mScoreLog = gcnew List<Int32>();
    mLoggingTimer = gcnew Timer();
    mLoggingTimer->Interval = 30000; // Log every 30 seconds
    mLoggingTimer->Tick += gcnew EventHandler(this, &Score::MakeLogEntry);
}

void HdB::Score::MakeLogEntry(Object^ sender, EventArgs^ args)
{
    Int32 score = 0;
    for each (Unit^ u in mPlayer->Units)
        score += u->Points();
    mScoreLog->Add(score);
}
