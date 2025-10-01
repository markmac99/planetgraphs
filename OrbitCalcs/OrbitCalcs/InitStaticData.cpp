#include "OrbitCalcs.h"
// shaddap with the strncpy warnings
#define  _CRT_SECURE_NO_WARNINGS 1


#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
Main planetary data from http://www.stjarnhimlen.se/comp/ppcomp.html#4 Data is J2000 Epoch
Minor bodies orbital elements from https://ssd.jpl.nasa.gov/sbdb.cgi?sstr=1 Data epoch stated on webpage

download multiple minor planet datasets here: https://ssd.jpl.nasa.gov/sbdb_query.cgi#x

Other useful pages http://www.met.rdg.ac.uk/~ross/Astronomy/Planets.html - orbital elements (slightly different values!)
and http://cosinekitty.com/solar_system.html - calculator for comparing with my calcs
*/
int __stdcall InitialiseStatic(int dummy)
{
	return LoadOrbitalElements(elements);
}


int LoadOrbitalElements(OrbitalElements* elements)
{
	int yr = 2000, mo = 1, dy=0;
	memset(elements, 0, sizeof(OrbitalElements)*NUMELEMENTS);

	int n = 0;
	strcpy(elements[0].name,"Sun");
	elements[0].N[0] = 0;
	elements[0].N[1] = 0;
	elements[0].incl[0] = 0;
	elements[0].incl[1] = 0;
	elements[0].omega[0] = 282.9404;
	elements[0].omega[1] = 0.0000470935;
	elements[0].a[0] = 1.000001018;
	elements[0].a[1] = 0;
	elements[0].e[0] = 0.016709;
	elements[0].e[1] = -1.15091033538672 * pow(10,-9);
	elements[0].MA[0] = 356.047;
	elements[0].MA[1] = 0.9856002585;
	elements[0].mag[0] = -26.76;
	elements[0].mag[1] = elements[2].mag[1] = elements[0].mag[3] = 0;
	elements[0].siz = 1919.26;
	elements[0].epoch[0] = yr;
	elements[0].epoch[1] = mo;
	elements[0].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);

	n++;
	strcpy(elements[1].name,"Moon");
	elements[1].N[0] = 125.1228;
	elements[1].N[1] = -0.0529538083;
	elements[1].incl[0] = 5.1454;
	elements[1].incl[1] = 0;
	elements[1].omega[0] = 318.0634;
	elements[1].omega[1] = 0.1643573223;
	elements[1].a[0] = 60.2666;
	elements[1].a[1] = 0;
	elements[1].e[0] = 0.0549;
	elements[1].e[1] = 0;
	elements[1].MA[0] = 115.3654;
	elements[1].MA[1] = 13.0649929509;
	elements[1].mag[0] = 0.23;
	elements[1].mag[1] = 0.026;
	elements[1].mag[2] = 0.000000004;
	elements[1].mag[3] = 4;
	elements[1].siz = 1873.7;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[2].name,"Mercury");
	elements[2].N[0] = 48.3313;
	elements[2].N[1] = 0.0000324587;
	elements[2].incl[0] = 7.0047;
	elements[2].incl[1] = 0.00000005;
	elements[2].omega[0] = 29.1241;
	elements[2].omega[1] = 0.0000101444;
	elements[2].a[0] = 0.387098;
	elements[2].a[1] = 0;
	elements[2].e[0] = 0.205635;
	elements[2].e[1] = 0.000000000559;
	elements[2].MA[0] = 168.6562;
	elements[2].MA[1] = 4.0923344368;
	elements[2].mag[0] = -0.36;
	elements[2].mag[1] = 0.027;
	elements[2].mag[2] = 0.00000000000022;
	elements[2].mag[3] = 6;
	elements[2].siz = 6.74;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[3].name, "Venus");
	elements[3].N[0] = 76.6799;
	elements[3].N[1] = 0.000024659;
	elements[3].incl[0] = 3.3946;
	elements[3].incl[1] = 0.0000000275;
	elements[3].omega[0] = 54.891;
	elements[3].omega[1] = 0.0000138374;
	elements[3].a[0] = 0.72333;
	elements[3].a[1] = 0;
	elements[3].e[0] = 0.006773;
	elements[3].e[1] = -0.000000001302;
	elements[3].MA[0] = 48.0052;
	elements[3].MA[1] = 1.6021302244;
	elements[3].mag[0] = -4.34;
	elements[3].mag[1] = 0.013;
	elements[3].mag[2] = 0.00000042;
	elements[3].mag[3] = 3;
	elements[3].siz = 16.92;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[4].name, "Mars");
	elements[4].N[0] = 49.5574;
	elements[4].N[1] = 0.0000211081;
	elements[4].incl[0] = 1.8497;
	elements[4].incl[1] = -0.0000000178;
	elements[4].omega[0] = 286.5016;
	elements[4].omega[1] = 0.0000292961;
	elements[4].a[0] = 1.523688;
	elements[4].a[1] = 0;
	elements[4].e[0] = 0.093405;
	elements[4].e[1] = 0.000000002516;
	elements[4].MA[0] = 18.6021;
	elements[4].MA[1] = 0.5240207766;
	elements[4].mag[0] = -1.51;
	elements[4].mag[1] = 0.016;
	elements[4].mag[2] = 0;
	elements[4].mag[3] = 0;
	elements[4].siz = 9.36;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[5].name, "Jupiter");
	elements[5].N[0] = 100.4542;
	elements[5].N[1] = 0.0000276854;
	elements[5].incl[0] = 1.303;
	elements[5].incl[1] = -0.0000001557;
	elements[5].omega[0] = 273.8777;
	elements[5].omega[1] = 0.0000164505;
	elements[5].a[0] = 5.20256;
	elements[5].a[1] = 0;
	elements[5].e[0] = 0.048498;
	elements[5].e[1] = 0.000000004469;
	elements[5].MA[0] = 19.895;
	elements[5].MA[1] = 0.0830853001;
	elements[5].mag[0] = -9.25;
	elements[5].mag[1] = 0.014;
	elements[5].mag[2] = 0;
	elements[5].mag[3] = 0;
	elements[5].siz = 196.34;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[6].name, "Saturn");
	elements[6].N[0] = 113.71504 ; //113.6634;
	elements[6].N[1] = 0.000023898;
	elements[6].incl[0] = 2.48446; //2.4886;
	elements[6].incl[1] = -0.0000001081;
	elements[6].omega[0] = 339.329; //339.3939;
	elements[6].omega[1] = 0.0000297661;
	elements[6].a[0] = 9.53707032; //9.55475;
	elements[6].a[1] = 0;
	elements[6].e[0] = 0.05415060; //0.055546;
	elements[6].e[1] = -0.000000009499;
	elements[6].MA[0] = 317.020; //316.967;
	elements[6].MA[1] = 0.0334442282;
	elements[6].mag[0] = -9;
	elements[6].mag[1] = 0.044;
	elements[6].mag[2] = 0;
	elements[6].mag[3] = 0;
	elements[6].siz = 165.6;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[7].name, "Uranus");
	elements[7].N[0] = 74.0005;
	elements[7].N[1] = 0.000013978;
	elements[7].incl[0] = 0.7733;
	elements[7].incl[1] = 0.000000019;
	elements[7].omega[0] = 96.6612;
	elements[7].omega[1] = 0.000030565;
	elements[7].a[0] = 19.18171;
	elements[7].a[1] = -0.0000000155;
	elements[7].e[0] = 0.047318;
	elements[7].e[1] = 0.00000000745;
	elements[7].MA[0] = 142.5905;
	elements[7].MA[1] = 0.011725806;
	elements[7].mag[0] = -7.15;
	elements[7].mag[1] = 0.001;
	elements[7].mag[2] = 0;
	elements[7].mag[3] = 0;
	elements[7].siz = 65.8/0.93;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[8].name, "Neptune");
	elements[8].N[0] = 131.7806;
	elements[8].N[1] = 0.000030173;
	elements[8].incl[0] = 1.77;
	elements[8].incl[1] = -0.000000255;
	elements[8].omega[0] = 272.8461;
	elements[8].omega[1] = -0.000006027;
	elements[8].a[0] = 30.05826;
	elements[8].a[1] = 0.00000003313;
	elements[8].e[0] = 0.008606;
	elements[8].e[1] = 0.00000000215;
	elements[8].MA[0] = 260.2471;
	elements[8].MA[1] = 0.005995147;
	elements[8].mag[0] = -6.9;
	elements[8].mag[1] = 0.001;
	elements[8].mag[2] = 0;
	elements[8].mag[3] = 0;
	elements[8].siz = 62.2/0.91;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	strcpy(elements[9].name, "Earth");
	elements[9].N[0] = 174.873;
	elements[9].N[1] = 0;
	elements[9].incl[0] = 0;
	elements[9].incl[1] = 0;
	elements[9].omega[0] = 288.064;
	elements[9].omega[1] = 0.0000470935;
	elements[9].a[0] = 1.000001018;
	elements[9].a[1] = 0;
	elements[9].e[0] = 0.016709;
	elements[9].e[1] = -1.15091033538672 * pow(10,-9);
	elements[9].MA[0] = 356.047;
	elements[9].MA[1] = 0.9856002585;
	elements[9].mag[0] = 0;
	elements[9].mag[1] = 0;
	elements[9].mag[2] = 0;
	elements[9].mag[3] = 0;
	elements[9].siz = 17.59;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	// no proper orbital elements for pluto
	strcpy(elements[10].name, "Pluto");
	elements[n].a[0] = 40;
	elements[n].mag[0] = -1;
	elements[n].mag[1] = elements[n].mag[2] = elements[n].mag[3] = 0;
	elements[n].siz = 2376.0 / AU * 3600.0 * RAD2DEG;
	elements[n].epoch[0] = yr;
	elements[n].epoch[1] = mo;
	elements[n].epoch[2] = dy;
