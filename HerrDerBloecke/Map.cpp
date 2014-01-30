#include "Map.h"
#include "Globals.h"

using namespace System::IO;
using namespace System::Diagnostics;

#define FIELDS_X 128
#define FIELDS_Y 128
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 10

/*******
 * Map *
 *******/

HdB::Map::Map(Device^ device)
    : mDevice(device)
{
}

void HdB::Map::Draw()
{

}

/************
 * MapField *
 ************/

HdB::MapField::MapField(Device^ device, String^ texture)
    : mDevice(device)
{
    try {
        mTexture = Texture::FromFile(mDevice, TEXTURE_PATH + texture, Usage::None, Pool::Managed);
    } catch (Exception^ e) {
        mTexture = nullptr;
        Debug::WriteLine("ERROR: Could not load texture " + texture);
    }
    mGroundMesh = Mesh::CreateBox(mDevice, 10.f, 0.f, 10.f);
}

HdB::MapField::~MapField()
{
    delete mTexture;
    // TODO: Do we have to delete the Mesh? Plz check when ready!
}

void HdB::MapField::Draw()
{

}
