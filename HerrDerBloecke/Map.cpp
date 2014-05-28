#include "Map.h"
#include "Renderer.h"
#include "Globals.h"
#include "Unit.h"
#include "Model.h"

using namespace System::IO;
using namespace System::Diagnostics;

#define FIELDS_X 2049
#define FIELDS_Y 2049
#define FIELD_WIDTH 1.f
#define FIELD_HEIGHT 1.f
#define DIFFERENT_FIELDS 3
/*****************
 * MapOccupation *
 *****************/

HdB::MapOccupation::MapOccupation(HdB::Unit^ unit)
{
    mUnit = unit;
    mUnit->PositionChanged += gcnew PositionEvent(this, &MapOccupation::Update);
    Update(unit->Position);
}

void HdB::MapOccupation::Update(const Vector3% pos)
{
    Point mMinField = Map::GetFieldCoordinate(pos + mUnit->Model->Bounds.Minimum);
    Point mMaxField = Map::GetFieldCoordinate(pos + mUnit->Model->Bounds.Maximum);
    mMinField.X--; mMinField.Y--;
    mMaxField.X++; mMaxField.Y++;
    mArea = Rectangle(mMinField.X, mMinField.Y, mMaxField.X - mMinField.X, mMaxField.Y - mMinField.Y);
}

/*******
 * Map *
 *******/

HdB::Map::Map(Renderer^ renderer) : mRenderer(renderer)
{
    mOccupations = gcnew List<MapOccupation^>();

    String^ texture = "Grass.png";
    try {
        mTexture = Texture::FromFile(mRenderer->D3DDevice, TEXTURE_PATH + texture, Usage::None, Pool::Managed);
    } catch (Exception^ e) {
        mTexture = nullptr;
        Debug::WriteLine("ERROR: Could not load texture " + texture);
    }

    Mesh^ temp = Mesh::CreateBox(mRenderer->D3DDevice, FIELDS_X * FIELD_WIDTH, FIELDS_Y * FIELD_HEIGHT, 0.f);
    mGroundMesh = temp->Clone(mRenderer->D3DDevice, temp->CreationOptions, VertexFormat::Position | VertexFormat::Texture1);
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

void HdB::Map::Draw(long long timeSinceLastFrame)
{
    mRenderer->D3DDevice->Material = mMaterial;
    mRenderer->D3DDevice->SetTexture(0, mTexture);
        
    mRenderer->D3DDevice->SetTransform(TransformState::World, Matrix::Translation(Vector3::Zero));
    mGroundMesh->DrawSubset(0);
}

void HdB::Map::AddUnit(Unit^ unit)
{
    mOccupations->Add(gcnew MapOccupation(unit));
}

void HdB::Map::RemoveUnit(Unit^ unit)
{
    for each (MapOccupation^ occ in mOccupations) {
        if (occ->Unit == unit) {
            mOccupations->Remove(occ);
            break;
        }
    }
}

HdB::Unit^ HdB::Map::CheckOccupation(const Vector3% posOnGround)
{
    Point field = GetFieldCoordinate(posOnGround);
    for each (MapOccupation^ occ in mOccupations)
        if (occ->Area.Contains(field))
            return occ->Unit;
    return nullptr;
}

List<HdB::Unit^>^ HdB::Map::CheckOccupation(const Vector3% a, const Vector3% b)
{
    Point minField = GetFieldCoordinate(Vector3(Math::Min(a.X, b.X), Math::Min(a.Y, b.Y), 0.f));
    Point maxField = GetFieldCoordinate(Vector3(Math::Max(a.X, b.X), Math::Max(a.Y, b.Y), 0.f));
    Rectangle area(minField.X, minField.Y, maxField.X - minField.X, maxField.Y - minField.Y);
    List<HdB::Unit^>^ units = gcnew List<HdB::Unit^>();
    for each (MapOccupation^ occ in mOccupations)
    if (occ->Area.IntersectsWith(area))
        units->Add(occ->Unit);
    return units;
}

List<HdB::Unit^>^ HdB::Map::CheckOccupation(const Vector3% a, const Vector3% b, const Vector3% c, const Vector3% d)
{
    List<HdB::Unit^>^ units = gcnew List<HdB::Unit^>();
    Point field1 = GetFieldCoordinate(a);
    Point field2 = GetFieldCoordinate(b);
    Point field3 = GetFieldCoordinate(c);
    Point field4 = GetFieldCoordinate(d);
    System::Drawing::Drawing2D::GraphicsPath^ path = gcnew System::Drawing::Drawing2D::GraphicsPath();
    path->AddPolygon(gcnew array<Point> {field1, field2, field3, field4});
    if (path->GetBounds().IsEmpty) {
        Unit^ u = CheckOccupation(a);
        if (u)
            units->Add(u);
        return units;
    }
    Region^ area = gcnew Region(path);
    for each (MapOccupation^ occ in mOccupations)
        if (area->IsVisible(occ->Area))
            units->Add(occ->Unit);
    return units;
}

bool HdB::Map::CanBuild(Unit^ unit)
{
    Vector3 min = unit->Position + unit->Model->Bounds.Minimum;
    Vector3 max = unit->Position + unit->Model->Bounds.Maximum;
    if (CheckOccupation(min, max)->Count > 0)
        return false;
    if (!OnMap(min) || !OnMap(max))
        return false;
    return true;
}

bool HdB::Map::CanMove(Unit^ unit, const Vector3% direction)
{
    Unit^ occupant;
    Vector3 n = Vector3::Normalize(direction);

    // Respect bounds, look direction is negative Y of the bounding box
    Vector3 target = unit->Position + unit->Model->Bounds.Minimum.Y * n;

    do {
        target += n;
        occupant = CheckOccupation(target);
    } while (occupant == unit);

    if (occupant || !OnMap(target))
        return false;
    return true;
}

bool HdB::Map::OnMap(const Vector3% pos)
{
    Point field = GetFieldCoordinate(pos);
    if (field.X < (int)(-FIELDS_X / 2.f))
        return false;
    if (field.X > (int)(FIELDS_X / 2.f))
        return false;
    if (field.Y < (int)(-FIELDS_Y / 2.f))
        return false;
    if (field.Y > (int)(FIELDS_Y / 2.f))
        return false;
    return true;
}

Point HdB::Map::GetFieldCoordinate(const Vector3% posOnGround)
{
    int x = posOnGround.X > 0.f ? (int)(posOnGround.X + .5f) : (int)(posOnGround.X - .5f);
    int y = posOnGround.Y > 0.f ? (int)(posOnGround.Y + .5f) : (int)(posOnGround.Y - .5f);
    return Point(x, y);
}
