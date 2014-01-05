#include "Model.h"
#include "Unit.h"

using namespace System::IO;

value struct HdB::Submesh
{
    VertexFormat vertexFormat;
    int numVertices;
    int numFaces;
    int numTVerts;
    int vertexSize;
    VertexBuffer^ vertices;
    IndexBuffer^ indices;
    Material material;
};

HdB::Model::Model(String^ name, Device^ device)
{
    Name = name;
    mDevice = device;
    mMeshes = gcnew List<Submesh>();
    mInstances = gcnew List<Unit^>();

    LoadFromHBMFile("models" + Path::DirectorySeparatorChar + name + ".HBM");
}

HdB::Model::~Model()
{
    mInstances->Clear();

    for each(Submesh% m in mMeshes) {
        delete m.vertices;
        delete m.indices;
    }
    mMeshes->Clear();
}

void HdB::Model::AddInstance(Unit^ unit)
{
    mInstances->Add(unit);
}

void HdB::Model::Draw()
{
    for each (Submesh% m in mMeshes) {
        mDevice->Material = m.material;
        mDevice->VertexFormat = m.vertexFormat;
        mDevice->Indices = m.indices;
        mDevice->SetStreamSource(0, m.vertices, 0, m.vertexSize);

        for each (Unit^ u in mInstances) {
            mDevice->SetTransform(TransformState::World, Matrix::Translation(u->Position));
            mDevice->DrawIndexedPrimitives(PrimitiveType::TriangleList, 0, 0,
                    m.numVertices, 0, m.numFaces);
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

        String^ line = reader->ReadLine(); // Header
        line = reader->ReadLine(); // ModelName in File
        line = reader->ReadLine(); // Version
        line = reader->ReadLine(); // ObjectCount
        int objects = Convert::ToInt32(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);
        line = reader->ReadLine(); // Close Header

        // Loop through the objects
        // Counting to 1 as count not yet in header
        for (int i = 0; i < objects; i++) {
            Submesh mesh;
            line = reader->ReadLine(); // ObjectID
            line = reader->ReadLine(); // Mesh
            line = reader->ReadLine(); // NumVertices
            mesh.vertexFormat = VertexFormat::PositionNormal;
            mesh.vertexSize = 2 * Vector3::SizeInBytes;
            mesh.numVertices = Convert::ToInt32(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);
            mesh.vertices = gcnew VertexBuffer(mDevice, mesh.numVertices * mesh.vertexSize, Usage::WriteOnly,
                    mesh.vertexFormat, Pool::Managed);

            line = reader->ReadLine(); // NumFaces
            mesh.numFaces = Convert::ToInt32(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);
            mesh.indices = gcnew IndexBuffer(mDevice, mesh.numFaces * 3 * sizeof(UInt16), Usage::WriteOnly,
                    Pool::Managed, true);

            line = reader->ReadLine(); // NumTVerts
            mesh.numTVerts = Convert::ToInt32(line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries)[1]);

            line = reader->ReadLine(); // Vertices
            DataStream^ vBuf = mesh.vertices->Lock(0, 0, LockFlags::None);
            for (int vert = 0; vert < mesh.numVertices; vert++) {
                line = reader->ReadLine();
                array<String^>^ vertices = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                vBuf->Write(Vector3(Convert::ToSingle(vertices[0]), Convert::ToSingle(vertices[1]),
                        Convert::ToSingle(vertices[2])));
                vBuf->Seek(Vector3::SizeInBytes, SeekOrigin::Current);
            }
            vBuf->Close();
            line = reader->ReadLine(); // Close Vertices

            line = reader->ReadLine(); // Faces
            DataStream^ iBuf = mesh.indices->Lock(0, 0, LockFlags::None);
            for (int face = 0; face < mesh.numFaces; face++) {
                line = reader->ReadLine();
                array<String^>^ faces = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                iBuf->Write(Convert::ToUInt16(faces[0]));
                iBuf->Write(Convert::ToUInt16(faces[1]));
                iBuf->Write(Convert::ToUInt16(faces[2]));
            }
            iBuf->Close();
            line = reader->ReadLine(); // Close Faces

            line = reader->ReadLine(); // Normals
            vBuf = mesh.vertices->Lock(0, 0, LockFlags::None);
            for (int norm = 0; norm < mesh.numVertices; norm++) {
                line = reader->ReadLine();
                array<String^>^ normals = line->Split(controlChars, StringSplitOptions::RemoveEmptyEntries);
                vBuf->Seek(Vector3::SizeInBytes, SeekOrigin::Current);
                vBuf->Write(Vector3(Convert::ToSingle(normals[0]), Convert::ToSingle(normals[1]),
                        Convert::ToSingle(normals[2])));
            }
            vBuf->Close();
            line = reader->ReadLine(); // Close Normals

            line = reader->ReadLine(); // UVWMap
            for (int tv = 0; tv < mesh.numTVerts; tv++) {
                line = reader->ReadLine();
            }
            line = reader->ReadLine(); // Close UVWMap
            line = reader->ReadLine(); // Close Mesh

            // Read material data
            // TODO: FALSE Materials
            line = reader->ReadLine(); // Material

            // HAAAAAAAAAAAAX
            mesh.material.Diffuse = Color4(.75f, .75f, .75f);
            line = reader->ReadLine();
            line = reader->ReadLine();
            line = reader->ReadLine();
            line = reader->ReadLine();
            line = reader->ReadLine();

            line = reader->ReadLine(); // Close Material
            line = reader->ReadLine(); // Close Object
            mMeshes->Add(mesh);
        }
        reader->Close();
    } catch (Exception^ e) { }
}
