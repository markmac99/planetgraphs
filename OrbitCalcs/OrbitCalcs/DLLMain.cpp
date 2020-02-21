#define  _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include "shlwapi.h"
#include "OrbitCalcs.h"
#include <ole2.h> // OLE2 Definitions
#include <stdio.h>


extern "C" BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		GetModuleFileNameA((HMODULE)hinstDLL, szPath, sizeof szPath);
		PathRemoveFileSpecA(szPath);
		maxloaded = LoadOrbitalElements(elements);

		break;
	}
	return TRUE;
}

SAFEARRAY* __stdcall GetObjectNames(int *numloaded)
{
	// Create the SAFEARRAY bounds.
	SAFEARRAYBOUND bounds[2];            // 2-D array
	bounds[0].lLbound = 0;
	bounds[0].cElements = 1;             
	bounds[1].lLbound = 0;
	bounds[1].cElements = maxloaded;
	SAFEARRAY *retArray;
	retArray = SafeArrayCreate(VT_BSTR, 2, bounds);

	FILE *f = fopen("c:/temp/foo.txt", "w");
	long indices[2];
	for (int i = 0; i < maxloaded; i++)
	{
		indices[0] = 0;
		indices[1] = i;
		char *name = elements[i].name;
		HRESULT hResult = SafeArrayPutElement(retArray, indices, SysAllocStringByteLen(name, (UINT)strlen(name)));
		BSTR tmpBstr;
		hResult = SafeArrayGetElement(retArray, indices, &tmpBstr);
		char *c = (LPSTR)tmpBstr;

		fprintf(f, "%d %s %s\n", i, name, c);
	}
	fclose(f);
	*numloaded = maxloaded;
	return retArray;
}

