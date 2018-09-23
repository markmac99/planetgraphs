#include "OrbitCalcs.h"
#include <time.h>
#include <math.h>

// model taken from http://www.stjarnhimlen.se/comp/ppcomp.html


double __stdcall CometSemiMajorAxis(double e, double q)
{
	if (fabs(1-e) < 1E-7)
		return 1e12; // parabolic comet, a is infinity
	else
		return (q / (1 - e));
}

double __stdcall CometPeriod(double e, double q)
{
	if (fabs(1-e) < 1E-7)
		return 1e12; // parabolic comet, period is inifinity
	else
		return 365.2568984 * pow(CometSemiMajorAxis(e, q), 1.5);
}

double __stdcall CometDayNumber(int yr, int mth, double dy)
{
	int d1 = (int)floor(dy);
	double h = (dy - d1) * 24;
	int hh = (int)floor(h);
	double m = (h - hh) * 60;
	int mm = (int)floor(m);
	int ss = (int)((m - mm) * 60);

	double dd = days(yr, mth, d1, hh, mm, ss);
	return dd;
}

double __stdcall CometMeanAnomaly(int yr, int mth, double dy, double P)
{
	double dd = CometDayNumber(yr, mth, dy);
	double MA = 360.0*dd / P;

	while (MA > 360) 
		MA -= 360;
	while (MA < 0)
		MA += 360;
	return MA;
}