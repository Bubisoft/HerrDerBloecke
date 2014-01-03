#include <cmath>
#include "Blockporter.h"

const TCHAR* Blockporter::GetString(UINT id)
{
	static TCHAR buf[512];
	if (hInstance)
		return LoadString(hInstance, id, buf, _countof(buf)) ? buf : nullptr;

	return nullptr;
}

const TCHAR* Blockporter::GetString(UINT id, TCHAR* buf)
{
	if (hInstance)
		return LoadString(hInstance, id, buf, MB_BUFFER_LENGTH) ? buf : nullptr;

	return nullptr;
}

const TCHAR* Blockporter::Ext(int n)
{
	switch(n)
	{
	case ENDING_HBM:
		return _T("hbm"); //HBM Herr der Blöcke Modell
	default:
		return _T("");
	}
}

int Blockporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL supressPrompts, DWORD options)
{
	INode* root;
	//caption and message for MessagesBoxes
	TCHAR msg[MB_BUFFER_LENGTH];
	TCHAR cap[MB_BUFFER_LENGTH];

	//Get the root node
	root = i->GetRootNode();

	//the node of our object should be a groupnode, which contains every object
	//we want to export
	i->PushPrompt(_T("Searching for Group..."));
	bool found = false;
	for(int idx = 0; idx < root->NumberOfChildren(); idx++)
	{
		if(root->GetChildNode(idx)->IsGroupHead())
		{
			//we found our group
			//next step is to make the group node our new root, because every object
			//we want is part of this group

			found = true;
			root = root->GetChildNode(idx);
			break;
		}
	}

	if(!found)
	{
		MessageBox(nullptr, GetString(IDS_ERROR_NO_GROUP, msg), GetString(IDS_GENERAL_ERROR, cap), MB_OK | MB_ICONERROR);
		return 0;
	}

	//Now that we have the groupnode let's compare the fileversions
	if(!IsNewModelVersion(name, root->GetName()))
	{
		if(MessageBox(nullptr, GetString(IDS_VER_TO_LOW_MSG, msg), GetString(IDS_VER_TO_LOW_CAP, cap), MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
			return 1;
	}

	i->PushPrompt(_T("Opening File"));
	Interface14* iface = GetCOREInterface14();
	UINT code = iface->DefaultTextSaveCodePage(true);
	MaxSDK::Util::Path storageNamePath(name);
	storageNamePath.SaveBaseFile();
	switch (code & MaxSDK::Util::MaxStringDataEncoding::MSDE_CP_MASK)
	{
	case CP_UTF8:
		mStream = _tfopen(name, _T("wt, ccs=UFT-8"));
		break;
	case MaxSDK::Util::MaxStringDataEncoding::MSDE_CP_UTF16:
		mStream = _tfopen(name, _T("wt, ccs=UTF-16BE"));
		break;
	default:
		mStream = _tfopen(name, _T("wt"));
	}
	if(!mStream)
		return 0;

	//now we have our file stream, so let's write the header
	i->PushPrompt(_T("Writing Header"));
	WriteHeader(root->GetName(), root->NumberOfChildren());

	//now that we have the header written, let's iterate through the objects in the
	//group and export the meshes and lights

	INode* child;

	for(int idx = 0; idx < root->NumberOfChildren(); idx++)
	{
		child = root->GetChildNode(idx);
		i->PushPrompt(_T("Processing Object %s", child->GetName()));
		if(child->IsGroupHead())
		{
			MessageBox(nullptr, GetString(IDS_ERROR_TO_MANY_GROUPS, msg), GetString(IDS_GENERAL_ERROR, cap), MB_OK | MB_ICONERROR);
			continue;
		}

		ObjectState os = child->EvalWorldState(0);

		//let's take a look at the SuperClassID of the object
		//so we find out if it's a mesh or a light
		if(!os.obj)
			continue; //somehow this node doesn't have an object

		switch(os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			_ftprintf(mStream, _T("<ObjectID=%i>\n"), idx);
			i->PushPrompt(_T("Writing MeshData for Object %s", child->GetName()));
			WriteMeshData(child, idx);
			i->PushPrompt(_T("Writing MaterialData for Object %s", child->GetName()));
			WriteMaterialData(child);
			_ftprintf(mStream, _T("</Object>\n"));
			break;
		//case LIGHT_CLASS_ID:
		//	WriteLightData(child, idx);
		//	break;
		}
	}

	//we are done exporting, so close the stream
	i->PushPrompt(_T("Closing file..."));
	fclose(mStream);

	MessageBox(nullptr, GetString(IDS_FINISH_MSG, msg), GetString(IDS_FINISH_CAP, cap), MB_OK | MB_ICONINFORMATION);

	return 1;
}

bool Blockporter::IsNewModelVersion(const TCHAR* file, const TCHAR* nodeName)
{
	mStream = _tfopen(file, _T("r"));
	if(!mStream)
		return true; //the file doesn't exist, so the current modelversion is greater

	//Version is in the third line of the file, so let's get it
	TCHAR line[128];
	TCHAR model[64];
	wcscpy(model, nodeName);
	fgetws(line, _countof(line), mStream);
	//first check if this is´the proper file
	if(wcscmp(line, L"<Header>\n")) //The first line isn't <Header> so it's not the right file
	{
		fclose(mStream);
		return true;
	}

	fgetws(line, _countof(line), mStream);
	fgetws(line, _countof(line), mStream);

	TCHAR* oVer = wcstok(line, L"=");
	oVer = wcstok(nullptr, L"=");
	TCHAR* nVer = wcstok(model, L"=");
	nVer = wcstok(nullptr, L"=");

	if(_wtoi(oVer) <= _wtoi(nVer))
	{
		fclose(mStream);
		return true; //The old version is below the new one
	}

	fclose(mStream);
	return false; //The old version is above the new one. Better ask if we really want to export.
}

void Blockporter::BuildVertexNormals(Point3* normals, Mesh* m)
{
	for(int i = 0; i < m->getNumVerts(); i++)
		normals[i] = Point3(0,0,0);
	for(int norm = 0; norm < m->getNumFaces(); norm++)
	{
		Face f = m->faces[norm];
		normals[f.v[0]] += m->getFaceNormal(norm);
		normals[f.v[1]] += m->getFaceNormal(norm);
		normals[f.v[2]] += m->getFaceNormal(norm);
	}

	//normalize the normals
	for(int norm = 0; norm < m->getNumVerts(); norm++)
	{
		float l, x, y, z;
		x = normals[norm].x;
		y = normals[norm].y;
		z = normals[norm].z;

		l = sqrt(x*x+y*y+z*z);
		normals[norm] = Point3(x/l, y/l, z/l);
	}
}
