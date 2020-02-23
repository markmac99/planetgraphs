// MoonCalcs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// shaddap with the fopen warnings
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <time.h>
#include "orbitcalcs.h"

int main(int argc, char** argv)
{
	double temp = 10, press = 1010;
	if (argc < 4)
	{
		std::cout << "Usage: MoonCalcs lat long years_to_calc_for" << std::endl;
		exit(0);
	}
	double lati = atof(argv[1]);
	double longi = atof(argv[2]);
	long maxdate = atol(argv[3]) * 365;

	time_t now = time(0);
	struct tm* tstruct = gmtime(&now);
	int yr = tstruct->tm_year + 1900;
	int mth =tstruct->tm_mon + 1;
	int day = tstruct->tm_mday;
	int hr = 12; // tstruct->tm_hour;
	int mins = 0; // = tstruct->tm_min;
	int sec = 0; // = tstruct->tm_sec;
	tstruct->tm_year = yr - 1900;
	tstruct->tm_mon = mth - 1;
	tstruct->tm_mday = day;
	tstruct->tm_hour = hr;
	tstruct->tm_min = mins;
	tstruct->tm_sec = sec;
	now = mktime(tstruct);

	strcpy(szPath, "./");

	double dd = days(yr, mth, day, hr, mins, sec);
	double dtval = GetDtvalFromDate(yr, mth, day, hr, mins, sec);

	printf("loading data\n"); fflush(stdout);
	double maxloaded = LoadOrbitalElements(elements);

	printf("loaded data\n"); fflush(stdout);
	if (maxloaded > 10) maxloaded = 11; // just the planets thanks

	FILE* outf = fopen("mooncalcs.js", "w");
	fprintf(outf, "$(function() {\n");
	fprintf(outf, "var table = document.createElement(\"table\");\n");
	fprintf(outf, "table.className = \"table table-striped table-bordered table-hover table-condensed\";\n");
	fprintf(outf, "var header = table.createTHead();\n");
	fprintf(outf, "header.className = \"h4\"; \n");
	fprintf(outf, "\n");

	for (int i = 0; i < maxdate; i++)
	{
		double thisdd = dd + i;
		double thisdv = dtval + i;
		now = AstroDtToUnixTS(thisdd);
		tstruct = gmtime(&now);

		// local sidereal time
		double lst2 = LocalSiderealTime(tstruct->tm_year + 1900, tstruct->tm_mon + 1, tstruct->tm_mday,
			tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, longi);
		lst2/= 24.0;

		double h = -0.25; //the upper limb is just rising
		double pres = 1010, temp = 10;

		double ra = PlanetXYZ(MOON, thisdd, 6, lst2, lati, temp, press);
		double dec = PlanetXYZ(MOON, thisdd, 7, lst2, lati, temp, press);
		double rise = RiseSet(MOON, thisdv, lati, longi, 1, h, temp, pres);
		double set  = RiseSet(MOON, thisdv, lati, longi, 2, h, temp, pres);
		double phase = PhaseOrElongGeneral(MOON, thisdd,1, lst2, lati);
		double phaseangle = PhaseOrElongGeneral(MOON, thisdd, 0, lst2, lati)*RAD2DEG;
		double elong = PhaseOrElongGeneral(MOON, thisdd, 2, lst2, lati) * RAD2DEG;

		int rah = (int)floor(ra / 15);
		int ram = (int)floor((ra - rah*15) * 4);

		printf("%04d-%02d-%02d,%02d:%02d:%02d,%03.3f,%03.3f, %0.2f, %0.2f, %0.2f\n",
			tstruct->tm_year + 1900, tstruct->tm_mon + 1, tstruct->tm_mday,
			tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec,
			ra, dec, rise, set, phase);

		fprintf(outf, "var row = table.insertRow(-1);\n");
		fprintf(outf, "var cell = row.insertCell(0);\n");
		fprintf(outf, "cell.innerHTML = \"%4.4d-%2.2d-%2.2d\";\n", 
			tstruct->tm_year + 1900, tstruct->tm_mon+1, tstruct->tm_mday);
		fprintf(outf, "var cell = row.insertCell(1);\n");
		fprintf(outf, "cell.innerHTML = \"%d:%d\";\n", rah, ram);
		fprintf(outf, "var cell = row.insertCell(2);\n");
		fprintf(outf, "cell.innerHTML = \"%3.3f\";\n", dec);
		fprintf(outf, "var cell = row.insertCell(3);\n");
		fprintf(outf, "cell.innerHTML = \"%2.2f\";\n", rise);
		fprintf(outf, "var cell = row.insertCell(4);\n");
		fprintf(outf, "cell.innerHTML = \"%2.2f\";\n", set);
		fprintf(outf, "var cell = row.insertCell(5);\n");
		fprintf(outf, "cell.innerHTML = \"%.0f\";\n", phase*100);



	}
	fprintf(outf, "var row = header.insertRow(0);\n");
	fprintf(outf, "var cell = row.insertCell(0);\n");
	fprintf(outf, "cell.innerHTML = \"Date\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(1);");
	fprintf(outf, "cell.innerHTML = \"RA\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(2);\n");
	fprintf(outf, "cell.innerHTML = \"Dec\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(3);\n");
	fprintf(outf, "cell.innerHTML = \"Rise\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(4);\n");
	fprintf(outf, "cell.innerHTML = \"Seet\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(5);\n");
	fprintf(outf, "cell.innerHTML = \"Phase\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var outer_div = document.getElementById(\"table6hrs\");\n");
	fprintf(outf, "outer_div.appendChild(table);\n");
	fprintf(outf, "})\n");
	fprintf(outf, "\n");

	fclose(outf);
}
