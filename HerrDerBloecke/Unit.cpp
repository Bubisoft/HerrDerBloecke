#include "Unit.h"
#include "Model.h"

HdB::Unit::Unit(HdB::Model^ model, const Vector3% pos)
{
    mModel = model;
    Position = pos;
    MoveTo = pos;
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
