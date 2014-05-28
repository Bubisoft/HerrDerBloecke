#include "Model.h"
#include "Unit.h"
#include "Renderer.h"
#include "Globals.h"
#include "Map.h"

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

HdB::Model::Model(String^ name, Renderer^ renderer)
{
    Name = name;
    mRenderer = renderer;
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

void HdB::Model::RemoveInstance(Unit^ unit)
{
    mInstances->Remove(unit);
}

void HdB::Model::Draw(long long timeSinceLastFrame)
{
    Device^ dev = mRenderer->D3DDevice;
    for each (Submesh^ m in mMeshes) {
        dev->Material = m->material;
        dev->VertexFormat = Vertex::Format;
        dev->Indices = m->indices;
        dev->SetStreamSource(0, m->vertices, 0, Vertex::Size);
        dev->SetTexture(0, m->texture);

        for each (Unit^ u in mInstances) {
            if (u->GetType()->IsSubclassOf(Soldier::typeid) && (u->Position != u->MoveTo))
            {
                Soldier^ su = safe_cast<Soldier^>(u);
                float v = su->Speed();
                Vector3 mov = Vector3::Subtract(u->MoveTo, u->Position);
                float x = mov.Length();
                float xt = v * timeSinceLastFrame/10000000;
                if(xt < x)
                    mov *= (xt/x);

                if(mRenderer->Map->CanMove(u, mov))
                {
                    u->Position += mov;
                    u->LookAt += mov;
                }
                else
                {
                    Vector3 vec = Vector3::Cross(Vector3::UnitZ, mov);
                    vec.Normalize();
                    vec *= 3;
                    u->MoveTo += vec;
                    u->Position += vec;
                }
                if(su->IsInRange())
                    u->MoveTo = u->Position;

            }
            dev->SetTransform(TransformState::World, u->GetTransform());
            dev->DrawIndexedPrimitives(PrimitiveType::TriangleList, 0, 0,
                m->numVertices, 0, m->numFaces);
        }
    }
}

void HdB::Model::SetAlpha(float alpha)
{
    for each (Submesh^ m in mMeshes) {
        Color4 color = Color4(alpha, m->material.Diffuse.Red, m->material.Diffuse.Green, m->material.Diffuse.Blue);
        m->material.Diffuse = color;
    }
}

void HdB::Model::SetTeamColor(Color4 color)
{
    for each (Submesh^ m in mMeshes) {
        m->material.Diffuse = color;
        m->material.Specular = Color4(color.Red / 2.f, color.Green / 2.f, color.Blue / 2.f);
        m->material.Ambient = Color4(color.Red / 4.f, color.Green / 4.f, color.Blue / 4.f);
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
            mesh->vertices = gcnew VertexBuffer(mRenderer->D3DDevice, mesh->numVertices * Vertex::Size, Usage::WriteOnly,
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
            mesh->indices = gcnew IndexBuffer(mRenderer->D3DDevice, mesh->numFaces * 3 * sizeof(UInt32),
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
                Color4 diffuse = Color4(StrToFloat(parts[1]), StrToFloat(parts[2]), StrToFloat(parts[3]));
                line = reader->ReadLine(); // Specular
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                mesh->material.Specular = Color4(StrToFloat(parts[1]), StrToFloat(parts[2]), StrToFloat(parts[3]));
                line = reader->ReadLine(); // Transparency
                float alpha = 1.f - StrToFloat(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);
                mesh->material.Diffuse = Color4(alpha, diffuse.Red, diffuse.Green, diffuse.Blue);
                line = reader->ReadLine(); // TextureName
                String^ texture = TEXTURE_PATH + line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1];
                try {
                    mesh->texture = Texture::FromFile(mRenderer->D3DDevice, texture, Usage::None, Pool::Managed);
                } catch (Exception^ e) {
                    mesh->texture = nullptr;
                    Debug::WriteLine("ERROR: Could not load texture " + texture);
                }
            } else {
                line = reader->ReadLine(); // Color
                parts = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                mesh->material.Diffuse = Color4(StrToFloat(parts[1]), StrToFloat(parts[2]), StrToFloat(parts[3]));
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
