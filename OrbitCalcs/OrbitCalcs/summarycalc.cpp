#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "OrbitCalcs.h"
#include "Comets.h"

#define MINMAG 17.1

extern struct cometData aComet;
void WriteHeader(FILE* outf);
void CreateOutputLine(FILE* outf, char*name, double siz, double mag, char* t,
	double alti, double azi, char* f, double dec, char* ri, char* se, char* targ, double ra, int outputtype);
void WriteFooter(FILE* outf, int outputtype);

int main(int argc, char **argv)
{
	double temp = 10, press = 1010, lati = 51.88, longi = -1.31;
	double tt, alti, azi, ra, dec, brig, siz, crise, cset;
	char ri[6], se[6], f[6], t[6];

	if (argc < 6)
	{
		printf("usage: summarycalc cometfile outputfile datafilepath cometlist cometoutput\n");
		return -1;
	}

	strcpy(szPath, argv[3]);
	maxloaded = LoadOrbitalElements(elements);

	FILE *inf = fopen(argv[1], "r");
	FILE * outf = fopen(argv[2], "w");
	FILE *comf = fopen(argv[4], "w");
	FILE *comoutf = fopen(argv[5], "w");

	if (!inf || !outf || !comf || !comoutf) {
		printf("unable to open input or output file\n");
		return -1;
	}
	time_t now = time(0);
	struct tm* tstruct = gmtime(&now);
	int yr = tstruct->tm_year + 1900;
	int mth = tstruct->tm_mon + 1;
	double dy = tstruct->tm_mday + tstruct->tm_hour / 24.0 + tstruct->tm_min / (24.0*60.0);

	FILE *jd = fopen("jd.txt", "w");
	double thejd = JulianDate(yr, mth, tstruct->tm_mday, 12, 0, 0);
	fprintf(jd, "%.0f\n",thejd);
	fclose(jd);
	// local sidereal time
	double lst = LocalSiderealTime(yr, mth, tstruct->tm_mday,
		tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, longi) / 24.0;

	WriteHeader(outf);
	WriteHeader(comoutf);

	int planetno;
	double dt= GetDtvalFromDate(yr, mth, tstruct->tm_mday, 0,0,0);
	for (planetno = MERCURY; planetno <= maxloaded; planetno++)
	{
		if (planetno == EARTH) planetno++;
		cleanup_name(elements[planetno].name);
		tt = IsVisible(planetno, dt, lati, longi, 1, 2, temp, press);

		// adjust LST to match best time
		int hh = (int)(tt*24.0);
		int mm = (int)((tt *24.0 - hh) * 60);
		lst = LocalSiderealTime(yr, mth, (int)dy, hh, mm, 0, longi)/24.0;

		alti = IsVisible(planetno, dt, lati, longi, 1, 1, temp, press);
		if (alti== 0)
			azi = 0;
		else
		{
			alti = PlanetXYZ(planetno, AstroDaysFromDt(dt + tt), 8, lst, lati, temp, press);
			azi = PlanetXYZ(planetno, AstroDaysFromDt(dt + tt), 9, lst, lati, temp, press);
		}
		ra = PlanetXYZ(planetno, AstroDaysFromDt(dt+tt), 6, lst, lati, temp, press);
		dec = PlanetXYZ(planetno, AstroDaysFromDt(dt+tt), 7, lst, lati, temp, press);
		brig = VisualMagnitude(planetno, AstroDaysFromDt(dt+tt));
		siz = ApparentSize(planetno, AstroDaysFromDt(dt+tt));
		crise = RiseSet(planetno, dt, lati, longi, 1, 5, temp, press, 1);
		cset = RiseSet(planetno, dt, lati, longi, 2, 5, temp, press, 1);

		strcpy(f, fmt_hours(ra));
		strcpy(t,fmt_hours(tt * 24* 15));
		strcpy(ri, fmt_hours(crise * 15));
		strcpy(se, fmt_hours(cset * 15));

		printf("Check %s %.2f %.2f %s %.2f %.2f %s %.2f %s %s %f %f\n", 
			elements[planetno].name, siz, brig, t, alti, azi, f, dec, ri, se, lst, tt);
		CreateOutputLine(outf, elements[planetno].name, siz, brig, t, alti, azi, f, dec, ri, se, "planets", ra/15,1);
	}

	int sts = 1;
	siz = 0; // comets too small to show a disk
	while (sts) {
		sts = readoneline(inf);
		if (!sts)
			break;

		double sundist = CometSunDist(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy);
		double earthdist = CometEarthDist(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy,
			aComet.N, aComet.omega, aComet.incl);
		double mag = CometMagnitude(aComet.mag[0], aComet.mag[1], sundist, earthdist);

		if (mag < MINMAG)
		{
			double minalt = 5.0; // minimum altitude to be vaguely visible

			// get the declination to work out if the comet is visible
			earthdist = CometXYZ(aComet.peri, aComet.e, yr, mth, dy, aComet.yr, aComet.mth, aComet.dy,
				aComet.N, aComet.omega, aComet.incl, temp, press, lst, lati, ra, dec, alti, azi);

			// local hour angle is the angle between local south and set or rise
			// if this is greater than 1 then the object never rises. If its less than -1 the object never sets.
			double coslha = (sin(minalt / RAD2DEG) - sin(dec / RAD2DEG) * sin(lati / RAD2DEG)) / (cos(dec / RAD2DEG) * cos(lati / RAD2DEG));
			double dd = days(yr, mth, (int)dy, 0, 0, 0);
			double tt = GenericTimeofTransit(dd, ra, 0, longi); //tz=0 == GMT

			// work this out again but at the best time
			// adjust LST to match best time
			int hh = (int)(tt*24.0);
			int mm = (int)((tt *24.0 - hh) * 60);
			lst = LocalSiderealTime(yr, mth, (int)dy, hh, mm, 0, longi) / 24.0;
			earthdist = CometXYZ(aComet.peri, aComet.e, yr, mth, dy+tt/24.0, aComet.yr, aComet.mth, aComet.dy,
				aComet.N, aComet.omega, aComet.incl, temp, press, lst, lati, ra, dec, alti, azi);

			if (coslha < 1) // comet above horizon at some point
			{

				if (coslha < -1)
				{
					strcpy(ri, "N/S"); strcpy(se, "N/S");
				}
				else
				{

					double lha = acos(coslha);
					lha = lha * RAD2DEG;
					crise = tt - lha / HR2DEG;
					cset = tt + lha / HR2DEG;

					while (crise < 0)
						crise += 24;

					while (cset > 24)
						cset -= 24;

					strcpy(ri, fmt_hours(crise * 15));
					strcpy(se, fmt_hours(cset * 15));
				}
			}
			else // comet below my horizon
			{
				azi = 0; alti=0;
				strcpy(ri, "N/R"); strcpy(se, "N/R");
			}
			char f[6], t[6];
			strcpy(f, fmt_hours(ra));
			strcpy(t, fmt_hours(tt * 15));
			cleanup_name(aComet.name);
			printf("Check2 %s %.2f %.2f %s %.2f %.2f %s %.2f %s %s\n", aComet.name, siz, mag, t, alti, azi, f, dec, ri, se);
			if (mag < 10){
				CreateOutputLine(outf, aComet.name, siz, mag, t, alti, azi, f, dec, ri, se, "comets", ra/15, 1);
			}
			CreateOutputLine(comoutf, aComet.name, siz, mag, t, alti, azi, f, dec, ri, se, "comets", ra/15, 0);
 			fprintf(comf, "%s,%s\n", aComet.id, aComet.name);
		}
	}

	WriteFooter(outf, 1);
	WriteFooter(comoutf, 0);
	fclose(inf);
	fclose(outf);
	fclose(comoutf);
        fclose(comf);

	return 0;
}

