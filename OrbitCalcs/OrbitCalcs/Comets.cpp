#define _CRT_SECURE_NO_WARNINGS 1
#include "OrbitCalcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "Comets.h"

// model taken from http://www.stjarnhimlen.se/comp/ppcomp.html

char aline[200];
struct cometData aComet;

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

double __stdcall CometMeanAnomaly(int yr, int mth, double dy, double P, int pyr, int pmth, double pdy)
{
	int d1 = (int)floor(dy);
	double h = (dy - d1) * 24;
	int hh = (int)floor(h);
	double m = (h - hh) * 60;
	int mm = (int)floor(m);
	int ss = (int)((m - mm) * 60);
	double dd = days(yr, mth, d1, hh, mm, ss);

	double dt = CometDayNumber(pyr, pmth, pdy);
	double MA = 360.0*(dd-dt) / P;

	while (MA > 360) 
		MA -= 360;
	while (MA < 0)
		MA += 360;
	return MA;
}

double __stdcall CometSunDist(double q, double e, int yr, int mth, double dy, int pyr, int pmth, double pdy)
{
	if (e < 0.9801)
	{
		double a = CometSemiMajorAxis(e, q);
		double P = CometPeriod(e, q);
		double m = CometMeanAnomaly(yr, mth, dy, P, pyr, pmth, pdy) / RAD2DEG;
		double EA = EccentricAnomaly(m, e);
		return (a* (1 - e*cos(EA)));
	}
	else
	{
		// parabolic or nearly so
		double k = 0.01720209895; // exact value of gaussian gravitational constant

		int d1 = (int)floor(dy);
		double h = (dy - d1) * 24;
		int hh = (int)floor(h);
		double m = (h - hh) * 60;
		int mm = (int)floor(m);
		int ss = (int)((m - mm) * 60);

		double dd = days(yr, mth, d1, hh, mm, ss);
		double dt = CometDayNumber(pyr, pmth, pdy);

		if (fabs(1 - e) < 1E-7)
		{
			// really parabolic
			double H = (dd - dt) * (k / sqrt(2)) / pow(q, 1.5);
			double h = 1.5 * H;
			double g = sqrt(1.0 + h*h);
			double s = pow(g + h, 1.0 / 3.0) - pow(g - h, 1.0 / 3.0);

			// heliocentric distance
			return q*(1.0 + s*s);
		}
		else
		{
			// not quite parabolic or possibly hyperbolic (e>1)
			double a = 0.75 * (dd - dt) * k * sqrt((1 + e) / pow(q, 3));
			double b = sqrt(1 + a*a);
			double W = pow(b + a, 1.0 / 3.0) - pow(b - a, 1.0 / 3.0);
			double f = (1 - e) / (1 + e);

			double a1 = (2 / 3) + (2 / 5) * W*W;
			double a2 = (7 / 5) + (33 / 35) * W*W + (37 / 175) * pow(W, 4);
			double a3 = W*W * ((432 / 175) + (956 / 1125) * W*W + (84 / 1575) * pow(W, 4));

			double C = W*W / (1 + W*W);
			double g = f * C*C;
			double w = W * (1 + f * C * (a1 + a2*g + a3*g*g));

			// heliocentric distance
			return (q * (1 + w*w) / (1 + w*w * f));
		}
	}
}

