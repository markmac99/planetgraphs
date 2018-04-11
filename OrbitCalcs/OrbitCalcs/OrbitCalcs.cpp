#include <Windows.h>
#include <math.h>
#include "OrbitCalcs.h"

// model taken from http://www.stjarnhimlen.se/comp/ppcomp.html

struct OrbitalElements elements[NUMELEMENTS];

extern "C" BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) 
	{
		case DLL_PROCESS_ATTACH:
			LoadOrbitalElements(elements);
			break;
	}	
	return TRUE;
}

double __stdcall MeanAnomaly(int planetno , double dd ) 
{
	double k = elements[planetno].MA[0];
	double p = elements[planetno].MA[1];

	double ma = (k + p * dd) / RAD2DEG;
	while (ma > TWOPI)
		ma = ma - TWOPI;
	while (ma < 0)
		ma = ma + TWOPI;
	return ma ;
}

double __stdcall LongOfAscNode(int planetno, double d)
{
	double n1 = elements[planetno].N[0];
	double n2 = elements[planetno].N[1];
	double n = (n1 + n2 * d);
	while (n > TWOPI)
		n -= TWOPI;
	while (n < 0)
		n += TWOPI;
	return n;
}

double __stdcall Eccentricity(int planetno, double d)
{
	double e1 = elements[planetno].e[0];
	double e2 = elements[planetno].e[1];
	return (e1 + e2 * d);
}

double __stdcall Inclination(int planetno, double d)
{
	double e1 = elements[planetno].incl[0];
	double e2 = elements[planetno].incl[1];
	return (e1 + e2 * d)/RAD2DEG;
}

double __stdcall EccentricAnomaly(double m, double e)
{
	double deltae = 1;
	double e0 = m + e * sin(m) * (1.0 + e * cos(m));
	while (deltae > 0.00000001)
	{
		double e1 = e0 - (e0 - e * sin(e0) - m) / (1 - e * cos(e0));
		deltae = fabs(e1 - e0);
		e0 = e1;
	}
	return e0;
}

double __stdcall ArgOfPerihelion(int planetno, double dd)
{
	double k = elements[planetno].omega[0];
	double d = elements[planetno].omega[1];
	double aop = (k + d * dd) / RAD2DEG;
	while (aop > TWOPI)
		aop = aop - TWOPI;
	while (aop < 0)
		aop = aop + TWOPI;
	return aop;
}
double __stdcall ObliquityofEcliptic(double dd)
{
	return (23.4392911 - 0.000000356115 * dd)/RAD2DEG;
}

double __stdcall PrecessionCorr(double epoch, double dd)
{
	return (0.0000382394 * (365.2422 * (epoch - 2000.0) - dd));
}

double __stdcall SunLongitude(double v, double dd)
{
	double w = ArgOfPerihelion(SUN, dd);
	return (v + w);
}

double __stdcall SunRA(double dd)
{
	return (PlanetXYZ(SUN, dd, 6, 0, 0, 0, 0));
}
double __stdcall SunDec(double dd)
{
	return (PlanetXYZ(SUN, dd, 7, 0, 0, 0, 0));
}


double __stdcall PlanDist(int planetno, double dd)
{
	if (planetno == PLUTO)
	{
		double lonecl, latecl, r;
		DoPluto(dd, lonecl, latecl, r);
		return r;
	}
	else
	{
		double a = elements[planetno].a[0] + dd * elements[planetno].a[1];
		double m = MeanAnomaly(planetno, dd);
		double e = Eccentricity(planetno, dd);
		double ea = EccentricAnomaly(m, e);
		double xv = cos(ea) - e;
		double yv = sqrt(1.0 - e*e) * sin(ea);
		return (a * sqrt(xv*xv + yv*yv));
	}
}

double __stdcall PlanTrueAnomaly(int planetno, double d)
{
	double m = MeanAnomaly(planetno, d);
	double e = Eccentricity(planetno, d);
	double ea = EccentricAnomaly(m, e);
	double xv = (cos(ea) - e);
	double yv = (sqrt(1.0 - e*e) * sin(ea));
	double pta = atan2(yv, xv);

	while (pta > TWOPI)
		pta = pta - TWOPI;
	while (pta < 0)
		pta = pta + TWOPI;
	return pta;
}

double __stdcall AzFromRADec(double  lst, double ra, double dec, double lat, int zora, double temp, double pres)
{

	double ha = (lst * 360 - ra) / RAD2DEG;
	dec = dec / RAD2DEG;
	lat = lat / RAD2DEG;

	double x = cos(ha) * cos(dec);
	double y = sin(ha) * cos(dec);
	double Z = sin(dec);

	double xhor = x * sin(lat) - Z * cos(lat);
	double yhor = y;
	double zhor = x * cos(lat) + Z * sin(lat);

	double az = atan2(yhor, xhor) * RAD2DEG + 180.0;
	if (zora == 0)
		return az;
	else
	{
		double alti = atan2(zhor, sqrt(xhor*xhor + yhor*yhor));
		// correction for atmospheric distortion - after Saemundsson
		double h = alti*RAD2DEG;
		double hh = h + (10.3 / (h + 5.11));
		double r = 1.02 / tan(hh/RAD2DEG);

		//temperature & pressure correction - above formula assumes P=1010 and T=10
		r = r * pres / 1010 * 283.0 / (temp + 273.0);

		return (alti * RAD2DEG + r / 60.0);
	}
}

