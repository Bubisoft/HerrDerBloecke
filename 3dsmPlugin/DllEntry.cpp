#include "Blockporter.h"

HINSTANCE hInstance = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hInstance);
	}

	return(TRUE);
}

__declspec(dllexport) const TCHAR* LibDescription()
{
	return Blockporter::GetString(IDS_SHORTDESC);
}

__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	switch(i)
	{
	case 0:
		return &ClassDescInst;
	default:
		return 0;
	}
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

__declspec(dllexport) ULONG CanAutoDefer()
{
	return 1;
}
