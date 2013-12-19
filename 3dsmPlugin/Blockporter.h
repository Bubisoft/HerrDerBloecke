#pragma once
#include <max.h>
#include <iparamb2.h>
#include "resource.h"

//if you change the class name you also have to change it in the ClassDesc below
class Blockporter : public SceneExport
{
public:
	Blockporter() { };
	~Blockporter() { };

	/*we currently only need to export into one format
	Maybe we have to add another format for animations a bit later...*/
	int ExtCount() { return 1; }
	const TCHAR* Ext(int n);

	//Read the following 3 from Stringtable (TODO)
	const TCHAR* LongDesc();
	const TCHAR* ShortDesc();
	const TCHAR* AuthorName();
	const TCHAR* CopyrightMessage() { return _T(""); }
	const TCHAR* OtherMessage1() { return _T(""); }
	const TCHAR* OtherMessage2() { return _T(""); }
	unsigned int Version() { return 1; }
	void ShowAbout(HWND hWnd) { }

	//the actual export function
	/* TODO:
	-write the export function
	*/

	int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL supressPrompts=FALSE, DWORD options=0) { return 1; }
};

//ClassDesc
class BlockporterDesc : public ClassDesc2
{
public:
	int IsPublic() { return 1; }
	void* Create(BOOL loading=FALSE) { return new Blockporter; }
	const TCHAR* ClassName() { return _T("Blockporter"); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID ClassID() { return Class_ID(0xfb73d79, 0xb511c5c); }
	const TCHAR* Category() { return _T("Scene Exporter"); }
};

static BlockporterDesc ClassDescInst;
