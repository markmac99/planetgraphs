// ConjunctionCalculator.cpp : Defines the entry point for the console application.
//

// shaddap with the fopen warnings
#define _CRT_SECURE_NO_WARNINGS 1

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include "OrbitCalcs.h"

typedef struct Conjunction
{
	int yr;
	int mth;
	int dy;
	int hr;
	int p1;
	char p1name[12];
	double p1ra, p1dec;
	int p2;
	char p2name[12];
	double radiff;
	double decdiff;
	double minsep;
	int from;
	int to;
	double bestalti;
	double sunriseset;
}_Conjunction;

int CompareDates(const void *c1, const void *c2);

void ConjWriteHeader(FILE* outf);
void ConjCreateOutputLine(FILE* outf, struct Conjunction c);
void ConjWriteFooter(FILE* outf);

struct Conjunction Conjunctions[1000] = { 0 };

void addConj(int &conjno, int yr, int mm, int dy, int hr, int p1, int p2, double radiff, double decdiff, 
	double minsep, double ra, double dec, double alti, double sunriseset);

int main(int argc, char** argv)
{
	double temp = 10, press = 1010;
	if (argc < 6)
	{
		std::cout << "Usage: ConjunctionCalculator lat long minsep_in_degrees years_to_calc_for output_folder" << std::endl;
		exit(0);
	}
	double lati = atof(argv[1]);
	double longi = atof(argv[2]);
	double minconj = 0.5; // minimum separation in degrees
	minconj = atof(argv[3]);
	long maxdate = atol(argv[4])*365;
	
	time_t now = time(0);
	struct tm* tstruct = gmtime(&now);
	int yr = tstruct->tm_year + 1900;
	int mth = tstruct->tm_mon + 1;

	strcpy(szPath, argv[5]);

//	tstruct->tm_mday=7;// for debugging purposes, set to start of current month

	double dd = days(yr, mth, tstruct->tm_mday, 0, 0, 0);

	// local sidereal time
	double lst = LocalSiderealTime(yr, mth, tstruct->tm_mday,
		tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, longi) / 24.0;

	printf("loading data\n"); fflush(stdout);
	int maxloaded = LoadOrbitalElements(elements);

	printf("Loaded %d planets, calculating conjunctions for %ld days\n", maxloaded, maxdate); fflush(stdout);
	if (maxloaded > 10) maxloaded = 11; // just the planets thanks
	int conjno = 0;

	// iterate over the planets
	for (int j = 1; j < maxloaded; j++) 
	{
		if (j == 9)j++; //skip conjunctions with the Earth - these are unlikely...
		for (int k = j + 1; k < maxloaded; k++)
		{
			if (k == 9)k++; //skip conjunctions with the Earth - again.

			for (int i = 0; i < maxdate*24; i++)
			{
				double sunalt = -0.8333;   // civil sunset when centre of sun is 0.8333 degrees below horizon
				double nowdt = dd + i / 24.0;
				time_t now = AstroDtToUnixTS(nowdt);
				struct tm* tstruct = gmtime(&now);
				// current hour - adding 1/24 is not precise hence rounding up
				int currhour = tstruct->tm_hour + (tstruct->tm_min > 35 ? 1 : 0); 
				double dtval = GetDtvalFromDate(tstruct->tm_year + 1900, tstruct->tm_mon + 1,
					tstruct->tm_mday, tstruct->tm_hour, tstruct->tm_min, 0);

				double sunrise = RiseSet(SUN, dtval, lati, longi, 1, sunalt, 10, 1010);
				double sunset = RiseSet(SUN, dtval, lati, longi, 2, sunalt, 10, 1010);

				// no point calculating during daylight
				if (currhour > sunrise && currhour < sunset)
					continue; 

				double sunriseset = sunset;
				if (currhour < sunrise) 
						sunriseset = sunrise;

				// local sidereal time
				double lst2 = LocalSiderealTime(tstruct->tm_year + 1900, tstruct->tm_mon + 1, tstruct->tm_mday,
					tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, longi)/24.0;

				double ra1 = PlanetXYZ(j, nowdt, 6, lst, lati, temp, press);
				double dec1 = PlanetXYZ(j, nowdt, 7, lst, lati, temp, press);
				double ra2 = PlanetXYZ(k, nowdt, 6, lst, lati, temp, press);
				double dec2 = PlanetXYZ(k, nowdt, 7, lst, lati, temp, press);
				double radiff = fabs(ra1-ra2);
				double decdiff = fabs(dec1-dec2);
				// catch conjunctions where ra or dec straddles the zero line
				if (radiff > 180) radiff = fabs(radiff-360); 
				if (decdiff > 180) decdiff = fabs(decdiff-360);
				double minsep = sqrt(radiff * radiff + decdiff * decdiff);
				if (minsep < minconj)
				{
					double alti = PlanetXYZ(j, nowdt, 8, lst2, lati, temp, press);

					if (alti > 0)
					{
						addConj(conjno, tstruct->tm_year + 1900, tstruct->tm_mon + 1, tstruct->tm_mday,
							currhour, 
							j, k, radiff, decdiff, minsep, ra1, dec1, alti, sunriseset);
					}
				}
					
			}
		}
	}
	printf("%d conjunctions calculated, sorting data\n", conjno); fflush(stdout);
	qsort(Conjunctions, conjno, sizeof(struct Conjunction), CompareDates);

	printf("writing header\n"); fflush(stdout);
	char fileloc[256]={0};
	sprintf(fileloc, "%s/conjunctions.js", szPath);
	FILE* outf = fopen(fileloc,"w");
	sprintf(fileloc, "%s/conjunctions.csv", szPath);
	FILE* csvf = fopen(fileloc, "w");
	ConjWriteHeader(outf);
	fprintf(csvf,"Subject,StartDate,AllDayEvent, Description\n");
	for (int i = 1; i <= conjno; i++)
	{
		Conjunction c = Conjunctions[i];
		fprintf(stdout, "%4.4d-%2.2d-%2.2d,%8.8s,%8.8s,%7.4f,%7.4f,%7.4f,%3.3d,%3.3d,%6.2f,%6.2f,%6.2f,%3.3d\n",
			c.yr, c.mth, c.dy, c.p1name, c.p2name,
			c.radiff, c.decdiff, c.minsep, c.to, c.from, c.p1ra, c.p1dec, c.bestalti, c.hr);
		char timeofday[] ="at night";

		if (c.hr >= (c.sunriseset-2) && c.hr <=c.sunriseset) 
			strcpy(timeofday, "at dawn");
		else if (c.hr >= c.sunriseset && c.hr <= (c.sunriseset+2)) 
			strcpy(timeofday, "at dusk");
		else if (c.hr > c.sunriseset && c.hr <= c.sunriseset) strcpy(timeofday, "in day");

		fprintf(csvf, "%s-%s Conjunction, %02d/%02d/%04d,true,%s near %s %s, %2.2d hrs\n", 
			c.p1name, c.p2name, c.mth, c.dy, c.yr, c.p1name, c.p2name, timeofday,c.hr);
		ConjCreateOutputLine(outf, c);
	}
	ConjWriteFooter(outf);
	fclose(outf);
	fclose(csvf);

	return 0;
}
int CompareDates(const void	 *c1, const void *c2)
{
	struct Conjunction *c1a = (struct Conjunction*)c1;
	struct Conjunction *c2a = (struct Conjunction*)c2;
	long d1 = c1a->yr * 10000 + c1a->mth * 100 + c1a->dy;
	long d2 = c2a->yr * 10000 + c2a->mth * 100 + c2a->dy;

	if(d1 < d2) return -1;
	if (d1 > d2) return 1;
	return 0;
}

