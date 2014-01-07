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

    //now that we have the triobject write the start
	_ftprintf(mStream, _T("\t<Mesh>\n"));
	Mesh* mesh = &tri->GetMesh();
	mesh->buildNormals();
	_ftprintf(mStream, _T("\t\t<NumVertices=%i>\n"), mesh->getNumTVerts());
	_ftprintf(mStream, _T("\t\t<NumFaces=%i>\n"), mesh->getNumFaces());
	_ftprintf(mStream, _T("\t\t<Vertices>\n"));

	float* vBuf = new float[mesh->getNumTVerts()*sizeof(float)*8];
	int* iBuf = new int[mesh->getNumFaces()*sizeof(int)*3];

	Point3* normals = new Point3[mesh->getNumVerts()];
	BuildVertexNormals(normals, mesh);

	for(int i = 0; i < mesh->getNumFaces(); i++)
	{
		TVFace t = mesh->tvFace[i];
		Face f = mesh->faces[i];

		int fid, tid;
		for(int j = 0; j < 3; j++)
		{
			fid = f.v[j];
			tid = t.t[j];

			Point3 pos = mesh->verts[fid] * tm;
			//position
			vBuf[tid*8+0] = pos.x;
			vBuf[tid*8+1] = pos.y;
			vBuf[tid*8+2] = pos.z;
			//normals
			vBuf[tid*8+3] = normals[fid].x;
			vBuf[tid*8+4] = normals[fid].y;
			vBuf[tid*8+5] = normals[fid].z;
			//UV
			vBuf[tid*8+6] = mesh->tVerts[tid].x;
			vBuf[tid*8+7] = 1 - mesh->tVerts[tid].y;

			iBuf[i*3+j] = tid;
		}
	}
	//write the buffers into the file
	for(int i = 0; i < mesh->getNumTVerts(); i++)
		_ftprintf(mStream, _T("\t\t\t%f,%f,%f;%f,%f,%f;%f,%f;\n"),vBuf[i*8+0],vBuf[i*8+1],vBuf[i*8+2],vBuf[i*8+3],vBuf[i*8+4],vBuf[i*8+5],vBuf[i*8+6],vBuf[i*8+7]);
	_ftprintf(mStream, _T("\t\t</Vertices>\n"));
	_ftprintf(mStream, _T("\t\t<Faces>\n"));
	for(int i = 0; i < mesh->getNumFaces(); i++)
		_ftprintf(mStream, _T("\t\t\t%i,%i,%i;\n"),iBuf[i*3+0],iBuf[i*3+1],iBuf[i*3+2]);
	_ftprintf(mStream, _T("\t\t</Faces>\n"));
	_ftprintf(mStream, _T("\t</Mesh>\n"));

	delete[] vBuf;
	delete[] iBuf;
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
