#include "Score.h"
#include "Player.h"
#include "Unit.h"

HdB::Score::Score(Player^ player) : mPlayer(player)
{
    mScoreLog = gcnew List<UInt32>();
    mLoggingTimer = gcnew Timer();
    mLoggingTimer->Interval = 3000; // Log every 3 seconds
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

void HdB::Score::Load(BinaryReader^ br, Renderer^ rendere)
{
    mLoggingTimer->Stop();
    int x=br->ReadUInt32();
    for(int i=0;i<x;++i)
        mScoreLog->Add(br->ReadUInt32());

    mLoggingTimer->Start();
}

void HdB::Score::Save(BinaryWriter^ bw)
{
    mLoggingTimer->Stop();
    bw->Write(mScoreLog->Count);
    for each(UInt32 u in mScoreLog)
        bw->Write(u);
    mLoggingTimer->Start();
}
