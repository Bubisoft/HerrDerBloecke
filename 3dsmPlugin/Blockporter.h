#pragma once
#include <max.h>
#include <iparamb2.h>
#include <maxtextfile.h>
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

	const TCHAR* LongDesc() { return GetString(IDS_LONGDESC); }
	const TCHAR* ShortDesc() { return GetString(IDS_SHORTDESC); }
	const TCHAR* AuthorName() { return GetString(IDS_AUTHORNAME); }
	const TCHAR* CopyrightMessage() { return GetString(IDS_COPYRIGHT); }
	const TCHAR* OtherMessage1() { return GetString(IDS_OTHERMESSAGE_1); }
	const TCHAR* OtherMessage2() { return GetString(IDS_OTHERMESSAGE_2); }
	unsigned int Version() { return 1; }
	void ShowAbout(HWND hWnd) { }

	//the actual export function
	/* TODO:
	-write the export function
	-parse a version from the groupname
	-export lights
	*/

	int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL supressPrompts=FALSE, DWORD options=0);

private:
	const TCHAR* GetString(UINT id);
	void WriteHeader(const TCHAR* grpname);
	void WriteMeshData(INode* objNode, int id);
	//void WriteLightData(INode* objNode, int id);

	FILE* mStream;
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
