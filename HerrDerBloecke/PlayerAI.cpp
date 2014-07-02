#include "PlayerAI.h"
#include "Renderer.h"
#include "Map.h"
#include "Unit.h"

// Schedule macros
#define ADD_BUILDING(atTime, priority,UnitType, Model, pos) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        Unit^ ph = gcnew UnitType(mRenderer->GetAlphaModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        mEvents->Add(gcnew AIUnitEvent(atTime,priority,u,ph)); }

#define ADD_SOLDIER(atTime, priority,UnitType, Model, pos) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        mEvents->Add(gcnew AIUnitEvent(atTime,priority,u,nullptr)); }

#define AGGRESSIVENESS 70 //default 70
#define DEFENSEDISTANCE 130 //default 130
HdB::PlayerAI::PlayerAI(Renderer^ renderer, List<Unit^>^ enemyUnits) : mRenderer(renderer), mEnemyUnits(enemyUnits)
{
    // Initialize AI schedule timer
    mTimer = gcnew Timer();
    mTimer->Interval = 1000;
    mTimer->Tick += gcnew EventHandler(this, &PlayerAI::CheckSchedule);
    mTimer->Enabled = true;

    // Initialize boolean parameter for letting him do his schedule
    IsBuildingFarm=true;
    IsBuildingHaus=true;
    IsBuildingStatt=true;
    IsBuildingWerk=true;

    // Initialize Randomobject
    mRandom=gcnew Random((int)DateTime::Now.Ticks);

    // Register callback for soldiers
    UnitBuilt += gcnew UnitEvent(this, &PlayerAI::OnNewUnit);
    UnitDestroyed +=gcnew UnitEvent(this, &PlayerAI::OnUnitDestroyed);

    IsBlue=false;
    mToDo=nullptr;

    mEvents=gcnew List<AIEvent^>();
    mDefendingSoldier=gcnew List<Soldier^>();
    IsMissingBuilding=true;
    IsBuilding=false;
    CanBuildSoldier=false;
    mSoldiers=gcnew List<Soldier^>();
}

void HdB::PlayerAI::NewGame(const Vector3% pos)
{
    // Spawn Headquarters
    mPositionHQ = pos;
    Unit^ u = gcnew Hauptgebaeude(mRenderer->GetRedModel("Hauptgebaeude"), mPositionHQ);
    BuildUnit(u, u->BuildTime(), nullptr);
    mRenderer->Map->AddUnit(u);
    Headquarters = u;

    // Initial schedule
    static const Vector2 blockstattPos = Vector2(25.f, 25.f);
    ADD_BUILDING(5, 5,Blockhuette,"Blockhaus",Vector2(-25.f, -25.f));
    ADD_BUILDING(10, 4,Blockfarm, "Kastenfarm", Vector2(25.f, -25.f));
    ADD_BUILDING(20, 4,Blockfarm, "Kastenfarm", Vector2(25.f, -25.f));
    ADD_BUILDING(15, 3, Blockwerk, "Blockwerk", Vector2(50.f, 50.f));
    ADD_BUILDING(20, 2, Blockstatt, "Blockstatt", blockstattPos);
}

int HdB::PlayerAI::CheckSoldiers()
{
    int i=0;
    for each(Unit^ u in Units)
    {
        if(u->GetType()->IsSubclassOf(Soldier::typeid))
            ++i;
    }
    return i;
}

void HdB::PlayerAI::Attack(Unit^ target, bool isDefense)
{

}

void HdB::PlayerAI::MoveUnits(const Vector3% pos)
{
    for each(Unit^ u in Units)
        if(Soldier^ s=dynamic_cast<Soldier^>(u))
            s->MoveTo=pos;
}

