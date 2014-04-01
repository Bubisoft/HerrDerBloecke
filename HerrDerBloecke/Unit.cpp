#include "Unit.h"

HdB::Unit::Unit(String^ model, const Vector3% pos)
{
    ModelName = model;
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

HdB::Building::Building(String^ model, const Vector3% pos)
    : Unit(model, pos)
{
}

HdB::Soldier::Soldier(String^ model, const Vector3% pos)
    : Unit(model, pos)
{
}

/********
 * TEMP *
 ********/
HdB::TestUnit::TestUnit(String^ model, const Vector3% pos)
    : Unit(model, pos)
{
}
