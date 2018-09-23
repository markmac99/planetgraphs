#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include "shlwapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OrbitCalcs.h"

#pragma comment(lib, "shlwapi.lib")

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		printf("usage: OrbitCalcsCL lat long date\n");
		printf("lat and long as decimals(west negative)\n");
		printf("date as yyyymmdd\n\n");
		printf("eg OrbitCalcsSL 51.88 -1.31 20180411\n");
		//sleep(100);;
		return -1;
	}

	double lati = atof(argv[1]);
	double longi = atof(argv[2]);
	long dtval = atoi(argv[3]);

	int yy = (dtval / 10000);
	int mo = (dtval - yy * 10000) / 100;
	int dy = dtval - yy * 10000 - mo * 100;

	int vis_or_tele = 1;//  (planetno < 4 ? 1: 2);
	double dd = days(yy, mo, dy, 0, 0, 0);
#ifndef _WIN32
	strcpy(szPath, "/var/www/html/astro/mpcdata");
	strcpy(szOutputPath, "/var/www/html/planetgraphs/data");
	strccpy(szErrorPath, "/tmp");
#else
	char fname[512];
	strcpy(fname, argv[0]);
	PathRemoveFileSpecA(fname);
	strcat(fname, "\\OrbitCalcs.ini");
	GetPrivateProfileStringA("OrbitalElements", "Asteroids", "c:/temp", szPath, 512, fname);
	GetPrivateProfileStringA("Output", "DataFiles", "c:/temp", szOutputPath, 512, fname);
	GetPrivateProfileStringA("Output", "Errors", "c:/temp", szOutputPath, 512, fname);
#endif

	maxloaded = LoadOrbitalElements();
	if(maxloaded > 0)
		CreateOutputFiles(lati, longi, dd + 36525);	
	free(elements);
	return 0;
}
