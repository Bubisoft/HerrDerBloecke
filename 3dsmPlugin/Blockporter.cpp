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

const TCHAR* Blockporter::LongDesc()
{
	static TCHAR buf[512];
	if (hInstance)
		return LoadString(hInstance, IDS_LONGDESC, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}

const TCHAR* Blockporter::ShortDesc()
{
	static TCHAR buf[256];
	if (hInstance)
		return LoadString(hInstance, IDS_SHORTDESC, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}

const TCHAR* Blockporter::AuthorName()
{
	static TCHAR buf[64];
	if (hInstance)
		return LoadString(hInstance, IDS_AUTHORNAME, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}

int Blockporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL supressPrompts, DWORD options)
{
	//first we need the Interface pointer
	ip = i;

	//setup the file stream
	Interface14* iface = GetCOREInterface14();
	UINT code = iface->DefaultTextSaveCodePage(true);
	MaxSDK::Util::Path storageNamePath(name);
	storageNamePath.SaveBaseFile();
	switch (code & MaxSDK::Util::MaxStringDataEncoding::MSDE_CP_MASK)
	{
	case CP_UTF8:
		fStream = _tfopen(name, _T("wt, ccs=UFT-8"));
		break;
	case MaxSDK::Util::MaxStringDataEncoding::MSDE_CP_UTF16:
		fStream = _tfopen(name, _T("wt, ccs=UTF-16BE"));
		break;
	default:
		fStream = _tfopen(name, _T("wt"));
	}
	if(!fStream)
		return 0;

	//now we have our file stream, so let's get the root node
	root = ip->GetRootNode();

	//the node of our object should be a groupnode, which contains every object
	//we want to export
	for(int idx = 0; idx < root->NumberOfChildren(); idx++)
	{
		if(root->GetChildNode(idx)->IsGroupHead())
		{
			//we found our group
			//next step is to make the group node our new root, because every object
			//we want is part of this group

			root = root->GetChildNode(idx);
			break;
		}
	}

	//we have our object, so let's write the header
	WriteHeader();

	//we are done exporting, so close the stream
	fclose(fStream);

	return 1;
}
