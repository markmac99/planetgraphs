#include <Windows.h>
#include "OrbitCalcs.h"

struct OrbitalElements elements[10];

extern "C" BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) 
	{
		case DLL_PROCESS_ATTACH:
			LoadOrbitalElements(elements);
			break;
	}	
	return TRUE;
}

double __stdcall MeanAnomaly(int planetno , double dd ) 
{
	double k = elements[planetno].MA[0];
	double p = elements[planetno].MA[1];

	double ma = (k + p * dd) / RAD2DEG;
	while (ma > TWOPI)
		ma = ma - TWOPI;
	while (ma < 0)
		ma = ma + TWOPI;
	return ma ;
}