//	printf("Loaded %s %lf %lf\n", elements[n].name, elements[n].N[0], elements[n].MA[0]);
	n++;

	// Asteroids start from ID 11
	n = 11;
	int maxn = LoadAsteroidsMPC(n);
	if (maxn == -1)
		return 10;
	for (int i = n; i <= maxn; i++)
	{
		// adjust size to be in arcsecs
		elements[i].siz *= (3600.0 * RAD2DEG) / (elements[i].a[0] * AU);

		//  adjust to J2000 epoch - 2451543.5 - from the epoch of the data from JPL
		double jd = JulianDate(elements[i].epoch[0], elements[i].epoch[1], elements[i].epoch[2], 0, 0, 0);
		double dd = days(elements[i].epoch[0], elements[i].epoch[1], elements[i].epoch[2], 0, 0, 0);
		double epochyr = elements[i].epoch[0] + elements[i].epoch[1] / 12.0 + elements[i].epoch[2] / (30.0*12.0); // approx value

		elements[i].MA[0] -= elements[i].MA[1] * (jd - 2451543.5);
		while (elements[i].MA[0] > 360)
			elements[i].MA[0] -= 360;
		while (elements[i].MA[0] < 0)
			elements[i].MA[0] += 360;

		// precession correction to the longitude of the ascending node - makes very small difference!
		elements[i].N[0] -= PrecessionCorr(epochyr, dd);
	}
	return maxn;
}

