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

    //Initialize Event Timer
    mEventTimer = gcnew Timer();
    mEventTimer->Interval = 400;
    mEventTimer->Tick += gcnew EventHandler(this, &PlayerAI::CheckEvents);
    mEventTimer->Enabled = true;

    // Initialize Randomobject
    mRandom=gcnew Random((int)DateTime::Now.Ticks);

    // Register callback for soldiers
    UnitBuilt += gcnew UnitEvent(this, &PlayerAI::OnNewUnit);
    UnitDestroyed +=gcnew UnitEvent(this, &PlayerAI::OnUnitDestroyed);
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

void HdB::PlayerAI::Attack(Unit^ target, bool isDefense)
{
    for each(Unit^ u in Units) //start attack with all Soldiers
    {
        if(Soldier^ s= dynamic_cast<Soldier^>(u))
        {
            if(isDefense || s->AttackTarget == nullptr)
            {
                s->StartAttack(target);
                //s->MoveTo = target->Position;
            }
        }
    }
}

Unit^ HdB::PlayerAI::GetRandomSoldier()
{
    int i=mRandom->Next(3);
    Unit^ unit;

    if(i==0)
        unit= gcnew ZuseZ3(mRenderer->GetRedModel("Zusez3"), mPositionHQ + Vector3(25.f, -25.f, 0.f));
    else if(i==1)
        unit= gcnew Wirth(mRenderer->GetRedModel("Wirth"), mPositionHQ + Vector3(25.f, -25.f, 0.f));
    else if(i==2)
        unit= gcnew Gates(mRenderer->GetRedModel("Gates"), mPositionHQ + Vector3(25.f, -25.f, 0.f));
    else
        unit= gcnew Turing(mRenderer->GetRedModel("Turing"), mPositionHQ + Vector3(25.f, -25.f, 0.f));

    while(!mRenderer->Map->CanBuild(unit)) 
    {
        unit->Position = unit->Position + Vector3(5,0,0);
        unit->MoveTo = unit->Position;
    }
    return unit;
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
    START_BUILDING(12, Blockfarm, "Kastenfarm", Vector2(35.f, -25.f));
    START_BUILDING(15, Blockwerk, "Blockwerk", Vector2(50.f, 50.f));
    START_BUILDING(20, Blockstatt, "Blockstatt", blockstattPos);
    START_SOLDIER(30, ZuseZ3, "ZuseZ3", blockstattPos + Vector2(25.f, -25.f));
}

void HdB::PlayerAI::CheckEvents(Object^ source, EventArgs^ e)
{
    //check if something is being attacked to defend
    Unit^ attacker=IsAttacked();
    if( attacker != nullptr )
    {
        Attack(attacker, true);
    }

    //check for build/rebuild new Soldiers
    if(!CheckSoldiers() &&  mSeconds >= 60)
    {
        for(int i=0; i < 5; ++i)    //TODO: ->positioning of built soldiers
        {
            Unit^ u=GetRandomSoldier();
            if(Res->CheckAmount(u->GetCosts()))
            {
                BuildUnit(u, u->BuildTime(), nullptr);
            }
        }
    }

    //check randomly for attack
    if(mRandom->Next(40) == 0)
        if(CheckSoldiers()) { 
            Attack(mEnemyUnits[mRandom->Next(mEnemyUnits->Count)], false);
        }

}

Unit^ HdB::PlayerAI::IsAttacked()
{
    for each(Unit^ u in Units)
    {
        for each(Unit^ a in mEnemyUnits)
        {
            if(Soldier^ s = dynamic_cast<Soldier^>(a))
                if(s->AttackTarget == u)
                    return s;
        }
    }
    
    return nullptr;
}

void HdB::PlayerAI::OnNewUnit(Unit^ unit)
{
    if (unit->GetType()->IsSubclassOf(Soldier::typeid))
        mRenderer->Map->AddUnit(unit);

    if(HdB::ProductionBuilding^ b= dynamic_cast<ProductionBuilding^>(unit))
    {
        if(b->GetProductionType() == ProductionType::eBlockterie)
            AddBlockterieUnit(1);
        else if(b->GetProductionType() == ProductionType::eFood)
            AddFoodUnit(1);
        else
            AddGoldUnit(1);
    }
}

void HdB::PlayerAI::OnUnitDestroyed(Unit^ unit)
{
    if(HdB::ProductionBuilding^ b= dynamic_cast<ProductionBuilding^>(unit))
    {
        if(b->GetProductionType() == ProductionType::eBlockterie)
            AddBlockterieUnit(-1);
        else if(b->GetProductionType() == ProductionType::eFood)
            AddFoodUnit(-1);
        else
            AddGoldUnit(-1);
    }
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
