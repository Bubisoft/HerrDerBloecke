#pragma once
#include <cstring>
#include <max.h>
#include <iparamb2.h>
#include <maxtextfile.h>
#include "resource.h"

#define MB_BUFFER_LENGTH 512
#define ENDING_HBM 0

//if you change the class name you also have to change it in the ClassDesc below
class Blockporter : public SceneExport
{
public:
	Blockporter() { };
	~Blockporter() { };

	//use this function to get Strings required for the plugin (as ShortDesc, Authorname, ...)
	static const TCHAR* GetString(UINT id);
	//use this function to get strings for Messageboxes or if you need to get more than one string per function
	static const TCHAR* GetString(UINT id, TCHAR* buf);

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
	-export lights
	-export animation
	*/

	int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL supressPrompts=FALSE, DWORD options=0);

private:
	//helper functions
	bool IsNewModelVersion(const TCHAR* file, const TCHAR* nodeName);
	void BuildVertexNormals(Point3* normals, Mesh* m);
	//writer functions
	void WriteHeader(const TCHAR* nodeName, int objNumber);
	void WriteMeshData(INode* objNode, int id);
	void WriteMaterialData(INode* objNode);
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
