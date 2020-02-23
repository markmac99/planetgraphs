// shaddap with the fopen warnings
#define _CRT_SECURE_NO_WARNINGS 1

#include <string.h>
#include <math.h>
#include <stdio.h>

#include "OrbitCalcs.h"
#include "Comets.h"
#include "Outputter.h"

void addHeader(FILE* f, char* pname, char* typ)
{
	fprintf(f, "$(function() {\n");
	fprintf(f, "Morris.Line({\n");
	fprintf(f, "  element: 'planet-%s',\n", typ);
	fprintf(f, "data: [ \n");
}

void addFooter(FILE* f, char* unt, char* typ, double minv, double maxv)
{
	fprintf(f, "        xkey: 'time',\n");
	fprintf(f, "        ykeys: ['%s'],\n", typ);
	fprintf(f, "        labels: ['%s'],\n", typ);
	fprintf(f, "        hideHover: 'auto',\n");
	fprintf(f, "        xLabelAngle: 45,\n");
	fprintf(f, "        ymax: %.0lf ,\n", maxv);
	fprintf(f, "        ymin: %.0lf,\n", minv);
	fprintf(f, "        postUnits: '%s',\n", unt);
	fprintf(f, "        resize: true\n");
	fprintf(f, "    });\n");
	fprintf(f, "});\n");
}


void SaveOrbitalElements(void)
{
	char fileloc[256];
	sprintf(fileloc, "%s/orbitalelements.csv", szPath);
	FILE* f = fopen(fileloc, "w");
	for (int i = 0; i < 10; i++)
	{
		fprintf(f, "%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
			elements[i].name, 
			elements[i].N[0], elements[i].N[1],
			elements[i].incl[0], elements[i].incl[1],
			elements[i].omega[0], elements[i].omega[1],
			elements[i].a[0], elements[i].a[1],
			elements[i].e[0], elements[i].e[1],
			elements[i].MA[0], elements[i].MA[1],
			elements[i].mag[0], elements[i].mag[1],
			elements[i].siz);
	}
	fclose(f);
}

