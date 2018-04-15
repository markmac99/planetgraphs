#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include "../OrbitCalcs/OrbitCalcs.h"

int main(int argc, char **argv)
{
	if (argc < 5)
	{
		printf("usage: OrbitCalcsCL planetno lat long date\n");
		printf("planetno as integer\n");
		printf("lat and long as decimals(west negative)\n");
		printf("date as yyyymmdd\n\n");
		printf("Moon=1,Mercury=2,Venus=3,Mars=4,Jupiter=5,Saturn=6,Uranus=7,Neptune=8,Pluto=10\n");
		//sleep(100);;
		return -1;
	}

	int planetno = atoi(argv[1]);
	double lati = atof(argv[2]);
	double longi = atof(argv[3]);
	long dtval = atoi(argv[4]);

	int yy = (dtval / 10000);
	int mo = (dtval - yy * 10000) / 100;
	int dy = dtval - yy * 10000 - mo * 100;

	int vis_or_tele = 1;//  (planetno < 4 ? 1: 2);
	int intvl = (planetno < 4 ? 7 : 30);

	int maxiters = (planetno < 4?104:72); 

	double dd = days(yy, mo, dy, 0, 0, 0);
	long maxloaded = LoadOrbitalElements(elements);

	char fname[128];
	sprintf(fname, "c:/temp/%d-%3.3lf-%3.3lf-%ld.csv", planetno, lati, longi, dtval);
	FILE * outf = fopen(fname, "w");
	for (int i = 1; i < maxiters; i++)
	{
		long unixdt = AstroDtToUnixTS(dd);
		double balt = IsVisible(planetno, dd+36525, lati, longi, vis_or_tele, 1, 10, 1010);
		double btim = IsVisible(planetno, dd + 36525, lati, longi, vis_or_tele, 2, 10, 1010)*24;
		fprintf(outf, "%ld, %lf, %lf, %lf\n", unixdt, btim, balt, dd);
		dd += intvl;
	}
	fclose(outf);
	return 0;
}