void addConj(int &conjno, int yr, int mm, int dy, int hr, int p1, int p2, double radiff, double decdiff, 
	double minsep, double ra, double dec, double alti, double sunriseset)
{
	struct Conjunction c;
	c = Conjunctions[conjno];

// look for repeat conjunctions
	if (yr == c.yr && mm == c.mth && c.p1 == p1 && c.p2 == p2 && dy < c.dy + 15) // ongoing event
	{
		if (minsep < c.minsep)
		{
			// closer conjunction
			Conjunctions[conjno].yr = yr;
			Conjunctions[conjno].mth = mm;
			Conjunctions[conjno].dy = dy;
			Conjunctions[conjno].hr = hr;
			Conjunctions[conjno].radiff = radiff;
			Conjunctions[conjno].decdiff = decdiff;
			Conjunctions[conjno].minsep = minsep;
			Conjunctions[conjno].from++;
			Conjunctions[conjno].p1ra=ra;
			Conjunctions[conjno].p1dec=dec;
			Conjunctions[conjno].bestalti = alti;
			Conjunctions[conjno].sunriseset = sunriseset;
		}
		else
			Conjunctions[conjno].to++;
	}
	else
	{
		conjno++;
		Conjunctions[conjno].yr = yr;
		Conjunctions[conjno].mth = mm;
		Conjunctions[conjno].dy = dy;
		Conjunctions[conjno].hr = hr;
		Conjunctions[conjno].radiff = radiff;
		Conjunctions[conjno].decdiff = decdiff;
		Conjunctions[conjno].minsep = minsep;
		Conjunctions[conjno].from=0;
		Conjunctions[conjno].to = 0;
		Conjunctions[conjno].p1 = p1;
		Conjunctions[conjno].p2 = p2;
		strcpy(Conjunctions[conjno].p1name,elements[p1].name);
		strcpy(Conjunctions[conjno].p2name,elements[p2].name);
		Conjunctions[conjno].p1ra = ra;
		Conjunctions[conjno].p1dec = dec;
		Conjunctions[conjno].bestalti = alti;
		Conjunctions[conjno].sunriseset = sunriseset;
	}
}

