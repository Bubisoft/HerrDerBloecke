#include "Player.h"
#include "Resources.h"
#include "Unit.h"

ref struct HdB::BuildTask
{
    UInt16 seconds;
    Unit^ unit;
    Unit^ placeholder;
};

HdB::Player::Player()
    : mBlockterieUnits(0), mFoodUnits(0), mGoldUnits(0)
{
    Res = gcnew Resources(100,300,80); //setting up start resources
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

void HdB::Player::BuildUnit(Unit^ unit, UInt16 seconds, Unit^ placeholder)
{
    BuildTask^ task = gcnew BuildTask();
    task->unit = unit;
    task->seconds = seconds;
    task->placeholder = placeholder;
    if (placeholder)
        placeholder->Spawn();
    mBuildTasks->Add(task);
}

void HdB::Player::BuildTimerCallback(Object^ source, EventArgs^ e)
{
    // Runs every second and decrements remaining time
    for (int i = 0; i < mBuildTasks->Count; i++) {
        if (--mBuildTasks[i]->seconds == 0) {
            if (mBuildTasks[i]->placeholder)
                mBuildTasks[i]->placeholder->Despawn();
            mUnits->Add(mBuildTasks[i]->unit);
            mBuildTasks[i]->unit->Spawn();
            UnitBuilt(mBuildTasks[i]->unit);
            mBuildTasks->Remove(mBuildTasks[i]);
        }
    }
}

void HdB::Player::AddGoldUnit()
{ mGoldUnits++; }

void HdB::Player::AddBlockterieUnit()
{ mBlockterieUnits++; }

void HdB::Player::AddFoodUnit()
{ mFoodUnits++; }