void HdB::PlayerAI::CheckBuilding()
{
    Unit^ unit=nullptr;
    Unit^ alpha = nullptr;
    UInt16 foundFarm = 0, foundStatt = 0, foundHaus = 0, foundWerk = 0;
    

    for each (Unit^ u in Units) {
        Type^ t=u->GetType();
        if(t == Blockfarm::typeid)
            foundFarm++;
        else if(t == Blockstatt::typeid)
            foundStatt++;
        else if(t == Blockwerk::typeid)
            foundWerk++;
        else if(t == Blockhuette::typeid)
            foundHaus++;
    }

    if(foundFarm < foundHaus*2 + 1){
        unit=gcnew Blockfarm(mRenderer->GetRedModel("Kastenfarm"),mPositionHQ + Vector3(25.f, -25.f, 0.f));
        alpha=gcnew Blockfarm(mRenderer->GetAlphaModel("Kastenfarm"),mPositionHQ + Vector3(25.f, -25.f, 0.f));
    }
    else if(foundStatt < foundWerk){
        unit=gcnew Blockstatt(mRenderer->GetRedModel("Blockstatt"),mPositionHQ + Vector3(25.f, 25.f, 0.f));
        alpha=gcnew Blockstatt(mRenderer->GetAlphaModel("Blockstatt"),mPositionHQ + Vector3(25.f, 25.f, 0.f));
    }
    else if(foundWerk < foundHaus){
        unit=gcnew Blockwerk(mRenderer->GetRedModel("Blockwerk"),mPositionHQ + Vector3(50.f, 50.f, 0.f));
        alpha=gcnew Blockwerk(mRenderer->GetAlphaModel("Blockwerk"),mPositionHQ + Vector3(50.f, 50.f, 0.f));
    }
    else{
        unit=gcnew Blockhuette(mRenderer->GetRedModel("Blockhaus"),mPositionHQ + Vector3(-25.f, 25.f, 0.f));
        alpha=gcnew Blockhuette(mRenderer->GetAlphaModel("Blockhaus"),mPositionHQ + Vector3(-25.f, 25.f, 0.f));
    }

    mEvents->Add(gcnew AIUnitEvent(mSeconds+30, 3, unit, alpha));
}

HdB::Unit^ HdB::PlayerAI::GetRandomSoldier()
{
    int i=mRandom->Next(20);
    Unit^ unit;

    if(i>=0 && i < 9)
        unit= gcnew ZuseZ3(mRenderer->GetRedModel("Zusez3"), mPositionHQ + Vector3(25.f, -40.f, 0.f));
    else if(i >= 9 && i < 15)
        unit= gcnew Wirth(mRenderer->GetRedModel("Wirth"), mPositionHQ + Vector3(25.f, -40.f, 0.f));
    else if(i >= 15 && i < 20)
        unit= gcnew Gates(mRenderer->GetRedModel("Gates"), mPositionHQ + Vector3(25.f, -40.f, 0.f));
    else
        unit= gcnew Turing(mRenderer->GetRedModel("Turing"), mPositionHQ + Vector3(25.f, -40.f, 0.f));

    PositionUnit(unit, nullptr);
    return unit;
}

