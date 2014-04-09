#include "Map.h"
#include "Globals.h"

using namespace System::IO;
using namespace System::Diagnostics;

#define FIELDS_X 1280 // Only for one side ==> total is double + 1 (center)
#define FIELDS_Y 1280 // Only for one side ==> total is double + 1 (center)
#define FIELD_WIDTH 1.f
#define FIELD_HEIGHT 1.f

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

    Mesh^ temp = Mesh::CreateBox(mDevice, FIELD_WIDTH, FIELD_HEIGHT, 0.f);
    mGroundMesh = temp->Clone(mDevice, temp->CreationOptions, VertexFormat::Position | VertexFormat::Texture1);
    delete temp;
    DataStream^ dataStream = mGroundMesh->LockVertexBuffer(LockFlags::None);
    array<Vector3>^ vectors = D3DX::GetVectors(dataStream, mGroundMesh->VertexCount, mGroundMesh->VertexFormat);
    BoundingBox box = BoundingBox::FromPoints(vectors);
    dataStream->Seek(0, SeekOrigin::Begin);
    for each (Vector3% v in vectors) {
        dataStream->Position += Vector3::SizeInBytes;
        dataStream->Write(v.X == box.Maximum.Y ? 1.f : 0.f);
        dataStream->Write(v.Y == box.Maximum.Y ? 0.f : 1.f);
    }
    dataStream->Close();

    mInstances = gcnew List<Vector3>();

    // Default material
    mMaterial.Diffuse = Color4(.6f, .6f, .6f);
    mMaterial.Ambient = Color4(.5f, .5f, .5f);
    mMaterial.Specular = Color4(.9f, .9f, .9f);
}

HdB::MapField::~MapField()
{
    mInstances->Clear();
    delete mGroundMesh;
    delete mTexture;
}

void HdB::MapField::Draw()
{
    mDevice->Material = mMaterial;
    mDevice->SetTexture(0, mTexture);

    for each (Vector3% field in mInstances) {
        mDevice->SetTransform(TransformState::World, Matrix::Translation(field));
        mGroundMesh->DrawSubset(0);
    }
}

void HdB::MapField::AddField(int x, int y)
{
    Vector3 pos(x * FIELD_WIDTH, y * FIELD_HEIGHT, 0.f);
    mInstances->Add(pos);
}

/*******
* Map *
*******/

HdB::Map::Map(Device^ device)
: mDevice(device)
{
    mGrass = gcnew MapField(mDevice, "Grass.png");
    for (int i = -FIELDS_X; i <= FIELDS_X; i++) {
        for (int j = -FIELDS_Y; j <= FIELDS_Y; j++) {
            mGrass->AddField(i, j);
        }
    }
}

HdB::Map::~Map()
{
    delete mGrass;
}

void HdB::Map::Draw()
{
    mGrass->Draw();
}
