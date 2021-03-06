#include "Player.h"
#include "Resources.h"
#include "Unit.h"
#include "Renderer.h"
#include "Map.h"

#define MAX_UNITS 50

ref struct HdB::BuildTask
{
    UInt16 seconds;
    Unit^ unit;
    Unit^ placeholder;
};

HdB::Player::Player()
{
    Res = gcnew Resources(100,300,80); //setting up start resources
    mUnits = gcnew List<Unit^>();
    mBuildTasks = gcnew List<BuildTask^>();
    mBuildTimer = gcnew Timer();
    mBuildTimer->Interval = 1000;
    mBuildTimer->Tick += gcnew EventHandler(this, &Player::BuildTimerCallback);
    mBuildTimer->Enabled = true;

    mBlockterieUnits = 0;
    mGoldUnits = 0;
    mFoodUnits = 0;
    IsBlue = true;
}

HdB::Player::~Player()
{

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

    // Spawn building and let them grow
    if (Building^ b = dynamic_cast<Building^>(unit)) {
        b->BuildProgress = 0.f;
        b->Spawn();
    }

    mBuildTasks->Add(task);
    Res->Pay(unit->GetCosts());
}

void HdB::Player::BuildTimerCallback(Object^ source, EventArgs^ e)
{
    UInt16 soldiersFound = 0;
    UInt16 numBlockstatt = CountBlockstatt();

    // Runs every second and decrements remaining time
    for (int i = 0; i < mBuildTasks->Count; i++) {
        // Only build as many soldiers as you have Blockstätte at the same time, skip others
        if (mBuildTasks[i]->unit->GetType()->IsSubclassOf(Soldier::typeid)) {
            if (soldiersFound >= numBlockstatt)
                continue;
            soldiersFound++;
        }

        // Calculate BuildProgress as percent of BuildTime
        Building^ b = dynamic_cast<Building^>(mBuildTasks[i]->unit);
        if (b && b->BuildTime()) {
            float prog = 1.f - (float)mBuildTasks[i]->seconds / b->BuildTime();
            b->BuildProgress = prog;
        }

        if (mBuildTasks[i]->seconds-- == 0) {
            if (mBuildTasks[i]->placeholder)
                mBuildTasks[i]->placeholder->Despawn();
            if (b)
                b->BuildProgress = 1.f;
            mBuildTasks[i]->unit->Spawn();
            mUnits->Add(mBuildTasks[i]->unit);
            UnitBuilt(mBuildTasks[i]->unit);
            mBuildTasks->Remove(mBuildTasks[i]);
        }
    }
}

bool HdB::Player::OwnUnit(Unit^ u)
{
    for each (BuildTask^ t in mBuildTasks)
        if (t->unit == u)
            return true;
    return mUnits->Contains(u);
}

void HdB::Player::AddGoldUnit(UInt16 value)
{
    mGoldUnits+=value;
}

void HdB::Player::AddBlockterieUnit(UInt16 value)
{
    mBlockterieUnits += value;
}

void HdB::Player::AddFoodUnit(UInt16 value)
{
    mFoodUnits += value;
}

void HdB::Player::Save(BinaryWriter^ bw)
{
    Res->Save(bw);
    bw->Write(mGoldUnits);
    bw->Write(mBlockterieUnits);
    bw->Write(mFoodUnits);
    
    bw->Write(mUnits->Count);
    for each(Unit^ u in mUnits)
        u->Save(bw);

}

void HdB::Player::Load(BinaryReader^ br,Renderer^ renderer)
{
    Res->Load(br,renderer);
    mGoldUnits=br->ReadUInt16();
    mBlockterieUnits=br->ReadUInt16();
    mFoodUnits=br->ReadUInt16();

    int UnitCount=br->ReadInt32();

    for(int i = 0; i < UnitCount; ++i)
    {
        Unit^ LoadedUnit = Unit::Load(br, renderer, IsBlue);
        LoadedUnit->Spawn();
        renderer->Map->AddUnit(LoadedUnit);
        mUnits->Add(LoadedUnit);
        if (LoadedUnit->GetType() == Hauptgebaeude::typeid)
            Headquarters = LoadedUnit;
    }
}

bool HdB::Player::CheckUnitSpace()
{
    UInt16 i = 0;
    for each (Unit^ u in mUnits)
        if (u->GetType()->IsSubclassOf(Soldier::typeid))
            i++;

    for each (BuildTask^ b in mBuildTasks)
        if (b->unit->GetType()->IsSubclassOf(Soldier::typeid))
            i++;

    return i < MAX_UNITS;
}

UInt16 HdB::Player::CountBlockstatt()
{
    UInt16 count = 0;
    for each (Unit^ u in mUnits)
        if (u->GetType() == Blockstatt::typeid)
            count++;
    return count;
}
