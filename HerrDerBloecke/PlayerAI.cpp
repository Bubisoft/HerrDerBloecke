#include "PlayerAI.h"
#include "Renderer.h"
#include "Map.h"
#include "Unit.h"
using namespace HdB;
// Schedule macros
#define START_BUILDING(atTime, UnitType, Model, pos) \
    if (mSeconds == atTime) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        Unit^ ph = gcnew UnitType(mRenderer->GetAlphaModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        BuildUnit(u, u->BuildTime(), ph); \
        mRenderer->Map->AddUnit(u); \
    }
#define START_SOLDIER(atTime, UnitType, Model, pos) \
    if (mSeconds == atTime) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        BuildUnit(u, u->BuildTime(), nullptr); \
    }


HdB::PlayerAI::PlayerAI(Renderer^ renderer, const Vector3% posHQ, List<Unit^>^ enemyUnits) : mRenderer(renderer), mPositionHQ(posHQ), mEnemyUnits(enemyUnits)
{
    // Initialize AI schedule timer
    mTimer = gcnew Timer();
    mTimer->Interval = 1000;
    mTimer->Tick += gcnew EventHandler(this, &PlayerAI::CheckSchedule);
    mTimer->Enabled = true;

    // Initialize Randomobject
    mRandom=gcnew Random((int)DateTime::Now.Ticks);

    // Register callback for soldiers
    UnitBuilt += gcnew UnitEvent(this, &PlayerAI::OnNewUnit);

    Unit^ u = gcnew Hauptgebaeude(mRenderer->GetRedModel("Hauptgebaeude"), mPositionHQ);
    BuildUnit(u, 0, nullptr);
    mRenderer->Map->AddUnit(u);
    Headquarters = u;

    IsBlue=false;
}

bool HdB::PlayerAI::CheckSoldiers()
{
    for each(Unit^ u in Units)
    {
        if(u->GetType()->IsSubclassOf(Soldier::typeid))
            return true;
    }
    return false;
}

void HdB::PlayerAI::Attack(Unit^ target)
{
    for each(Unit^ u in Units) //start attack with all Soldiers
    {
        if(Soldier^ s= dynamic_cast<Soldier^>(u))
        {
            s->StartAttack(target);
            s->MoveTo = target->Position;
        }
    }
}

Unit^ HdB::PlayerAI::GetRandomSoldier()
{
    int i=mRandom->Next(5);
    if(i==0)
        return gcnew ZuseZ3(mRenderer->GetRedModel("Zusez3"), mPositionHQ + Vector3(25.f, -25.f, 0.f));
    else
        return gcnew Wirth(mRenderer->GetRedModel("Wirth"), mPositionHQ + Vector3(25.f, -25.f, 0.f));    
}

void HdB::PlayerAI::CheckSchedule(Object^ source, EventArgs^ e)
{
    if (Headquarters && Headquarters->PercentHP() <= 0.f)
        return;

    mSeconds++;
    static const Vector2 blockstattPos = Vector2(25.f, 25.f);

    // Define build schedule here, positions relative to the headquarters
    START_BUILDING(5, Blockhuette, "Blockhaus", Vector2(-25.f, -25.f));
    START_BUILDING(10, Blockfarm, "Kastenfarm", Vector2(25.f, -25.f));
    START_BUILDING(15, Blockwerk, "Blockwerk", Vector2(50.f, 50.f));
    START_BUILDING(20, Blockstatt, "Blockstatt", blockstattPos);
    //START_SOLDIER(30, ZuseZ3, "ZuseZ3", blockstattPos + Vector2(25.f, -25.f));

    //check for build/rebuild new Soldiers
    if(!CheckSoldiers())
    {
        for(int i=0; i < 5; ++i)    //TODO: -> paying ressources , positioning of build soldiers
        {
            Unit^ u=GetRandomSoldier();
            BuildUnit(u, u->BuildTime(), nullptr);
        }
    }

    //check randomly for attack
    if(mRandom->Next(100)==0)
        if(CheckSoldiers()) //TODO: -> dont interrupt attacks except for defending
        { 
            //attack here
            Attack(mEnemyUnits[mRandom->Next(mEnemyUnits->Count)]);
            
        }
}

void HdB::PlayerAI::OnNewUnit(Unit^ unit)
{
    if (unit->GetType()->IsSubclassOf(Soldier::typeid))
        mRenderer->Map->AddUnit(unit);
}

void HdB::PlayerAI::Save(BinaryWriter^ bw)
{
    Player::Save(bw);

    bw->Write(mPositionHQ.X);
    bw->Write(mPositionHQ.Y);
    bw->Write(mPositionHQ.Z);
    bw->Write(mSeconds);
}

void HdB::PlayerAI::Load(BinaryReader^ br,Renderer^ renderer)
{
    Player::Load(br,renderer);

    mPositionHQ.X=br->ReadSingle();
    mPositionHQ.Y=br->ReadSingle();
    mPositionHQ.Z=br->ReadSingle();
    mSeconds=br->ReadUInt64();
}
