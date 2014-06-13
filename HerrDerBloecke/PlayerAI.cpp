#include "PlayerAI.h"
#include "Renderer.h"
#include "Map.h"
#include "Unit.h"

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

#define ADD_BUILDING(atTime, priority,UnitType, Model, pos) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        Unit^ ph = gcnew UnitType(mRenderer->GetAlphaModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        mEvents->Add(gcnew AIUnitEvent(atTime,priority,u,ph)); }

#define ADD_SOLDIER(atTime, priority,UnitType, Model, pos) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        mEvents->Add(gcnew AIUnitEvent(atTime,priority,u,nullptr)); }

HdB::PlayerAI::PlayerAI(Renderer^ renderer, const Vector3% posHQ, List<Unit^>^ enemyUnits) : mRenderer(renderer), mPositionHQ(posHQ), mEnemyUnits(enemyUnits)
{
    // Initialize AI schedule timer
    mTimer = gcnew Timer();
    mTimer->Interval = 1000;
    mTimer->Tick += gcnew EventHandler(this, &PlayerAI::CheckSchedule);
    mTimer->Enabled = true;

    //Initialize boolean parameter for letting him do his schedule
    IsBuildingFarm=true;
    IsBuildingHaus=true;
    IsBuildingStatt=true;
    IsBuildingWerk=true;

    // Initialize Randomobject
    mRandom=gcnew Random((int)DateTime::Now.Ticks);

    // Register callback for soldiers
    UnitBuilt += gcnew UnitEvent(this, &PlayerAI::OnNewUnit);
    UnitDestroyed +=gcnew UnitEvent(this, &PlayerAI::OnUnitDestroyed);

    // Spawn Headquarters
    Unit^ u = gcnew Hauptgebaeude(mRenderer->GetRedModel("Hauptgebaeude"), mPositionHQ);
    u->Spawn();
    mRenderer->Map->AddUnit(u);
    Units->Add(u);
    Headquarters = u;

    IsBlue=false;
    mToDo=nullptr;

    //TEMP
    mEvents=gcnew List<AIEvent^>();
    IsMissingBuilding=true;
    IsBuilding=false;
    CanBuildSoldier=false;
    mSoldiers=gcnew List<Soldier^>();

    static const Vector2 blockstattPos = Vector2(25.f, 25.f);
    ADD_BUILDING(5, 5,Blockhuette,"Blockhaus",Vector2(-25.f, -25.f));
    ADD_BUILDING(10, 4,Blockfarm, "Kastenfarm", Vector2(25.f, -25.f));
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
    for each(Unit^ u in Units) //start attack with all Soldiers
    {
        if(Soldier^ s= dynamic_cast<Soldier^>(u))
        {
            if(isDefense || s->AttackTarget == nullptr)
            {
                mEvents->Add(gcnew AISoldierEvent(0, 4, s, target, s->AttackTarget, isDefense));
            }
        }
    }
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

    mEvents->Add(gcnew AIUnitEvent(mSeconds+5, 3, unit, alpha));
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

    while(!mRenderer->Map->CanBuild(unit)) 
    {
        unit->Position = unit->Position + Vector3(1,0,0);
        unit->MoveTo = unit->Position;
    }
    return unit;
}

void HdB::PlayerAI::CheckSchedule(Object^ source, EventArgs^ e)
{
    if (Headquarters && Headquarters->PercentHP() <= 0.f)
        return;

    mSeconds++;

    //Check all Events #################################
    //check if something is being attacked to defend
    Unit^ attacker=IsAttacked();
    if( attacker != nullptr )
    {
        IsBeingAttacked=true;
        Attack(attacker, true);
    }
    else
        IsBeingAttacked=false;

    if(!IsBuilding)
    {
        //check rebuilding destroyed buildings
        CheckMissingBuilding();

        /*check for build/rebuild new Soldiers. 
        Only do this if we are not waiting for a building except we are being attacked
        */
        //if(!IsMissingBuilding || IsBeingAttacked)
        if(CanBuildSoldier && (!IsBuildingSoldier) && (mSoldiers->Count < 5) &&  mSeconds >= 40)
        {
                Unit^ u=GetRandomSoldier();
                mEvents->Add(gcnew AIUnitEvent(mSeconds, 2, u, nullptr));           
        }
    
        if(mSeconds >= 70 && mSoldiers->Count >= 5)
        {
            CheckBuilding();
        }
    }

    //check randomly for attack  // have TODO this again !!!
    if(mRandom->Next(40) == 0)
        if(mSoldiers->Count > 0)
        {
            Unit^ target=mEnemyUnits[mRandom->Next(mEnemyUnits->Count)]; //determine random target
            //for each(Soldier^ s in mSoldiers)
            // mEvents->Add(gcnew AISoldierEvent(0, 4,s,target,s->AttackTarget, false));
        }
    //#####################################################

    //process 
    IsBuildingSoldier=false;
    IsWaitingForBuilding=false;
    mToDo=nullptr;
    for each(AIEvent^ aievent in mEvents)
    {

        //try to finish open events
        if(aievent->Status==HdB::EventStatus::OPEN)
        {
            if(AISoldierEvent^ sevent=dynamic_cast<AISoldierEvent^>(aievent))
                if(sevent->Target==nullptr && sevent->OldTarget!=nullptr && sevent->IsDefense) //continue attack on oldtarget wich was interrupted by defense call
                { 
                    sevent->Soldier->StartAttack(sevent->OldTarget);
                    sevent->Status=HdB::EventStatus::CLOSED;
                }
                else if(sevent->OldTarget==nullptr)
                {
                    //start next attack or pull back Soldiers
                    MoveUnits(mPositionHQ + Vector3(10.f,-10.f,0.f));
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
        if(u->GetType()->IsSubclassOf(Building::typeid))
            mRenderer->Map->AddUnit(u);
        uevent->Status=HdB::EventStatus::CLOSED;
    }
    else if( AISoldierEvent^ sevent= dynamic_cast<AISoldierEvent^>(mToDo))
    {
        if(sevent->Target!=nullptr)
            Attack(sevent->Target,sevent->IsDefense);
        else if(sevent->OldTarget!=nullptr)
            Attack(sevent->OldTarget,false);
        else
            MoveUnits(mPositionHQ + Vector3(5.f, 5.f, 0.f));

        sevent->Status=HdB::EventStatus::OPEN;
    }
    
    for each(AIEvent^ aievent in mEvents->ToArray())
    {
        if(aievent->Status==HdB::EventStatus::CLOSED)
            mEvents->Remove(aievent);
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


HdB::Unit^ HdB::PlayerAI::IsAttacked()
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
    else
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
    else if(HdB::Soldier^ soldier=dynamic_cast<Soldier^>(unit))
        mSoldiers->Remove(soldier);
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
    while(!mRenderer->Map->CanBuild(unit)) 
    {
        unit->Position = unit->Position + Vector3(5,0,0);
        unit->MoveTo = unit->Position;
    }
    if(placeholder!=nullptr)
    {
        placeholder->Position=unit->Position;
        placeholder->MoveTo=unit->MoveTo;
    }
}