void CreateOutputFiles(double lati, double longi, double dt)
{
	int planetno;

	for(planetno = MERCURY; planetno <= maxloaded; planetno++)
	{
		if (planetno == EARTH) planetno++;
		FILE *f1, *f2, *f3, *f4;
		char fnam[256];
		char* pname = elements[planetno].name;
		printf("%s\n", pname);
		if (pname[0] == '(')
		{
			char tmpnam[256] = { 0 };
			int j = 0;
			for (int i = 1; i < (int)strlen(pname); i++)
			{
				if (pname[i] == ')') pname[i] = '-';
				if (pname[i] == ' ') i++;
				tmpnam[j++] = pname[i];
			}
			strcpy(pname, tmpnam);
		}

		int intvl = (planetno < 4|| planetno > PLUTO ? 7 : 30);
		int maxiters = (planetno < 4 || planetno > PLUTO ? 104 : 72);

		sprintf(fnam, "%s/%sAltitude.js", szOutputPath, pname);
		f1 = fopen(fnam, "w");
		if(!f1) 
		{ 
			FILE *errf = fopen("/tmp/orbitcalcs.err","w");
			fprintf(errf, "unable to open %s for writing\n", fnam);
			fclose(errf);
			return;
		}
		sprintf(fnam, "%s/%sMagnitude.js", szOutputPath, pname);
		f2 = fopen(fnam, "w");
		sprintf(fnam, "%s/%sSize.js", szOutputPath, pname);
		f3 = fopen(fnam, "w");
		sprintf(fnam, "%s/%sData.csv", szOutputPath, pname);
		f4 = fopen(fnam, "w");

		addHeader(f1, pname, "altitude");
		addHeader(f2, pname, "magnitude");
		addHeader(f3, pname, "size");
		double minsiz = 100;
		double maxsiz = 0;
		double maxbri = -20;
		double minbri = 200;
		double minalt = 0;
		double maxalt = 0;
		fprintf(f4, "Date, Best Altitude, Magnitude, Size, Best Azimith, RA, Dec\n");

		for (int i = 0; i <= maxiters; i++)
		{
			double dv = (double)dt + ((double)intvl * i);
			double alti = IsVisible(planetno, dv, lati, longi, 1, 1, 10, 1010);
			double best = IsVisible(planetno, dv, lati, longi, 1, 2, 10, 1010);
			double brig = VisualMagnitude(planetno, AstroDaysFromDt(dv));
			double siz = ApparentSize(planetno, AstroDaysFromDt(dv));

			if (siz > maxsiz) maxsiz = siz;
			if (siz < minsiz) minsiz = siz;
			if (brig > maxbri) maxbri = brig;
			if (brig < minbri) minbri = brig;
			if (alti > maxalt) maxalt = alti;
			long tms2 = DtvalToUnixTS(dv + (double)best);

			if (i < maxiters)
			{
				fprintf(f1, "{time: %ld000, altitude: %.2lf},\n", tms2, alti);
				fprintf(f2, "{time: %ld000, magnitude: %.2lf},\n", tms2, brig);
				fprintf(f3, "{time: %ld000, size: %.2lf},\n", tms2, siz);
			}
			else
			{
				fprintf(f1, "{time: %ld000, altitude: %.2lf}],\n", tms2, alti);
				fprintf(f2, "{time: %ld000, magnitude: %.2lf}],\n", tms2, brig);
				fprintf(f3, "{time: %ld000, size: %.2lf}],\n", tms2, siz);
			}
			int yy, mo, dy, hh, mm, ss;
			GetDateFromDtval(dv + best, yy, mo, dy, hh, mm, ss);
			double dd = days(yy, mo, dy, 0, 0, 0);
			double lst = LocalSiderealTime(yy, mo, dy, 0, 0, 0, longi);
			double ra = PlanetXYZ(planetno, dd, 6, lst, lati, 10, 1010);
			double dec = PlanetXYZ(planetno, dd, 7, lst, lati, 10, 1010);
			double azi = PlanetXYZ(planetno, dd, 9, lst, lati, 10, 1010);

			fprintf(f4, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d, %.2lf, %.2lf, %.2lf, %.2lf, %s, %02lf\n", 
				yy, mo, dy, hh, mm, ss, alti, brig, siz, azi, fmt_hours(ra), dec);
		}

		maxbri = MyRound(maxbri, 0, 1);
		minbri = MyRound(minbri, 0, 2);
		maxsiz = MyRound(maxsiz * 1.2, 2, 1);
		minsiz = MyRound(minsiz * 0.8, 2, 2);
		maxalt = MyRound(maxalt * 1.2, 2, 1);

		addFooter(f1, "\\xB0", "altitude", minalt, maxalt);
		fclose(f1);
		addFooter(f2, " mag", "magnitude", maxbri, minbri);
		fclose(f2);
		addFooter(f3, " as", "size", minsiz, maxsiz);
		fclose(f3);
		fclose(f4);
	}
	CometOutputter(lati, longi, dt);
}