void CreateOutputLine(FILE* outf, char*name, double siz, double mag, char* t, 
	double alti, double azi, char* f, double dec, char* ri, char* se, char* targ, double ra, int outputtype=1)
{
	fprintf(outf,"var row = table.insertRow(-1);\n");
	fprintf(outf, "var cell = row.insertCell(0);\n");
	fprintf(outf, "cell.innerHTML = \"\\<a href=\\\"%s/\\\"\\>%s\\</a\\>\";\n", name, name);
	fprintf(outf, "var cell = row.insertCell(1);\n");
	fprintf(outf, "cell.innerHTML = \"%2.2f\";\n", siz);

		fprintf(outf, "var cell = row.insertCell(2);\n");
	fprintf(outf, "cell.innerHTML = \"%2.2f\";\n", mag);
	fprintf(outf, "var cell = row.insertCell(3);\n");
	if (alti <0.01) 
	{
		fprintf(outf, "cell.innerHTML = \"N/V\";\n");
		fprintf(outf, "var cell = row.insertCell(4);\n");
		fprintf(outf, "cell.innerHTML = \"N/V\";\n");
		fprintf(outf, "var cell = row.insertCell(5);\n");
	}
	else 
	{
		fprintf(outf, "cell.innerHTML = \"%.2f\";\n", alti);
		fprintf(outf, "var cell = row.insertCell(4);\n");
		fprintf(outf, "cell.innerHTML = \"%2.2f\";\n", azi);
		fprintf(outf, "var cell = row.insertCell(5);\n");
	}
	fprintf(outf, "cell.innerHTML = \"%s\";\n", f);
	fprintf(outf, "var cell = row.insertCell(6);\n");
	fprintf(outf, "cell.innerHTML = \"%.2f\";\n", dec);
	fprintf(outf, "var cell = row.insertCell(7);\n");
	fprintf(outf, "cell.innerHTML = \"%s\";\n", t);
	fprintf(outf, "var cell = row.insertCell(8);\n");
	fprintf(outf, "cell.innerHTML = \"%s\";\n", ri);
	fprintf(outf, "var cell = row.insertCell(9);\n");
	fprintf(outf, "cell.innerHTML = \"%s\";\n", se);
	fprintf(outf, "var cell = row.insertCell(10);\n");
	fprintf(outf, "cell.innerHTML = getConstellation(%f,%f,2000);\n", ra, dec);
	fprintf(outf, "\n");
}


