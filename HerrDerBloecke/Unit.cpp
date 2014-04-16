#include "Unit.h"
#include "Model.h"

HdB::Unit::Unit(HdB::Model^ model, const Vector3% pos)
{
    mModel = model;
    Position = pos;
    LookAt = -Vector3::UnitY + Position;
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

/********
 * TEMP *
 ********/
HdB::TestUnit::TestUnit(HdB::Model^ model, const Vector3% pos)
    : Unit(model, pos)
{
}
