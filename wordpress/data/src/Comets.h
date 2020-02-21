#define _CRT_SECURE_NO_WARNINGS 1

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct cometData
{
	char id[13];	// comet number, type, provisional ID	a12
	int yr;			// year of perihelion				i4
	int mth;		// month of perihelion				i2
	double dy;		// day of perihelion (TT)			f7.4
	double peri;	// perihelion distance (AU)			f9.6
	double e;		// eccentricity						f8.6
	double omega;	// arg of perihelion J2000 (degs)	f8.4
	double N;		// long of asc node J2000 (degs)	f8.4
	double incl;	// inclination (degs)				f8.4
	int	epoch[3];	//epoch of data						yyyymmdd i4 i2 i2
	double mag[2];	// abs mag and slope param			f4.1 and f4.0
	char name[57];	// name								a56
	char ref[10];	// MPC reference					a9
} _cometData;

extern struct cometData aComet;
extern char aline[200];

double __stdcall CometSemiMajorAxis(double e, double q);
double __stdcall CometPeriod(double e, double q);
double __stdcall CometMeanAnomaly(int yr, int mth, double dy, double P, int pyr, int pmth, double pdy);
double __stdcall CometDayNumber(int yr, int mth, double dy);
double __stdcall CometSunDist(double q, double e, int yr, int mth, double dy, int pyr, int pmth, double pdy);
double __stdcall CometMagnitude(double H, double G, double sundist, double earthdist);
double __stdcall CometEarthDist(double q, double e, int yr, int mth, double dy,
	int pyr, int pmth, double pdy, double N, double w, double i);
double __stdcall CometXYZ(double q, double e, int yr, int mth, double dy,
	int pyr, int pmth, double pdy, double N, double w, double i,
	double temp, double press, double lst, double lat, 
	double& ra, double& decl, double& alti, double& azi);

void CometOutputter(double lati, double longi, double dt);

int readoneline(FILE* inf);

/*
Comet Data column description
Columns   F77    Use

1 -   4  i4     Periodic comet number
5        a1     Orbit type (generally `C', `P' or `D')
6 -  12  a7     Provisional designation (in packed form)

15 -  18  i4     Year of perihelion passage
20 -  21  i2     Month of perihelion passage
23 -  29  f7.4   Day of perihelion passage (TT)

31 -  39  f9.6   Perihelion distance (AU)
42 -  49  f8.6   Orbital eccentricity

52 -  59  f8.4   Argument of perihelion, J2000.0 (degrees)
62 -  69  f8.4   Longitude of the ascending node, J2000.0
(degrees)
72 -  79  f8.4   Inclination in degrees, J2000.0 (degrees)

82 -  85  i4     Year of epoch for perturbed solutions
86 -  87  i2     Month of epoch for perturbed solutions
88 -  89  i2     Day of epoch for perturbed solutions

92 -  95  f4.1   Absolute magnitude
97 - 100  f4.0   Slope parameter

103 - 158  a56    Designation and Name

160 - 168  a9     Reference
*/