void HdB::PlayerAI::CheckSchedule(Object^ source, EventArgs^ e)
{
    if (Headquarters && Headquarters->PercentHP() <= 0.f)
        return;

    mSeconds++;

    //Check all Events #################################
    //check if something is being attacked to defend
    IsAttacked();

    if(IsBeingAttacked)
    {
        //build more Soldiers
        if(mEnemyUnits->Count > mCountSoldier){
            Unit^ u=GetRandomSoldier();
            mEvents->Add(gcnew AIUnitEvent(0,5,u,nullptr));
            mCountSoldier++;
        }
    }
    else if(!IsBuilding)
    {
        //check rebuilding destroyed buildings
        CheckMissingBuilding();

        if(mSeconds >= 70 && mSoldiers->Count >= 5)
        {
            CheckBuilding();
        }

        /*check for build/rebuild new Soldiers. 
        */
        if(!IsBuilding && CanBuildSoldier && (!IsBuildingSoldier) && (mCountSoldier <= 5) &&  mSeconds >= 40)
        {
                Unit^ u=GetRandomSoldier();
                mEvents->Add(gcnew AIUnitEvent(mSeconds + 5, 2, u, nullptr));
                mCountSoldier++;
        } 
    }

    //check randomly for attack 
    if(!IsAttacking && mRandom->Next(40) == 0)
        if(mSoldiers->Count > 0)
        {
            Unit^ target=mEnemyUnits[mRandom->Next(mEnemyUnits->Count)]; //determine random target
            for each(Soldier^ s in mSoldiers)
                mEvents->Add(gcnew AISoldierEvent(0, 0,s,target,s->AttackTarget, false));

        }
    //#####################################################

    //process 
    IsBuildingSoldier=false;
    IsAttacking=false;
    IsWaitingForBuilding=false;
    mToDo=nullptr;
    for each(AIEvent^ aievent in mEvents->ToArray())
    {
        //always process soldierevents
        if(aievent->Status==EventStatus::NEW)
        {
            if(AISoldierEvent^ sevent= dynamic_cast<AISoldierEvent^>(aievent))
            {

                    sevent->Soldier->StartAttack(sevent->Target);

                    if(sevent->IsDefense)
                        mDefendingSoldier->Add(sevent->Soldier);

                    sevent->Status=HdB::EventStatus::OPEN;
                    IsAttacking=true;
            }
        }
        else if(aievent->Status==HdB::EventStatus::OPEN)
        {
            if(AISoldierEvent^ sevent=dynamic_cast<AISoldierEvent^>(aievent))
            {
                if(sevent->Soldier->AttackTarget == nullptr && sevent->OldTarget != nullptr) //continue attack on oldtarget wich was interrupted by defense call
                { 
                    sevent->Soldier->StartAttack(sevent->OldTarget);
                    if(sevent->IsDefense==true)
                        mDefendingSoldier->Remove(sevent->Soldier);
                }
                else if(sevent->Soldier->AttackTarget==nullptr && sevent->OldTarget==nullptr) //attack was succesfull, pull back or start next attack
                {
                    //start next attack or pull back Soldiers
                    if(sevent->IsDefense==true)
                        mDefendingSoldier->Remove(sevent->Soldier);

                    if(mRandom->Next(100) <= AGGRESSIVENESS) //default 70%
                    {
                        mEvents->Add(gcnew AISoldierEvent(0,2,sevent->Soldier,mEnemyUnits[mRandom->Next(mEnemyUnits->Count)],nullptr,false));
                    }
                    else //pull back soldier
                    {
                        MoveUnits(mPositionHQ + Vector3(10.f,-10.f,0.f));
                    }
                    mEvents->Remove(aievent);

                }
                IsAttacking=true;
            }
        }

        if(AIUnitEvent^ uevent=dynamic_cast<AIUnitEvent^>(aievent)){
                if(uevent->Unit->GetType()->IsSubclassOf(Soldier::typeid))
                    IsBuildingSoldier=true;
                else
                    IsBuilding=true;
            }
        
        if(aievent->AtTime > mSeconds)
            continue;

        if(mToDo!=nullptr)
        {
            if(AIUnitEvent^ ubuildingevent=dynamic_cast<AIUnitEvent^>(aievent))
            {
                if(ubuildingevent->Unit->GetType()->IsSubclassOf(Building::typeid) && !ubuildingevent->CanProcess(Res)) //wait for ressources
                    IsWaitingForBuilding=true;
            }
            if(IsWaitingForBuilding)
                mToDo=nullptr;
            else if(aievent->CanProcess(Res) && (aievent->Priority > mToDo->Priority))
                mToDo=aievent;
        }
        else if(aievent->CanProcess(Res))
            mToDo=aievent;
           
    }
    
    if(mToDo==nullptr)
        return;

    //process the todo event
    if(AIUnitEvent^ uevent=dynamic_cast<AIUnitEvent^>(mToDo))
    {
        Unit^ u=uevent->Unit;
        PositionUnit(u, uevent->Alpha);
        BuildUnit(u,u->BuildTime(),uevent->Alpha);
        mRenderer->Map->AddUnit(u);
        mEvents->Remove(mToDo);
    }
}


void HdB::PlayerAI::CheckMissingBuilding()
{
 
    UInt16 foundFarm = 0, foundStatt = 0, foundHaus = 0, foundWerk = 0;
    Unit^ unit = nullptr;
    Unit^ alpha = nullptr;

    for each (Unit^ u in Units) {
        Type^ t=u->GetType();
        if(t == Blockfarm::typeid)
            foundFarm++;
        else if(t == Blockstatt::typeid)
            foundStatt++;
        else if(t == Blockwerk::typeid)
            foundWerk++;
        else if(t == Blockhuette::typeid)
            foundHaus++;
    }

    if (!IsBuildingFarm && foundFarm < 2) {
        unit=gcnew Blockfarm(mRenderer->GetRedModel("Kastenfarm"),mPositionHQ + Vector3(25.f,-25.f,0.f));
        alpha=gcnew Blockfarm(mRenderer->GetAlphaModel("Kastenfarm"),mPositionHQ + Vector3(25.f,-25.f,0.f));
        IsMissingBuilding=true;
        IsBuildingFarm=true;
    } else if(!IsBuildingHaus && foundHaus < 1) {
        unit=gcnew Blockhuette(mRenderer->GetRedModel("Blockhaus"),mPositionHQ + Vector3(-25.f, -25.f, 0.f));
        alpha=gcnew Blockhuette(mRenderer->GetAlphaModel("Blockhaus"),mPositionHQ + Vector3(-25.f, -25.f, 0.f));
        IsMissingBuilding=true;
        IsBuildingHaus=true;
    } else if(!IsBuildingWerk && foundWerk < 1) {
        unit=gcnew Blockwerk(mRenderer->GetRedModel("Blockwerk"),mPositionHQ + Vector3(50.f, 50.f, 0.f));
        alpha=gcnew Blockwerk(mRenderer->GetAlphaModel("Blockwerk"),mPositionHQ + Vector3(50.f, 50.f, 0.f));
        IsMissingBuilding=true;
        IsBuildingWerk=true;
    } else if(!IsBuildingStatt && foundStatt < 1) {
        unit=gcnew Blockstatt(mRenderer->GetRedModel("Blockstatt"),mPositionHQ + Vector3(25.f, 25.f, 0.f));
        alpha=gcnew Blockstatt(mRenderer->GetAlphaModel("Blockstatt"),mPositionHQ + Vector3(25.f, 25.f, 0.f));
        IsMissingBuilding=true;
        IsBuildingStatt=true;
    }
    else {
        IsMissingBuilding=false;
        return;
    }
    mEvents->Add(gcnew AIUnitEvent(mSeconds + 5,5,unit,alpha));
    IsBuilding=true;
}

