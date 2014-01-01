#include "Model.h"
#include "Unit.h"

HdB::Model::Model(String^ name, Mesh^ mesh)
    : mMesh(mesh)
{
    Name = name;
    mInstances = gcnew List<Unit^>();

    // Temporary HACK
    mMaterial.Diffuse = Color4(.75f, .75f, .75f);
}

HdB::Model::~Model()
{
    mInstances->Clear();
}

void HdB::Model::AddInstance(Unit^ unit)
{
    mInstances->Add(unit);
}

void HdB::Model::Draw(Device^ device)
{
    device->Material = mMaterial;
    device->VertexFormat = mMesh->VertexFormat;
    device->Indices = mMesh->IndexBuffer;
    device->SetStreamSource(0, mMesh->VertexBuffer, 0, mMesh->BytesPerVertex);

    for each (Unit^ u in mInstances) {
        device->SetTransform(TransformState::World, Matrix::Translation(u->Position));
        device->DrawIndexedPrimitives(PrimitiveType::TriangleList, 0, 0,
                mMesh->VertexCount, 0, mMesh->FaceCount);
    }
}
