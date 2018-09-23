#pragma once

#define MAXCOMETS 15

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

//extern cometData *Comets;

double __stdcall CometSemiMajorAxis(double e, double q);
double __stdcall CometPeriod(double e, double q);
double __stdcall CometMeanAnomaly(int yr, int mth, double dy, double P);
double __stdcall CometDayNumber(int yr, int mth, double dy);


