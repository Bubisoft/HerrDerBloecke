#include "Unit.h"
#include "Model.h"
#include "Renderer.h"

#define DEFAULT_CONSTRUCTOR(UnitType, Parent) \
    HdB::UnitType::UnitType(HdB::Model^ model, const Vector3% pos) : Parent(model, pos) {}

HdB::Unit::Unit(HdB::Model^ model, const Vector3% pos)
{
    mModel = model;
    Position = pos;
    MoveTo = pos;
    mHP = MaxHP();
    mSpawned = false;

    ResetLookAt();
}

void HdB::Unit::Spawn()
{
    if (mSpawned)
        return;
    mModel->AddInstance(this);
    mSpawned = true;
}

void HdB::Unit::Despawn()
{
    if (!mSpawned)
        return;
    mModel->RemoveInstance(this);
    mSpawned = false;
}

Matrix HdB::Unit::GetTransform()
{
    if (LookAt == Position) // Failsafe
        LookAt = -Vector3::UnitY + Position;
    float angle = Math::Atan2(Position.Y - LookAt.Y, Position.X - LookAt.X);
    return Matrix::RotationZ(angle - Math::PI / 2.f) * Matrix::Translation(Position);
}

void HdB::Unit::Damage(int dmg) {
    mHP -= dmg;
    if (mHP <= 0) {
        mHP = 0;
        if (mSpawned)
            UnitDestroyed(this);
    }
}

void HdB::Unit::Save(BinaryWriter^ bw)
{

    bw->Write(this->GetType()->FullName->ToString()); //saving the name of the class to create the correct unit subclass in the laod function
    bw->Write(mModel->Name);
    bw->Write(mHP);

    bw->Write(mPosition.X);
    bw->Write(mPosition.Y);
    bw->Write(mPosition.Z);

    bw->Write(LookAt.X);
    bw->Write(LookAt.Y);
    bw->Write(LookAt.Z);
    
}

HdB::Unit^ HdB::Unit::Load(BinaryReader^ br,Renderer^ renderer,bool isblue)
{
    String^ typeName=br->ReadString();
    String^ model=br->ReadString();
    int HP=br->ReadInt32();

    Vector3 Pos;
    Pos.X=br->ReadSingle();
    Pos.Y=br->ReadSingle();
    Pos.Z=br->ReadSingle();

    Vector3 lookAt;
    lookAt.X=br->ReadSingle();
    lookAt.Y=br->ReadSingle();
    lookAt.Z=br->ReadSingle();

    Unit^ u;
    if(isblue)
        u = safe_cast<Unit^>(Activator::CreateInstance(Type::GetType(typeName),
                        gcnew array<Object^> {renderer->GetBlueModel(model),
                        Pos}));
    else
    {
        u = safe_cast<Unit^>(Activator::CreateInstance(Type::GetType(typeName),
                        gcnew array<Object^> {renderer->GetRedModel(model),
                        Pos}));
    }
    u->LookAt=lookAt;
    if(Blockhuette^ b=dynamic_cast<Blockhuette^>(u))
        b->Enabled=br->ReadBoolean();

    return u;
}

/*********************
 * Unit Constructors *
 *********************/

// Unit base types
HdB::Soldier::Soldier(HdB::Model^ model, const Vector3% pos) : Unit(model,pos)
{
    mAttackTimer=gcnew Timer();
    mAttackTimer->Enabled=false;
    mAttackTimer->Interval=1000 * this->AttackSpeed();
    mAttackTimer->Tick+=gcnew EventHandler(this, &HdB::Soldier::AttackCallback);
}

void HdB::Soldier::StartAttack(Unit^ target)
{
    AttackTarget=target;
    mAttackTimer->Start();
    MoveTo=AttackTarget->Position;
}

bool HdB::Soldier::IsInRange()
{
    if (!AttackTarget)
        return false;

    Vector3 range = Range() * Vector3::Normalize(Vector3::UnitX + Vector3::UnitY);
    Vector3 min = -range + AttackTarget->Position + AttackTarget->Model->Bounds.Minimum;
    Vector3 max = range + AttackTarget->Position + AttackTarget->Model->Bounds.Maximum;
    RectangleF r = RectangleF(min.X, min.Y, max.X - min.X, max.Y - min.Y);
    return r.Contains(Position.X, Position.Y);
}

void HdB::Soldier::StopAttack()
{
    mAttackTimer->Stop();
    AttackTarget = nullptr;
}

void HdB::Soldier::Damage(int dmg)
{
    Unit::Damage(dmg);

    if (mHP <= 0)
        StopAttack();
}

void HdB::Soldier::AttackCallback(Object^ sender, EventArgs^ e)
{
    if (!IsInRange())
    {
        //TODO: set the moveto to let the units move afterwards so they can attack moving units
        MoveTo=AttackTarget->Position;
        return;
    }
    LookAt=AttackTarget->Position;

    Soldier^ target = dynamic_cast<Soldier^>(AttackTarget);
    if(target) //Is it a Soldier
    {
        int dmg = Math::Round(this->Attack() * (1.f - target->Defense() / 100.f));
        AttackTarget->Damage(dmg);
    }
    else //No it's a Building
    {
        AttackTarget->Damage(this->Attack());
    }
}

DEFAULT_CONSTRUCTOR(ProductionBuilding, Building);

// Soldier types
DEFAULT_CONSTRUCTOR(ZuseZ3, Soldier);
DEFAULT_CONSTRUCTOR(Wirth, Soldier);
DEFAULT_CONSTRUCTOR(Gates, Soldier);
DEFAULT_CONSTRUCTOR(Turing, Soldier);

// Building types
DEFAULT_CONSTRUCTOR(Hauptgebaeude, Building);
DEFAULT_CONSTRUCTOR(Blockstatt, Building);
DEFAULT_CONSTRUCTOR(Blockwerk, ProductionBuilding);
DEFAULT_CONSTRUCTOR(Blockfarm, ProductionBuilding);

HdB::Building::Building(HdB::Model^ model, const Vector3% pos)
    : Unit(model, pos), mProgress(1.f)
{
}

HdB::Blockhuette::Blockhuette(HdB::Model^ model, const Vector3% pos)
    : ProductionBuilding(model, pos)
{
    Enabled = true;
}

void HdB::Blockhuette::Save(BinaryWriter^ br)
{
    Unit::Save(br);
    br->Write(Enabled);
}
