#include "Unit.h"

HdB::Unit::Unit(String^ model, const Vector3& pos)
{
    ModelName = model;
    Position = pos;
}


/********
 * TEMP *
 ********/
HdB::Teapot::Teapot(String^ model, const Vector3& pos)
    : Unit(model, pos)
{
}
