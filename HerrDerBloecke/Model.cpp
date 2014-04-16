#include "Model.h"
#include "Unit.h"
#include "Globals.h"

using namespace System::IO;
using namespace System::Diagnostics;
using namespace System::Globalization;

#define StrToFloat(x) Convert::ToSingle(x, CultureInfo::InvariantCulture)
#define StrToInt(x) Convert::ToInt32(x, CultureInfo::InvariantCulture)
#define StrToUInt(x) Convert::ToUInt32(x, CultureInfo::InvariantCulture)

value struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 uv;

    static const int Size = 2 * Vector3::SizeInBytes + Vector2::SizeInBytes;
    static const VertexFormat Format = VertexFormat::Position | VertexFormat::Normal | VertexFormat::Texture1;

    void Write(DataStream^ stream)
    {
        stream->Write(position);
        stream->Write(normal);
        stream->Write(uv);
    }
};

ref struct HdB::Submesh
{
    int numVertices;
    int numFaces;
    VertexBuffer^ vertices;
    IndexBuffer^ indices;
    Texture^ texture;
    Material material;

    virtual ~Submesh()
    {
        delete texture;
        delete indices;
        delete vertices;
    }
};

HdB::Model::Model(String^ name, Device^ device)
{
    Name = name;
    mDevice = device;
    mMeshes = gcnew List<Submesh^>();
    mInstances = gcnew List<Unit^>();

    LoadFromHBMFile(MODEL_PATH + name + ".HBM");
}

HdB::Model::~Model()
{
    mInstances->Clear();

    for each(Submesh^ m in mMeshes)
        delete m;
    mMeshes->Clear();
}

void HdB::Model::AddInstance(Unit^ unit)
{
    mInstances->Add(unit);
}

void HdB::Model::Draw()
{
    for each (Submesh^ m in mMeshes) {
        mDevice->Material = m->material;
        mDevice->VertexFormat = Vertex::Format;
        mDevice->Indices = m->indices;
        mDevice->SetStreamSource(0, m->vertices, 0, Vertex::Size);
        mDevice->SetTexture(0, m->texture);

        for each (Unit^ u in mInstances) {
            mDevice->SetTransform(TransformState::World, u->GetTransform());
            mDevice->DrawIndexedPrimitives(PrimitiveType::TriangleList, 0, 0,
                m->numVertices, 0, m->numFaces);
        }
    }
}

void HdB::Model::LoadFromHBMFile(String^ filename)
{
    // This is not a real parser, we just fetch the information we need
    // from where it is supposed to be. Time is of the essence.
    try {
        StreamReader^ reader = File::OpenText(filename);

        // Constants for String trimming and splitting
        array<Char>^ controlChars = gcnew array<Char> {',', ';', '=', '\t', '<', '>'};
        array<String^>^ parts;

        String^ line = reader->ReadLine(); // Header
        line = reader->ReadLine(); // ModelName in File
        line = reader->ReadLine(); // Version
        line = reader->ReadLine(); // ObjectCount
        int objects = StrToInt(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);
        line = reader->ReadLine(); // Close Header

        // Loop through the objects
        for (int i = 0; i < objects; i++) {
            Submesh^ mesh = gcnew Submesh();
            line = reader->ReadLine(); // ObjectID
            line = reader->ReadLine(); // Mesh
            line = reader->ReadLine(); // NumVertices
            mesh->numVertices = StrToInt(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);

            line = reader->ReadLine(); // NumFaces
            mesh->numFaces = StrToInt(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);

            line = reader->ReadLine(); // Vertices
            mesh->vertices = gcnew VertexBuffer(mDevice, mesh->numVertices * Vertex::Size, Usage::WriteOnly,
                Vertex::Format, Pool::Managed);
            DataStream^ vBuf = mesh->vertices->Lock(0, 0, LockFlags::None);
            for (int vert = 0; vert < mesh->numVertices; vert++) {
                line = reader->ReadLine();
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                Vertex v;
                v.position = Vector3(StrToFloat(parts[0]), StrToFloat(parts[1]), StrToFloat(parts[2]));
                v.normal = Vector3(StrToFloat(parts[3]), StrToFloat(parts[4]), StrToFloat(parts[5]));
                v.uv = Vector2(StrToFloat(parts[6]), StrToFloat(parts[7]));
                v.Write(vBuf);
            }
            vBuf->Close();
            line = reader->ReadLine(); // Close Vertices

            line = reader->ReadLine(); // Faces
            mesh->indices = gcnew IndexBuffer(mDevice, mesh->numFaces * 3 * sizeof(UInt32),
                Usage::WriteOnly, Pool::Managed, false);
            DataStream^ iBuf = mesh->indices->Lock(0, 0, LockFlags::None);
            for (int face = 0; face < mesh->numFaces; face++) {
                line = reader->ReadLine();
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                iBuf->Write(StrToUInt(parts[0]));
                iBuf->Write(StrToUInt(parts[1]));
                iBuf->Write(StrToUInt(parts[2]));
            }
            iBuf->Close();
            line = reader->ReadLine(); // Close Faces
            line = reader->ReadLine(); // Close Mesh

            line = reader->ReadLine(); // Material
            if (line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1] == "TRUE") {
                line = reader->ReadLine(); // Ambient
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                mesh->material.Ambient = Color4(StrToFloat(parts[1]), StrToFloat(parts[2]), StrToFloat(parts[3]));
                line = reader->ReadLine(); // Diffuse
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                mesh->material.Diffuse = Color4(StrToFloat(parts[1]), StrToFloat(parts[2]), StrToFloat(parts[3]));
                line = reader->ReadLine(); // Specular
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                mesh->material.Specular = Color4(StrToFloat(parts[1]), StrToFloat(parts[2]), StrToFloat(parts[3]));
                line = reader->ReadLine(); // Transparency
                float alpha = 1.f - StrToFloat(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);
                mesh->material.Ambient.Alpha = alpha;
                mesh->material.Diffuse.Alpha = alpha;
                mesh->material.Specular.Alpha = alpha;
                line = reader->ReadLine(); // TextureName
                String^ texture = TEXTURE_PATH + line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1];
                try {
                    mesh->texture = Texture::FromFile(mDevice, texture, Usage::None, Pool::Managed);
                } catch (Exception^ e) {
                    mesh->texture = nullptr;
                    Debug::WriteLine("ERROR: Could not load texture " + texture);
                }
            } else {
                line = reader->ReadLine(); // Color
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                Color4 color = Color4(StrToFloat(parts[1]), StrToFloat(parts[2]), StrToFloat(parts[3]));
                mesh->material.Diffuse = color;
                mesh->material.Ambient = Color4(.5f, .5f, .5f); // Hardcoded default
                mesh->material.Specular = Color4(.9f, .9f, .9f); // Hardcoded default
            }
            line = reader->ReadLine(); // Close Material
            line = reader->ReadLine(); // Close Object
            mMeshes->Add(mesh);
        }

        line = reader->ReadLine(); // BoundingBox
        line = reader->ReadLine(); // Min
        parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
        Vector3 min(StrToFloat(parts[1]), StrToFloat(parts[2]), 0.f);
        mBounds.Minimum = min;
        line = reader->ReadLine(); // Max
        parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
        Vector3 max(StrToFloat(parts[1]), StrToFloat(parts[2]), 0.f);
        mBounds.Maximum = max;
        line = reader->ReadLine(); // Close BoundingBox

        reader->Close();
    } catch (Exception^ e) {
        Debug::WriteLine("ERROR: " + e->Message);
        Debug::WriteLine(e->StackTrace);
    }
}
