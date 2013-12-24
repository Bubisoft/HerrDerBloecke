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
		return LoadString(hInstance, id, buf, 512) ? buf : nullptr;
	/* I know you absolutly hate hardcoded numbers, but this is only temporay. If I use sizeof(buf) I only get
	the first 7 characters and if I use sizeof(buf)/sizeof(TCHAR) I only get the first 3 characters. I can't use 
	_countof(buf), because it requires an array and not a pointer. So I use the hardcoded number to get it work 
	for now. I hope you know where the problem is...*/

	return nullptr;
}

const TCHAR* Blockporter::Ext(int n)
{
	switch(n)
	{
	case 0:
		return _T("hbm"); //HBM Herr der Blöcke Modell
	default:
		return _T("");
	}
}

int Blockporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL supressPrompts, DWORD options)
{
	INode* root;
	//caption and message for MessagesBoxes
	TCHAR msg[512];
	TCHAR cap[512];

	//setup the file stream
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

	//now we have our file stream, so let's get the root node
	root = i->GetRootNode();

	//the node of our object should be a groupnode, which contains every object
	//we want to export
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
		fclose(mStream);
		return 0;
	}
	//we have our object, so let's write the header
	WriteHeader(root->GetName());

	//now that we have the header written, let's iterate through the objects in the
	//group and export the meshes and lights

	INode* child;

	for(int idx = 0; idx < root->NumberOfChildren(); idx++)
	{
		child = root->GetChildNode(idx);
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
			WriteMeshData(child, idx);
			break;
		//case LIGHT_CLASS_ID:
		//	WriteLightData(child, idx);
		//	break;
		}
	}

	//we are done exporting, so close the stream
	fclose(mStream);

	MessageBox(nullptr, GetString(IDS_FINISH_MSG, msg), GetString(IDS_FINISH_CAP, cap), MB_OK | MB_ICONINFORMATION);

	return 1;
}
