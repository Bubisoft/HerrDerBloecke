#include "Player.h"
#include "Resources.h"
#include "Unit.h"

ref struct HdB::BuildTask
{
    UInt16 seconds;
    Unit^ unit;
};

HdB::Player::Player()
    : mBlockterieUnits(0), mFoodUnits(0), mGoldUnits(0)
{
    Res = gcnew Resources();
    mUnits = gcnew List<Unit^>();
    mBuildTasks = gcnew List<BuildTask^>();
    mBuildTimer = gcnew Timer();
    mBuildTimer->Interval = 1000;
    mBuildTimer->Tick += gcnew EventHandler(this, &Player::BuildTimerCallback);
    mBuildTimer->Enabled = true;

    /* *********
       * TEMP  *
       ********* */
    mBlockterieUnits=1;
    mGoldUnits=1;
    mFoodUnits=1;

    //*********  TEMP END  ***
}
void HdB::Player::ProcessResources()
{
    Res->AddResources(mGoldUnits, mBlockterieUnits, mFoodUnits);
}

void HdB::Player::BuildUnit(Unit^ unit, UInt16 seconds)
{
    BuildTask^ task = gcnew BuildTask();
    task->unit = unit;
    task->seconds = seconds;
    mBuildTasks->Add(task);
}

void HdB::Player::BuildTimerCallback(Object^ source, EventArgs^ e)
{
    // Runs every second and decrements remaining time
    for (size_t i = 0; i < mBuildTasks->Count; i++) {
        if (--mBuildTasks[i]->seconds == 0) {
            mUnits->Add(mBuildTasks[i]->unit);
            UnitBuilt(mBuildTasks[i]->unit);
            mBuildTasks->Remove(mBuildTasks[i]);
        }
    }
}
