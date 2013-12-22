#include <mesh.h>
#include "Blockporter.h"

void Blockporter::WriteHeader(const TCHAR* grpname)
{
	_ftprintf(mStream, _T("<Header>\n"));
	_ftprintf(mStream, _T("\t<Modelname=%s>\n"), grpname);
	_ftprintf(mStream, _T("\t<Version=%i>\n"), 1);
	_ftprintf(mStream, _T("</Header>\n"));
}

void Blockporter::WriteMeshData(INode* objNode, int id)
{
	TriObject* tri;
	Matrix3 tm = objNode->GetObjTMAfterWSM(0);
	//Now we have to get the Triobject for the node
	Object* obj = objNode->EvalWorldState(0).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		tri = (TriObject*)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID,0));
	else
		return;

    //now that we have th triobject write the start
	_ftprintf(mStream, _T("<ObjectID=%i>\n"), id);
	_ftprintf(mStream, _T("\t<Mesh>\n"));
	Mesh* mesh = &tri->GetMesh();
	mesh->buildNormals();
	_ftprintf(mStream, _T("\t\t<NumVertices=%i>\n"), mesh->getNumVerts());
	_ftprintf(mStream, _T("\t\t<NumFaces=%i>\n"), mesh->getNumFaces());

	//start the vertexlist and export the vertices
	_ftprintf(mStream, _T("\t\t<Vertices>\n"));
	for(int vert = 0; vert < mesh->getNumVerts(); vert++)
	{
		Point3 v = mesh->verts[vert] * tm;
		_ftprintf(mStream, _T("\t\t\t%f,%f,%f;\n"), v.x, v.y, v.z);
	}
	_ftprintf(mStream, _T("\t\t</Vertices>\n")); //end vertexlist

	//start the facelist and export the faces
	_ftprintf(mStream, _T("\t\t<Faces>\n"));
	//is the object negativly scaled? If so we have to reverse the faces to see the object
	BOOL negScale = (DotProd(CrossProd(tm.GetRow(0),tm.GetRow(1)),tm.GetRow(2))<0.0) ? true : false;
	int v1,v3;
	if(negScale)
	{
		v1 = 2;
		v3 = 0;
	}
	else
	{
		v1 = 0;
		v3 = 2;
	}

	for(int face = 0; face < mesh->getNumFaces(); face++)
	{
		int vert1, vert2, vert3;
		vert1 = mesh->faces[face].v[v1];
		vert2 = mesh->faces[face].v[1];
		vert3 = mesh->faces[face].v[v3];
		_ftprintf(mStream, _T("\t\t\t%i,%i,%i;\n"), vert1, vert2, vert3);
	}
	_ftprintf(mStream, _T("\t\t</Faces>\n"));

	//start the normallist and export the normals
	_ftprintf(mStream, _T("\t\t<Normals>\n"));
	for(int norm = 0; norm < mesh->getNumFaces(); norm++)
	{
		Point3 n = mesh->getFaceNormal(norm);
		_ftprintf(mStream, _T("\t\t\t%f,%f,%f;\n"), n.x, n.y, n.z);
	}
	_ftprintf(mStream, _T("\t\t</Normals>\n")); //close the normallist
	_ftprintf(mStream, _T("\t</Mesh>\n")); //we are done with the Mesh so close it.
}
