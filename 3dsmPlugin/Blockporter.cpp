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
