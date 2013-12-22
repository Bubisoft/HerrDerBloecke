#include "Blockporter.h"

const TCHAR* Blockporter::Ext(int n)
{
	switch(n)
	{
	case 0:
		return _T("HBM"); //HBM Herr der Blöcke Modell
	default:
		return _T("");
	}
}

const TCHAR* Blockporter::GetString(UINT id)
{
	static TCHAR buf[512];
	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}

int Blockporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL supressPrompts, DWORD options)
{
	//first we need the Interface pointer
	Interface* mInterface = i;
	INode* mRoot;

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
	mRoot = mInterface->GetRootNode();

	//the node of our object should be a groupnode, which contains every object
	//we want to export
	bool found = false;
	for(int idx = 0; idx < mRoot->NumberOfChildren(); idx++)
	{
		if(mRoot->GetChildNode(idx)->IsGroupHead())
		{
			//we found our group
			//next step is to make the group node our new root, because every object
			//we want is part of this group

			found = true;
			mRoot = mRoot->GetChildNode(idx);
			break;
		}
	}

	if(!found)
	{
		MessageBox(NULL, GetString(IDS_ERROR_NO_GROUP), GetString(IDS_GENERAL_ERROR), MB_OK);
		fclose(mStream);
		return 0;
	}
	//we have our object, so let's write the header
	WriteHeader(mRoot->GetName());

	//now that we have the header written, let's iterate through the objects in the
	//group and export the meshes and lights

	INode* child;

	for(int idx = 0; idx < mRoot->NumberOfChildren(); idx++)
	{
		child = mRoot->GetChildNode(idx);
		if(child->IsGroupHead())
		{
			MessageBox(NULL, GetString(IDS_ERROR_TO_MANY_GROUPS), GetString(IDS_GENERAL_ERROR), MB_OK);
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

	return 1;
}