void ConjCreateOutputLine(FILE* outf, struct Conjunction c)
{
	fprintf(outf, "var row = table.insertRow(-1);\n");
	fprintf(outf, "var cell = row.insertCell(0);\n");
	fprintf(outf, "cell.innerHTML = \"%4.4d-%2.2d-%2.2d\";\n", c.yr, c.mth, c.dy);
	fprintf(outf, "var cell = row.insertCell(1);\n");
	fprintf(outf, "cell.innerHTML = \"\\<a href=\\\"/%s\\\"\\>%s\\</a\\>\";\n", c.p1name, c.p1name);
	fprintf(outf, "var cell = row.insertCell(2);\n");
	fprintf(outf, "cell.innerHTML = \"\\<a href=\\\"/%s\\\"\\>%s\\</a\\>\";\n", c.p2name, c.p2name);
	fprintf(outf, "var cell = row.insertCell(3);\n");
	fprintf(outf, "cell.innerHTML = \"%2.2f\";\n", c.minsep);
	fprintf(outf, "var cell = row.insertCell(4);\n");
	fprintf(outf, "cell.innerHTML = \"%2.2d:00\";\n", c.hr);
	fprintf(outf, "var cell = row.insertCell(5);\n");
	if (c.bestalti <0.01)
		fprintf(outf, "cell.innerHTML = \"N/V\";\n");
	else
		fprintf(outf, "cell.innerHTML = \"%.2f\";\n", c.bestalti);
}


void ConjWriteHeader(FILE* outf)
{
	fprintf(outf, "$(function() {\n");
	fprintf(outf, "var table = document.createElement(\"table\");\n");
	fprintf(outf, "table.className = \"table table-striped table-bordered table-hover table-condensed\";\n");
	fprintf(outf, "var header = table.createTHead();\n");
	fprintf(outf, "header.className = \"h4\"; \n");
	fprintf(outf, "\n");
}

void ConjWriteFooter(FILE* outf)
{
	fprintf(outf, "var row = header.insertRow(0);\n");
	fprintf(outf, "var cell = row.insertCell(0);\n");
	fprintf(outf, "cell.innerHTML = \"Date\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(1);");
	fprintf(outf, "cell.innerHTML = \"Planet 1\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(2);\n");
	fprintf(outf, "cell.innerHTML = \"Planet 2\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(3);\n");
	fprintf(outf, "cell.innerHTML = \"Sep (degs)\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(4);\n");
	fprintf(outf, "cell.innerHTML = \"Time(hh:mm)\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var cell = row.insertCell(5);\n");
	fprintf(outf, "cell.innerHTML = \"Best Alti\";\n");
	fprintf(outf, "cell.className = \"small\";\n");
	fprintf(outf, "var outer_div = document.getElementById(\"table6hrs\");\n");
	fprintf(outf, "outer_div.appendChild(table);\n");
	fprintf(outf, "})\n");
	fprintf(outf, "\n");
}