void WriteHeader(FILE* outf)
{
	fprintf(outf, "$(function() {\n");
	fprintf(outf, "var table = document.createElement(\"table\");\n");
	fprintf(outf, "table.className = \"table table-striped table-bordered table-hover table-condensed\";\n");
	fprintf(outf, "table.setAttribute(\"id\", \"comtableid\");\n");
	fprintf(outf, "var header = table.createTHead();\n");
	fprintf(outf, "header.className = \"h4\"; \n");
	fprintf(outf, "\n");
}

void WriteFooter(FILE* outf, int outputtype=1)
{
	fprintf(outf, "var row = header.insertRow(0);\n");
	fprintf(outf, "var cell = row.insertCell(0);\n");
	fprintf(outf, "cell.innerHTML = \"Body\";\n");
	fprintf(outf, "var cell = row.insertCell(1);\n");
	fprintf(outf, "cell.innerHTML = \"Size\";\n");

	fprintf(outf, "var cell = row.insertCell(2);\n");
	fprintf(outf, "cell.innerHTML = \"Mag\";\n");

	fprintf(outf, "var cell = row.insertCell(3);\n");
	fprintf(outf, "cell.innerHTML = \"Best Alt\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(4);\n");
	fprintf(outf, "cell.innerHTML = \"Best Az\";\n");
	fprintf(outf, "cell.className = \"small\";\n");

	fprintf(outf, "var cell = row.insertCell(3);\n");
	fprintf(outf, "cell.innerHTML = \"RA\";\n");
	fprintf(outf, "var cell = row.insertCell(4);\n");
	fprintf(outf, "cell.innerHTML = \"Dec\";\n");

	fprintf(outf, "var cell = row.insertCell(5);\n");
	fprintf(outf, "cell.innerHTML = \"Transit\";\n");
	fprintf(outf, "var cell = row.insertCell(6);\n");
	fprintf(outf, "cell.innerHTML = \"Rise\";\n");
	fprintf(outf, "var cell = row.insertCell(7);\n");
	fprintf(outf, "cell.innerHTML = \"Set\";\n");
	fprintf(outf, "var cell = row.insertCell(8);\n");
	fprintf(outf, "cell.innerHTML = \"Cons\";\n");
	fprintf(outf, "var outer_div = document.getElementById(\"table6hrs\");\n");
	fprintf(outf, "outer_div.appendChild(table);\n");
	fprintf(outf, "})\n");
	if (outputtype != 1) {
		fprintf(outf, "$(document).ready(function() {\n");
		fprintf(outf, "$(\"#comtableid\").DataTable({\n");
		fprintf(outf, "columnDefs : [\n");
		fprintf(outf, "{ Type : \"numeric\", targets : [2]}\n");
		fprintf(outf, "],\n");
		fprintf(outf, "order : [[ 2, \"asc\"]],\n");
		fprintf(outf, "paging: false\n");
		fprintf(outf, "});});\n");
	}
	fprintf(outf, "\n");
}