double __stdcall AltAtTransit(int planetno, double dtval, double lat, double longi, double temp, double pres)
{
	double tt = TimeofTransit(planetno, dtval, lat, longi);
	double dd = AstroDaysFromDt(dtval + tt / 24.0);
	double lst = LSTFromDt(dtval + tt/24.0, longi)/24.0;
	return PlanetXYZ(planetno, dd, 8, lst, lat, temp, pres);
}

double __stdcall PlanetXYZ(int planetno, double dd, int xyz, double lst, double lat, double temp, double pres)
{
	double lonecl, latecl, r, v;
	double xh, yh, zh, ra, decl, azi, alti;
	if (planetno != PLUTO)
	{
		double a = elements[planetno].a[0] + dd * elements[planetno].a[1];
		double N = (elements[planetno].N[0] + dd * elements[planetno].N[1]) / RAD2DEG;

		double m = MeanAnomaly(planetno, dd);
		double e = Eccentricity(planetno, dd);
		double ea = EccentricAnomaly(m, e);
		double w = ArgOfPerihelion(planetno, dd);
		double i = Inclination(planetno, dd);

		// location in its own orbit
		double xv = cos(ea) - e;
		double yv = sqrt(1.0 - e *e) * sin(ea);

		// distance and true anomaly from perihelion
		r = a * sqrt(xv*xv + yv*yv);
		v = atan2(yv, xv);

		// calculate heliocentric coords excluding perturbations
		// nb these coords are *geo*centric for the moon
		xh = r * (cos(N) * cos(v + w) - sin(N) * sin(v + w) * cos(i));
		yh = r * (sin(N) * cos(v + w) + cos(N) * sin(v + w) * cos(i));
		zh = r * (sin(v + w) * sin(i));

		r = sqrt(xh*xh + yh*yh + zh*zh);

		// coords in ecliptic plane
		lonecl = atan2(yh, xh);
		latecl = atan2(zh, sqrt(xh*xh + yh*yh));
	}
	else
	{
		// crazy speshul calcs for pluto - nonanalytic, only accurate till 2100
		DoPluto(dd, lonecl, latecl, r);
	}

	if (planetno == MOON)
	{
		//moon perturbations
		double errlon = MoonPerturbations(dd, 1);
		double errlat = MoonPerturbations(dd, 2);
		double errd = MoonPerturbations(dd, 3);
		lonecl = lonecl + errlon;
		latecl = latecl + errlat;
		r = r + errd;
	}
	else if (planetno > URANUS)
	{
		// nothing needed for the very outer planets
	}
	else if (planetno > MARS && planetno < PLUTO)
	{
		// jupiter, saturn and uranus
		// longitude adjustment for all three
		double errlon = GasPerturbations(planetno, dd, 1);
		// latutide correction too for saturn
		double errlat = 0;
		if (planetno == SATURN)
			errlat = GasPerturbations(planetno, dd, 2);
		lonecl = lonecl + errlon;
		latecl = latecl + errlat;
	}
	if (lonecl < 0)
		lonecl = lonecl + TWOPI;
	// convert perturbed ecliptic coords back to heliocentric
	xh = r * cos(lonecl) * cos(latecl);
	yh = r * sin(lonecl) * cos(latecl);
	zh = r * sin(latecl);

	// convert to geocentric coordinates - not needed for moon
	if (planetno > MOON)
	{
		//calculate position of sun
		v = PlanTrueAnomaly(SUN, dd);
		r = PlanDist(SUN, dd);
		double ls = SunLongitude(v, dd);
		double xs = r * cos(ls);
		double ys = r * sin(ls);

		//add to x-y coords of planet; sun is on ecliptic so z adjust is zero
		xh = xh + xs;
		yh = yh + ys;
	}

	// work out the distance from the earth
	double distearth = sqrt(xh*xh + yh*yh + zh*zh);
	if (xyz > 5 && xyz < 10)
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

		azi = AzFromRADec(lst, ra, decl, lat, 0, temp, pres);
		alti = AzFromRADec(lst, ra, decl, lat, 1, temp, pres);

		// correct to topographic coords (surface of earth)
		// mainly relevant for the moon but worth doing for the others
		alti = alti * PI / 180;
		if (planetno == MOON)
			alti = alti - cos(alti) * asin(1 / r);
		else
			alti = alti - (8.794 / 3600.0) / RAD2DEG / r;

		alti = alti * RAD2DEG;
	}
	// results
	switch (xyz)
	{
	case 1:		return xh;
	case 2:		return yh;
	case 3:		return zh;
	case 4: 	return lonecl * RAD2DEG;
	case 5:		return latecl * RAD2DEG;
	case 6:		return ra;
	case 7:		return decl;
	case 8:		return alti;
	case 9:		return azi;
	case 10:	return distearth;
	default:	return 0;
	}
}


