#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OrbitCalcs.h"

int main(int argc, char **argv)
{
	if (argc < 5)
	{
		printf("usage: orbitcalcs lat long date targetdir\n");
		printf("lat and long as decimals(west negative)\n");
		printf("date as yyyymmdd\n\n");
		printf("eg OrbitCalcsSL 51.88 -1.31 20180411 123456\n");
		//sleep(100);;
		return -1;
	}

	double lati = atof(argv[1]);
	double longi = atof(argv[2]);
	long dtval = atoi(argv[3]);

	int yy = (dtval / 10000);
	int mo = (dtval - yy * 10000) / 100;
	int dy = dtval - yy * 10000 - mo * 100;

	//int vis_or_tele = 1;//  (planetno < 4 ? 1: 2);
	double dd = days(yy, mo, dy, 0, 0, 0);

	strcpy(szPath, argv[5]);
	strcpy(szOutputPath, argv[4]);
	printf("input path %s\n output path %s\n", szPath, szOutputPath);


	maxloaded = LoadOrbitalElements(elements);
	CreateOutputFiles(lati, longi, dd + 36525);	
	return 0;
}
