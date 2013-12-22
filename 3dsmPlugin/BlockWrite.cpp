#include "Blockporter.h"

void Blockporter::WriteHeader()
{
	const TCHAR* grpname = root->GetName();

	_ftprintf(fStream, _T("<Header>\n"));
	_ftprintf(fStream, _T("<Modelname=%s>\n"), grpname);
	_ftprintf(fStream, _T("<Version=%i>\n"), 1);
	_ftprintf(fStream, _T("</Header>\n"));
}