void CometOutputter(double lati, double longi, double dt)
{
	FILE* inf;
	char fnam[256];
	sprintf(fnam, "%s/%s", szPath, "VisCometEls.txt");
	inf = fopen(fnam, "r");
	double temp = 10, press = 1010;
	int sts = 1;
	while (sts) 
	{
		FILE *f1, *f2, *f4;
		sts = readoneline(inf);
		if (!sts)
			break;
		int intvl = 1; 
		int maxiters = 90;
		cleanup_name(aComet.name);
		char* pname = aComet.name;

		sprintf(fnam, "%s/%sAltitude.js", szOutputPath, pname);
		f1 = fopen(fnam, "w");
		sprintf(fnam, "%s/%sMagnitude.js", szOutputPath, pname);
		f2 = fopen(fnam, "w");
		//sprintf(fnam, "%s/%sSize.js", szOutputPath, pname);
		//f3 = fopen(fnam, "w");
		sprintf(fnam, "%s/%sData.csv", szOutputPath, pname);
		f4 = fopen(fnam, "w");

		addHeader(f1, pname, "altitude");
		addHeader(f2, pname, "magnitude");
		//addHeader(f3, pname, "size");
		//double minsiz = 100;
		//double maxsiz = 0;
		double maxbri = -20;
		double minbri = 200;
		double minalt = 0;
		double maxalt = 0;
		fprintf(f4, "Date, Best Altitude, Magnitude, Size, Best Azimith, RA, Dec\n");

		for (int i = 0; i <= maxiters; i++)
		{ 
			int yr, mth, dy, hh, mm, ss;
			double dv = (double)dt + (double)intvl * i;
			GetDateFromDtval(dv, yr, mth, dy, hh, mm, ss);
			double dd = days(yr, mth, dy, 0, 0, 0);
			double lst = LocalSiderealTime(yr, mth, dy, 0, 0, 0, longi);

			double sundist = CometSunDist(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy);
			double earthdist = CometEarthDist(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy,
				aComet.N, aComet.omega, aComet.incl);
			double mag = CometMagnitude(aComet.mag[0], aComet.mag[1], sundist, earthdist);

			double ra, decl, azi, alti;

			// get the declination to work out if the comet is visible
			earthdist = CometXYZ(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy,
				aComet.N, aComet.omega, aComet.incl, temp, press, lst, lati, ra, decl, alti, azi);
			
			double best = GenericTimeofTransit(dd, ra, 0, longi)/24.0; //tz=0 == GMT
			hh = (int)(best*24.0);
			mm = (int)((best *24.0 - hh) * 60);
			lst = LocalSiderealTime(yr, mth, dy, hh, mm, 0, longi)/24.0;
			earthdist = CometXYZ(aComet.peri, aComet.e, yr, mth, dy+best, aComet.yr, aComet.mth, aComet.dy,
				aComet.N, aComet.omega, aComet.incl, temp, press, lst, lati, ra, decl, alti, azi);

			if (mag > maxbri) maxbri = mag;
			if (mag < minbri) minbri = mag;
			if (alti > maxalt) maxalt = alti;
			long tms2 = DtvalToUnixTS(dv + (double)best);
			if (i < maxiters)
			{
				fprintf(f1, "{time: %ld000, altitude: %.2lf},\n", tms2, alti);
				fprintf(f2, "{time: %ld000, magnitude: %.2lf},\n", tms2, mag);
			//	fprintf(f3, "{time: %ld000, size: %.2lf},\n", tms2, siz);
			}
			else
			{
				fprintf(f1, "{time: %ld000, altitude: %.2lf}],\n", tms2, alti);
				fprintf(f2, "{time: %ld000, magnitude: %.2lf}],\n", tms2, mag);
			//	fprintf(f3, "{time: %ld000, size: %.2lf}],\n", tms2, siz);
				GetDateFromDtval(dv + (double)best, yr, mth, dy, hh, mm, ss);
				fprintf(f4, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d, %.2lf, %.2lf, %.2lf, %.2lf, %s, %02lf\n",
					yr, mth, dy, hh, mm, ss, alti, mag, 0.0, azi, fmt_hours(ra), decl);
			}

		}
		maxbri = MyRound(maxbri, 0, 1);
		minbri = MyRound(minbri, 0, 2);
		maxalt = MyRound(maxalt * 1.2, 2, 1);

		addFooter(f1, "\\xB0", "altitude", minalt, maxalt);
		fclose(f1);
		addFooter(f2, " mag", "magnitude", maxbri, minbri);
		fclose(f2);
		//addFooter(f3, " as", "size", minsiz, maxsiz);
		//fclose(f3);
		fclose(f4);
	}
	fclose(inf);
}

double MyRound(double input, int places, int updown)
{
	double multi = pow(10, places);
	return updown == 1 ? ceil(input*multi) / multi: floor(input*multi) / multi;
}