void decode_datestr(char* epochstr, long &yr, long &mo, long &dy);

// get data file from http://www.minorplanetcenter.net/iau/MPCORB/MPCORB.DAT.gz
// will need to be gunzipped

int LoadAsteroidsMPC(int n)
{
	FILE *f = NULL;
	FILE *errf = NULL;
	char fileloc[1024];
	char id[8], name[32];
	double M, G, MA, w, N, i, e, dm, a;
//	long epochyyyymmdd;

	sprintf(fileloc, "%s/MPCORB.DAT", szPath);
	f = fopen(fileloc, "r");
	if (f == NULL)
	{
		sprintf(fileloc, "%s/orbitcalcs.err", szPath);
		errf = fopen(fileloc, "w");
		fprintf(errf, "unable to find %s", fileloc);
		fclose(errf);
		return -1;
	}
	char line[256] = { 0 };
	while (strncmp(line, "----", 4) != 0)
		fgets(line, 256, f);

	int ii = 0;
	while (fgets(line, 256, f) != NULL && ii < MAXMPC)
	{
		char tmp[11] = { 0 };
		char epochstr[6] = { 0 };

		memset(name, 0, 32);
		memset(id, 0, 8);

		strncpy(id, line, 7);
		trim(id);

		strncpy(tmp, line + 8, 5);
		M = atof(tmp);
		strncpy(tmp, line + 14, 5);
		G = atof(tmp);

		strncpy(epochstr, line + 20, 5);

		strncpy(tmp, line + 26, 9);
		MA = atof(tmp);
		strncpy(tmp, line + 37, 9);
		w = atof(tmp);
		strncpy(tmp, line + 48, 9);
		N = atof(tmp);
		strncpy(tmp, line + 59, 9);
		i = atof(tmp);
		strncpy(tmp, line + 70, 9);
		e = atof(tmp);
		strncpy(tmp, line + 81, 10);
		dm = atof(tmp);
		strncpy(tmp, line + 93, 10);
		a = atof(tmp);

		strncpy(name, line + 170, 20);
		trim(name);
//		epochyyyymmdd = atol(line + 194); // this is actually the last observation date - ignore! 

//		printf("Loaded %s %s %ld %lf\n", name, id, epochyyyymmdd, M);
		ii++;
		strcpy(elements[n].name, name);
		elements[n].N[0] = N; elements[n].N[1] = 0.0;
		elements[n].incl[0] = i; elements[n].incl[1] = 0.0;
		elements[n].omega[0] = w; elements[n].omega[1] = 0.0;
		elements[n].a[0] = a; elements[n].a[1] = 0.0;
		elements[n].e[0] = e; elements[n].e[1] = 0.0;
		elements[n].MA[0] = MA; elements[n].MA[1] = dm;
		elements[n].mag[0] = M; elements[n].mag[1] = G;
		elements[n].siz = 0;

		long yr, mo, dy;
		decode_datestr(epochstr, yr, mo, dy);

//		long yr = epochyyyymmdd / 10000;
//		long mo = (epochyyyymmdd - yr * 10000) / 100;
//		long dy = (epochyyyymmdd - yr * 10000 - mo * 100);
		elements[n].epoch[0] = yr;
		elements[n].epoch[1] = mo;
		elements[n].epoch[2] = dy;

		n++;
	}
	fclose(f);

	return n-1;
}

void decode_datestr(char* epochstr, long &yr, long &mo, long &dy)
{
	yr = 100 * (epochstr[0] - 'I' + 18) + 10 * (epochstr[1] - '0') + (epochstr[2] - '0');
	mo = epochstr[3] - '0';
	if (epochstr[4] > '9')
		dy = epochstr[4] - 'A' + 10;
	else
		dy = epochstr[4] - '0';
}