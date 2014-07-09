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

#define ATTACKPROBABILITY 100 
#define AGGRESSIVENESS 33 //probabilty for continuing attacks
#define DEFENSEDISTANCE 130 //distance in wich is searched for an enemy attack
#define DEFENDERDISTANCE 40 //distance in wich is searched for a defender
#define DIFFICULTY 1 //only chose between 1,2,3,4 !!!

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
    mLevel=DIFFICULTY;
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
    ADD_BUILDING(5, 5,Blockfarm, "Kastenfarm", Vector2(25.f, -25.f));
    ADD_BUILDING(10, 4,Blockhuette,"Blockhaus",Vector2(-25.f, -25.f)); 
    ADD_BUILDING(15, 4,Blockfarm, "Kastenfarm", Vector2(25.f, -25.f));
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

void HdB::PlayerAI::MoveUnits(const Vector3% pos)
{
    for each(Unit^ u in Units)
        if(Soldier^ s=dynamic_cast<Soldier^>(u))
            s->MoveTo=pos;
}

void HdB::PlayerAI::CheckHausShutdown()
{
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

    //check if we have to shutdown some blockhuetten to save food production
    if(foundFarm < foundHaus*2 + 1)
    {
        for each(Unit^ u in mUnits)
        {
            if(Blockhuette^ b=dynamic_cast<Blockhuette^>(u))
                b->Enabled=false;
        }
    }
    else
    {
        for each(Unit^ u in mUnits)
        {
            if(Blockhuette^ b=dynamic_cast<Blockhuette^>(u))
                b->Enabled=true;
        }
    }
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

    CheckHausShutdown();

    if(IsBeingAttacked)
    {
        //build more Soldiers
        int enemySoldier=0;
        for each(Unit^ u in mEnemyUnits)
            if(u->GetType()->IsSubclassOf(Soldier::typeid))
                ++enemySoldier;

        if(enemySoldier+(enemySoldier*(mLevel-2)/2) > mCountSoldier){
            Unit^ u=GetRandomSoldier();
            mEvents->Add(gcnew AIUnitEvent(0,5,u,nullptr));
            mCountSoldier++;
        }
    }
    else if(!IsBuilding)
    {

        if(mSeconds >= 70 && mSoldiers->Count >= 5)
        {
            //check if we have to build something
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
    if(!IsAttacking && mRandom->Next((ATTACKPROBABILITY/mLevel)) == 0)
        if(mSoldiers->Count > 0)
        {
            Unit^ target=mEnemyUnits[mRandom->Next(mEnemyUnits->Count)]; //determine random target
            for each(Soldier^ s in mSoldiers)
                mEvents->Add(gcnew AISoldierEvent(0, 0,s,target,s->AttackTarget, false));

        }
    //#####################################################

    //process events
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
                    if(sevent->IsDefense==true)
                        mDefendingSoldier->Remove(sevent->Soldier);

                    if(mRandom->Next(100) <= AGGRESSIVENESS * mLevel - 10) //start next attack
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

        if(AIUnitEvent^ uevent=dynamic_cast<AIUnitEvent^>(aievent)){ //check if we want to build soldiers
                if(uevent->Unit->GetType()->IsSubclassOf(Soldier::typeid))
                    IsBuildingSoldier=true;
                else
                    IsBuilding=true;
            }
        
        if(aievent->AtTime > mSeconds)
            continue;
        if(mToDo==nullptr)
            mToDo=aievent;

        if(AIUnitEvent^ ubuildingevent=dynamic_cast<AIUnitEvent^>(aievent))
        {
            if(ubuildingevent->Unit->GetType()->IsSubclassOf(Building::typeid) && !ubuildingevent->CanProcess(Res)) //wait for ressources for a building
                IsWaitingForBuilding=true;
        }
        if(IsWaitingForBuilding)
        {
            mToDo=aievent;
        }
        else if(aievent->CanProcess(Res) && (aievent->Priority > mToDo->Priority))
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


bool HdB::PlayerAI::IsInDistance(Soldier^ defender,Soldier^ attacker)
{
    float distance=Vector3::Distance(attacker->Position,defender->Position); 
    if(distance < DEFENDERDISTANCE*mLevel)
        return true;

    return false;
}

HdB::Soldier^ HdB::PlayerAI::GetDefender(Soldier^ attacker)
{
    Soldier^ defender=nullptr;
    float olddistance=99999999;
    for each(Soldier^ s in mSoldiers)
        if(!mDefendingSoldier->Contains(s))
        {
            float distance=Vector3::Distance(attacker->Position,s->Position); 
            if(distance <= olddistance)
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
                    //let soldiers defend theirselves and try to pull two others
                    mEvents->Add(gcnew AISoldierEvent(0,2,attacked,attacker,attacked->AttackTarget,true));
                    Soldier^ defender1=GetDefender(attacker);
                    Soldier^ defender2= GetDefender(attacker);

                    if(defender1 != nullptr && IsInDistance(defender1, attacker)){
                        mEvents->Add(gcnew AISoldierEvent(0,2,defender1,attacker,defender1->AttackTarget,true));
                        mDefendingSoldier->Add(defender1);
                    }
                    if(defender2 != nullptr && IsInDistance(defender2, attacker)){
                        mEvents->Add(gcnew AISoldierEvent(0,2,defender2,attacker,defender2->AttackTarget,true));
                        mDefendingSoldier->Add(defender2);
                    }
                    
                }
                else
                {
                    //pull one defender every tick
                    Soldier^ defender=GetDefender(attacker);
                    if(defender!=nullptr){
                        mEvents->Add(gcnew AISoldierEvent(0,2,defender,attacker,defender->AttackTarget,true));
                        mDefendingSoldier->Add(defender);
                    }
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
        soldier->StopAttack();
        mSoldiers->Remove(soldier);
        mCountSoldier--;
        for each(AIEvent^ aievent in mEvents->ToArray())
            if(AISoldierEvent^ sevent = dynamic_cast<AISoldierEvent^>(aievent))
                if(sevent->Soldier == soldier)
                    mEvents->Remove(aievent);
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

        if(!mRenderer->Map->OnMap(unit->Position))
        {
            unit->Position=placeholder->Position;
            unit->Position+=Vector3::UnitY;
            placeholder->Position+=Vector3::UnitY;
        }
    }
    if (placeholder)
    {
        placeholder->Position = unit->Position;
        placeholder->MoveTo = unit->MoveTo;
        placeholder->ResetLookAt();
    }
}
