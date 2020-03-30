#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "OrbitCalcs.h"
#include "Comets.h"

int main(int argc, char **argv)
{
	double temp = 10, press = 1010, lati = 51.88, longi = -1.31;

	if (argc < 4)
	{
		printf("usage: cometpreprocessor cometfile outputfile datafilepath\n");
		return -1;
	}
	strcpy(szPath, argv[3]);
	maxloaded = LoadOrbitalElements(elements);

	FILE *inf = fopen(argv[1], "r");
	FILE * outf = fopen(argv[2], "w");

	if (!inf || !outf) {
		printf("unable to open input or output file\n");
		exit(-1);
	}
	time_t now = time(0);
	struct tm* tstruct = gmtime(&now);	
	int yr = tstruct->tm_year+1900;
	int mth = tstruct->tm_mon + 1;
	double dy = tstruct->tm_mday + tstruct->tm_hour / 24.0 + tstruct->tm_min / (24.0*60.0);

	// local sidereal time
	double lst = LocalSiderealTime(yr, mth, tstruct->tm_mday,
		tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, longi) / 24.0;

	int sts = 1;
	while (sts) {
		sts = readoneline(inf);
		if (!sts)
			break;

		double sundist = CometSunDist(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy);
		double earthdist = CometEarthDist(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy,
			aComet.N, aComet.omega, aComet.incl);
		double mag = CometMagnitude(aComet.mag[0], aComet.mag[1], sundist, earthdist);
		fprintf(stderr, "%s %.1f\n", aComet.name, mag);

		if (mag < 15.1)
		{
			// all brighter comets to be analysed later
			fprintf(outf, "%s\n", aline);

			double ra, decl, azi, alti;
			double sunalt=-10.0 ; // -10 - sun low enough for good darkness
			double minalt = 5.0; // minimum altitude to be vaguely visible
 
			// get the declination to work out if the comet is visible
			earthdist = CometXYZ(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy,
				aComet.N, aComet.omega, aComet.incl, temp, press, lst, lati, ra, decl, alti, azi);
			 
			// local hour angle is the angle between local south and set or rise
			// if this is greater than 1 then the object never rises. If its less than -1 the object never sets.
			double coslha = (sin(minalt / RAD2DEG) - sin(decl / RAD2DEG) * sin(lati / RAD2DEG)) / (cos(decl / RAD2DEG) * cos(lati / RAD2DEG));

			if (coslha < 1) // comet above horizon at some point
			{
				double dd = days(yr, mth, (int)dy, 0, 0, 0);
				double tt = GenericTimeofTransit(dd, ra, 0, longi); //tz=0 == GMT
				if (coslha < -1)
				{
					// comet is always above the horizon
					printf("%s Mag %f RA/Dec %f %f Always visible, transits at %s\n",
						aComet.name, mag, ra, decl, TimeToStr(tt));
				}
				else
				{
					double sunrise, sunset0, crise, cset;
					// effective sunrise and set times
					double dtval = GetDtvalFromDate(yr, mth, (int)dy, 0, 0, 0);
					sunrise = RiseSet(SUN, dtval, lati, longi, 1, sunalt, temp, press);
					sunset0 = RiseSet(SUN, dtval, lati, longi, 2, sunalt, temp, press);

					double lha = acos(coslha);
					lha = lha * RAD2DEG;
					crise = tt - lha / HR2DEG;
					cset =  tt + lha / HR2DEG;

					while (crise < 0)
						crise += 24;

					while (cset > 24)
						cset -= 24;

					char cr[8] = {0}, cs[8] = { 0 }, tr[8] = { 0 };
					strncpy(cr, TimeToStr(crise), 7);
					strncpy(cs, TimeToStr(cset), 7);
					strncpy(tr, TimeToStr(tt), 7);

					if (crise < sunrise || crise > sunset0 || cset < sunrise || cset > sunset0)
					{
						printf("%s Mag %f RA/Dec %f %f rises at %s transits at %s sets at %s\n",
							aComet.name, mag, ra, decl, cr, tr, cs);
					}
				}
			}
		}
	}
	fclose(inf);
	fclose(outf);

	return 0;
}
