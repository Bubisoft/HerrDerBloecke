#include "Unit.h"
#include "Model.h"
#include "Renderer.h"
HdB::Unit::Unit(HdB::Model^ model, const Vector3% pos)
{
    mModel = model;
    Position = pos;
    LookAt = -Vector3::UnitY + Position;
    mHP = MaxHP();
    mSpawned = false;
}

void HdB::Unit::Spawn()
{
    mModel->AddInstance(this);
    mSpawned = true;
}

void HdB::Unit::Despawn()
{
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

HdB::Unit^ HdB::Unit::Load(BinaryReader^ br,Renderer^ renderer)
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

    Unit^ u = safe_cast<Unit^>(Activator::CreateInstance(Type::GetType(typeName),
                        gcnew array<Object^> {renderer->GetModel(model),
                        Pos}));
    u->LookAt=lookAt;
    if(Blockhuette^ b=dynamic_cast<Blockhuette^>(u))
        b->enabled=br->ReadBoolean();

    return u;
}

HdB::Building::Building(HdB::Model^ model, const Vector3% pos)
    : Unit(model, pos)
{
}

HdB::Soldier::Soldier(HdB::Model^ model, const Vector3% pos)
    : Unit(model, pos)
{
}

HdB::ProductionBuilding::ProductionBuilding(HdB::Model^ model, const Vector3% pos)
    : Building(model, pos)
{
}


/*************
 * BUILDINGS *
 *************/

HdB::Blockhuette::Blockhuette(HdB::Model^ model, const Vector3% pos)
    : ProductionBuilding(model, pos), enabled(true)
{
}

void HdB::Blockhuette::Save(BinaryWriter^ br)
{
    Unit::Save(br);
    br->Write(enabled);
}

HdB::Blockstatt::Blockstatt(HdB::Model^ model, const Vector3% pos)
    : Building(model, pos)
{
}

HdB::Blockwerk::Blockwerk(HdB::Model^ model, const Vector3% pos)
    : ProductionBuilding(model, pos)
{
}

HdB::Blockfarm::Blockfarm(HdB::Model^ model, const Vector3% pos)
    : ProductionBuilding(model, pos)
{
}



/********
 * TEMP *
 ********/
HdB::TestUnit::TestUnit(HdB::Model^ model, const Vector3% pos)
    : Soldier(model, pos)
{
}