double __stdcall CometXYZ(double q, double e, int yr, int mth, double dy, int pyr, int pmth, double pdy, 
	double N, double w, double i, double temp, double press, double lst, double lat, 
	double& ra, double& decl, double& alti, double& azi)
{
	double lonecl, latecl, r, v;
	double xh, yh, zh;

	if (e < 0.9801)
	{
		// not too eccentric
		double a = CometSemiMajorAxis(e, q);
		double P = CometPeriod(e, q);
		double m = CometMeanAnomaly(yr, mth, dy, P, pyr, pmth, pdy) / RAD2DEG;
		double ea = EccentricAnomaly(m, e);

		// location in its own orbit
		double xv = cos(ea) - e;
		double yv = sqrt(1.0 - e *e) * sin(ea);

		// distance and true anomaly from perihelion
		r = a * sqrt(xv*xv + yv*yv);
		v = atan2(yv, xv);
	}
	else
	{
		// parabolic or nearly so
		double k = 0.01720209895; // exact value of gaussian gravitational constant

		int d1 = (int)floor(dy);
		double h = (dy - d1) * 24;
		int hh = (int)floor(h);
		double m = (h - hh) * 60;
		int mm = (int)floor(m);
		int ss = (int)((m - mm) * 60);

		double dd = days(yr, mth, d1, hh, mm, ss);
		double dt = CometDayNumber(pyr, pmth, pdy);

		if (fabs(1 - e) < 1E-7)
		{
			// really parabolic
			double H = (dd - dt) * (k / sqrt(2)) / pow(q, 1.5);
			double h = 1.5 * H;
			double g = sqrt(1.0 + h*h);
			double s = pow(g + h, 1.0 / 3.0) - pow(g - h, 1.0 / 3.0);

			// true anomaly and heliocentric distance
			v = 2.0 * atan(s);
			r = q*(1.0 + s*s);
		}
		else
		{
			// not quite parabolic or possibly hyperbolic (e>1)
			double a = 0.75 * (dd - dt) * k * sqrt((1 + e) / pow(q, 3));
			double b = sqrt(1 + a*a);
			double W = pow(b + a, 1.0 / 3.0) - pow(b - a, 1.0 / 3.0);
			double f = (1 - e) / (1 + e);

			double a1 = (2 / 3) + (2 / 5) * W*W;
			double a2 = (7 / 5) + (33 / 35) * W*W + (37 / 175) * pow(W, 4);
			double a3 = W*W * ((432 / 175) + (956 / 1125) * W*W + (84 / 1575) * pow(W, 4));

			double C = W*W / (1 + W*W);
			double g = f * C*C;
			double w = W * (1 + f * C * (a1 + a2*g + a3*g*g));

			// true anomaly and heliocentric distance
			v = 2 * atan(w);
			r = q * (1 + w*w) / (1 + w*w * f);
		}
	}
	// calculate heliocentric coords excluding perturbations
	// nb these coords are *geo*centric for the moon
	xh = r * (cos(N) * cos(v + w) - sin(N) * sin(v + w) * cos(i));
	yh = r * (sin(N) * cos(v + w) + cos(N) * sin(v + w) * cos(i));
	zh = r * (sin(v + w) * sin(i));

	r = sqrt(xh*xh + yh*yh + zh*zh);

	// coords in ecliptic plane
	lonecl = atan2(yh, xh);
	latecl = atan2(zh, sqrt(xh*xh + yh*yh));

	if (lonecl < 0)
		lonecl = lonecl + TWOPI;
	// convert perturbed ecliptic coords back to heliocentric
	xh = r * cos(lonecl) * cos(latecl);
	yh = r * sin(lonecl) * cos(latecl);
	zh = r * sin(latecl);

	int d1 = (int)floor(dy);
	double h = (dy - d1) * 24;
	int hh = (int)floor(h);
	double mn = (h - hh) * 60;
	int mm = (int)floor(mn);
	int ss = (int)((mn - mm) * 60);

	double dd = days(yr, mth, d1, hh, mm, ss);

	v = PlanTrueAnomaly(SUN, dd);
	r = PlanDist(SUN, dd);
	double ls = SunLongitude(v, dd);
	double xs = r * cos(ls);
	double ys = r * sin(ls);

	//add to x-y coords of planet; sun is on ecliptic so z adjust is zero
	xh = xh + xs;
	yh = yh + ys;

	double distearth = sqrt(xh*xh + yh*yh + zh*zh);

	if (lst > 0)
	{
		//calculate RA and DEC
		double ecl = ObliquityofEcliptic(dd);
		double xe = xh;
		double ye = yh * cos(ecl) - zh * sin(ecl);
		double ze = yh * sin(ecl) + zh * cos(ecl);
		ra = atan2(ye, xe) * RAD2DEG;
		decl = atan2(ze, sqrt(xe*xe + ye*ye)) * RAD2DEG;
		while (ra < 0)
			ra = ra + 360.0;
		while (ra > 360.0)
			ra = ra - 360.0;

		azi = AzFromRADec(lst, ra, decl, lat, 0, temp, press);
		alti = AzFromRADec(lst, ra, decl, lat, 1, temp, press);

		// correct to topographic coords (surface of earth)
		// mainly relevant for the moon but worth doing for the others
		alti = alti * PI / 180;
		alti = alti - (8.794 / 3600.0) / RAD2DEG / r;

		alti = alti * RAD2DEG;
	}
	return distearth;
}

double __stdcall CometEarthDist(double q, double e, int yr, int mth, double dy,
	int pyr, int pmth, double pdy, double N, double w, double i)
{
	double ra, decl, azi, alti;
	double lst = 0;
	double temp = 20, press = 1010, lati = 51.88;

	double ed= CometXYZ(q, e, yr, mth, dy, pyr, pmth, pdy, N, w, i, 
		temp, press, lst, lati, ra, decl, alti, azi);
	return ed;
}

double __stdcall CometMagnitude(double H, double G, double sundist, double earthdist)
{
	return (H  + 5.0 * log10(earthdist) +2.5 * G * log10(sundist));
}

int readoneline(FILE* inf) {
	if (feof(inf))
		return 0;
	fgets(aline, 199, inf);

	char tmp[32] = { 0 };

	// id
	strncpy(tmp, aline, 13);
	trim(tmp);
	strcpy(aComet.id, tmp);

	// year month and day of perihelion including day-fraction
	strncpy(tmp, aline + 14, 4);
	aComet.yr = atoi(tmp);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 19, 2);
	aComet.mth = atoi(tmp);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 22, 7);
	aComet.dy = atof(tmp);

	// perihelion distance, eccentricity, arg of perihelion, long of asc node, inclination
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 30, 9);
	aComet.peri = atof(tmp);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 41, 8);
	aComet.e = atof(tmp);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 51, 8);
	aComet.omega = atof(tmp) / RAD2DEG;
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 61, 8);
	aComet.N = atof(tmp) / RAD2DEG;
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 71, 8);
	aComet.incl = atof(tmp) / RAD2DEG;

	// epoch of perturbed calcs
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 81, 4);
	aComet.epoch[0] = atoi(tmp);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 85, 2);
	aComet.epoch[1] = atoi(tmp);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 87, 2);
	aComet.epoch[2] = atoi(tmp);

	// abs mag and slope of mag
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 91, 4);
	aComet.mag[0] = atof(tmp);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 96, 2);
	aComet.mag[1] = atof(tmp);

	// name and identifier
	char tmp2[57] = { 0 };
	strncpy(tmp2, aline + 102, 56);
	trim(tmp2);
	strcpy(aComet.name, tmp2);
	memset(tmp, 0, 32);
	strncpy(tmp, aline + 159, 31);
	strcpy(aComet.ref, tmp);

	return 1;
}