HdB::Soldier^ HdB::PlayerAI::GetDefender(Soldier^ attacker)
{
    Soldier^ defender=nullptr;
    float olddistance=99999999;
    for each(Soldier^ s in mSoldiers)
        if(!mDefendingSoldier->Contains(s))
        {
            
            if(Vector3::Distance(attacker->Position,s->Position) <= olddistance)
            {
                olddistance=Vector3::Distance(attacker->Position,s->Position);
                defender=s;
            }
        }

    return defender;
}

void HdB::PlayerAI::IsAttacked()
{
    IsBeingAttacked=false;
    for each(Unit^ a in mEnemyUnits)
    {
        if(Soldier^ attacker = dynamic_cast<Soldier^>(a))
            if((attacker->AttackTarget !=nullptr && Vector3::Distance(attacker->Position,mPositionHQ) < DEFENSEDISTANCE) || attacker->IsInRange())
            {
                Soldier^ attacked=dynamic_cast<Soldier^>(attacker->AttackTarget);
                if(!mDefendingSoldier->Contains(attacked) && attacked!=nullptr)
                {
                    //let soldiers defend theirselves
                    mEvents->Add(gcnew AISoldierEvent(0,2,attacked,attacker,attacked->AttackTarget,true));
                    Soldier^ s1=GetDefender(attacker);
                    Soldier^ s2= GetDefender(attacker);

                    if(s1 != nullptr)
                        mEvents->Add(gcnew AISoldierEvent(0,2,s1,attacker,s1->AttackTarget,true));
                    if(s2 != nullptr)
                        mEvents->Add(gcnew AISoldierEvent(0,2,s2,attacker,s2->AttackTarget,true));
                    
                }
                else
                {
                    Soldier^ defender=GetDefender(attacker);
                    if(defender!=nullptr)
                        mEvents->Add(gcnew AISoldierEvent(0,2,defender,attacker,defender->AttackTarget,true));
                }
                IsBeingAttacked=true;
            }

    }
}

void HdB::PlayerAI::OnNewUnit(Unit^ unit)
{
    if (!(unit->GetType()->IsSubclassOf(Soldier::typeid)))
        IsBuilding=false;

    if(HdB::ProductionBuilding^ b= dynamic_cast<ProductionBuilding^>(unit))
    {
        if(b->GetProductionType() == ProductionType::eBlockterie)
        {
            IsBuildingWerk=false;
            AddBlockterieUnit(1);
        }
        else if(b->GetProductionType() == ProductionType::eFood)
        {
            IsBuildingFarm=false;
            AddFoodUnit(1);
        }
        else
        {
            IsBuildingHaus=false;
            AddGoldUnit(1);
        }
    }
    else if(HdB::Blockstatt^ s=dynamic_cast<Blockstatt^>(unit))
    {
        IsBuildingStatt=false;
        CanBuildSoldier=true;
    }
    else if(HdB::Soldier^ soldier=dynamic_cast<Soldier^>(unit))
        mSoldiers->Add(soldier);
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
    } else if (HdB::Blockstatt^ s = dynamic_cast<Blockstatt^>(unit)) {
        CanBuildSoldier = false;
    }
    else if(HdB::Soldier^ soldier=dynamic_cast<Soldier^>(unit)){
        mSoldiers->Remove(soldier);
        mCountSoldier--;
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
    Player::Load(br, renderer);

    mPositionHQ.X = br->ReadSingle();
    mPositionHQ.Y = br->ReadSingle();
    mPositionHQ.Z = br->ReadSingle();
    mSeconds = br->ReadUInt64();
}

void HdB::PlayerAI::PositionUnit(Unit^ unit, Unit^ placeholder)
{
    while (!mRenderer->Map->CanBuild(unit)) 
    {
        unit->Position += Vector3::UnitX;
        unit->MoveTo = unit->Position;
        unit->ResetLookAt();
    }
    if (placeholder)
    {
        placeholder->Position = unit->Position;
        placeholder->MoveTo = unit->MoveTo;
        placeholder->ResetLookAt();
    }
}
