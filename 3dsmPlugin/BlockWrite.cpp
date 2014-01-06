#include <mesh.h>
#include <stdmat.h>
#include "Blockporter.h"

void Blockporter::WriteHeader(const TCHAR* nodeName, int objNumber)
{
	TCHAR grpname[64];
	wcscpy(grpname, nodeName);
	_ftprintf(mStream, _T("<Header>\n"));
	//split the groupname into the name and the version. Syntax vor the groupname is Name=Version
	TCHAR* sVer = wcstok(grpname, L"=");
	_ftprintf(mStream, _T("\t<Modelname=%s>\n"), sVer);
	//we assume there is only one "=" in the groupname, so we are going right to the second part
	//of the string
	sVer = wcstok(nullptr, L"=");
	int ver = _wtoi(sVer);

	_ftprintf(mStream, _T("\t<Version=%i>\n"), ver);
	_ftprintf(mStream, _T("\t<ObjectCount=%i>\n"), objNumber);
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
	_ftprintf(mStream, _T("\t<Mesh>\n"));
	Mesh* mesh = &tri->GetMesh();
	mesh->buildNormals();
	_ftprintf(mStream, _T("\t\t<NumVertices=%i>\n"), mesh->getNumVerts());
	_ftprintf(mStream, _T("\t\t<NumFaces=%i>\n"), mesh->getNumFaces());
	_ftprintf(mStream, _T("\t\t<NumTVerts=%i>\n"), mesh->getNumTVerts()); //UVs

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

	//first build the vertex normals
	Point3* normals = new Point3[mesh->getNumVerts()];
	BuildVertexNormals(normals, mesh);

	for(int norm = 0; norm < mesh->getNumVerts(); norm++)
	{
		Point3 n = normals[norm];
		_ftprintf(mStream, _T("\t\t\t%f,%f,%f;\n"), n.x, n.y, n.z);
	}
	_ftprintf(mStream, _T("\t\t</Normals>\n")); //close the normallist

	delete[] normals;

	//start the UV List
	_ftprintf(mStream, _T("\t\t<UVWMap>\n"));
	std::set<TVert> tVerts = GetTVerts(mesh);
	for (const TVert& tv : tVerts)
		_ftprintf(mStream, _T("\t\t\t%i,%f,%f,%f;\n"), tv.idx, tv.u, tv.v, tv.w);
	_ftprintf(mStream, _T("\t\t</UVWMap>\n")); 	//end UV List

	_ftprintf(mStream, _T("\t</Mesh>\n")); //we are done with the Mesh so close it.
}

void Blockporter::WriteMaterialData(INode* objNode)
{
	Mtl* mtl = objNode->GetMtl();
	if(!mtl || mtl->ClassID() != Class_ID(DMTL_CLASS_ID, 0))
	{
		_ftprintf(mStream, _T("\t<Material=FALSE>\n")); //We haven't defined a Material, so we just export the wire color
		DWORD c = objNode->GetWireColor();
		_ftprintf(mStream, _T("\t\t<Wirecolor=%f,%f,%f>\n"), GetRValue(c) / 255.0f, GetGValue(c) / 255.0f, GetBValue(c) / 255.0f);
	}
	else
	{
		_ftprintf(mStream, _T("\t<Material=TRUE>\n"));
		StdMat* stdmat = (StdMat*)mtl;
		Color c;
		c = stdmat->GetAmbient(0);
		_ftprintf(mStream, _T("\t\t<Ambient=%f,%f,%f>\n"), c.r, c.g, c.b);
		c = stdmat->GetDiffuse(0);
		_ftprintf(mStream, _T("\t\t<Diffuse=%f,%f,%f>\n"), c.r, c.g, c.b);
		c = stdmat->GetSpecular(0);
		_ftprintf(mStream, _T("\t\t<Specular=%f,%f,%f>\n"), c.r, c.g, c.b);
		_ftprintf(mStream, _T("\t\t<Transparency=%f>\n"), stdmat->GetXParency(0));

		for (int i = 0; i < mtl->NumSubTexmaps(); i++)
		{
			Texmap* tex = mtl->GetSubTexmap(i);
			if (!tex || !stdmat->MapEnabled(i) || tex->ClassID() != Class_ID(BMTEX_CLASS_ID, 0))
				continue;

			TCHAR mapName[128];
			TCHAR mapExt[8];
			_tsplitpath(((BitmapTex *)tex)->GetMapName(), nullptr, nullptr, mapName, mapExt);
			_ftprintf(mStream, _T("\t\t<TextureName=%s%s>\n"), mapName, mapExt);
		}
	}
	_ftprintf(mStream, _T("\t</Material>\n"));
}
