#include "Map.h"
#include "Globals.h"
#include "Unit.h"
#include "Model.h"

using namespace System::IO;
using namespace System::Diagnostics;

#define FIELDS_X 255
#define FIELDS_Y 255
#define FIELD_WIDTH 1.f
#define FIELD_HEIGHT 1.f

/*****************
 * MapOccupation *
 *****************/

HdB::MapOccupation::MapOccupation(HdB::Unit^ unit)
{
    mUnit = unit;
    mUnit->PositionChanged += gcnew PositionEvent(this, &MapOccupation::Update);
    Update(unit->Position);
}

bool HdB::MapOccupation::Contains(Point pos)
{
    if (pos.X >= mMinField.X && pos.Y >= mMinField.Y && pos.X <= mMaxField.X && pos.Y <= mMaxField.Y)
        return true;
    return false;
}

void HdB::MapOccupation::Update(const Vector3% pos)
{
    mMinField = Map::GetFieldCoordinate(pos + mUnit->Model->Bounds.Minimum);
    mMaxField = Map::GetFieldCoordinate(pos + mUnit->Model->Bounds.Maximum);
}

/*******
 * Map *
 *******/

HdB::Map::Map(Device^ device)
: mDevice(device)
{
    mOccupations = gcnew List<MapOccupation^>();

    String^ texture = "Grass.png";
    try {
        mTexture = Texture::FromFile(mDevice, TEXTURE_PATH + texture, Usage::None, Pool::Managed);
    } catch (Exception^ e) {
        mTexture = nullptr;
        Debug::WriteLine("ERROR: Could not load texture " + texture);
    }

    Mesh^ temp = Mesh::CreateBox(mDevice, FIELDS_X * FIELD_WIDTH, FIELDS_Y * FIELD_HEIGHT, 0.f);
    mGroundMesh = temp->Clone(mDevice, temp->CreationOptions, VertexFormat::Position | VertexFormat::Texture1);
    delete temp;
    DataStream^ dataStream = mGroundMesh->LockVertexBuffer(LockFlags::None);
    array<Vector3>^ vectors = D3DX::GetVectors(dataStream, mGroundMesh->VertexCount, mGroundMesh->VertexFormat);
    BoundingBox box = BoundingBox::FromPoints(vectors);
    dataStream->Seek(0, SeekOrigin::Begin);
    for each (Vector3% v in vectors) {
        dataStream->Position += Vector3::SizeInBytes;
        dataStream->Write(v.X == box.Maximum.X ? FIELDS_X / 16.f : 0.f);
        dataStream->Write(v.Y == box.Maximum.Y ? 0.f : FIELDS_Y / 16.f);
    }
    dataStream->Close();

    // Default material
    mMaterial.Diffuse = Color4(.6f, .6f, .6f);
    mMaterial.Ambient = Color4(.5f, .5f, .5f);
    mMaterial.Specular = Color4(.9f, .9f, .9f);
}

HdB::Map::~Map()
{
    delete mGroundMesh;
    delete mTexture;
    mOccupations->Clear();
}

void HdB::Map::Draw()
{
    mDevice->Material = mMaterial;
    mDevice->SetTexture(0, mTexture);
        
    mDevice->SetTransform(TransformState::World, Matrix::Translation(Vector3::Zero));
    mGroundMesh->DrawSubset(0);
}

void HdB::Map::AddUnit(Unit^ unit)
{
    mOccupations->Add(gcnew MapOccupation(unit));
}

HdB::Unit^ HdB::Map::CheckOccupation(const Vector3% posOnGround)
{
    Point field = GetFieldCoordinate(posOnGround);
    for each (MapOccupation^ occ in mOccupations)
        if (occ->Contains(field))
            return occ->Unit;
    return nullptr;
}

Point HdB::Map::GetFieldCoordinate(const Vector3% posOnGround)
{
    int x = posOnGround.X > 0.f ? (int)(posOnGround.X + .5f) : (int)(posOnGround.X - .5f);
    int y = posOnGround.Y > 0.f ? (int)(posOnGround.Y + .5f) : (int)(posOnGround.Y - .5f);
    return Point(x, y);
}
