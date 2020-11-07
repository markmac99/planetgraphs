#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <string>
#include <algorithm>
#include "OrbitCalcs.h"

// model taken from http://www.stjarnhimlen.se/comp/ppcomp.html

char szPath[512]; // path to the DLL and asteroid and comet data files
char szOutputPath[512]; // path to the exported datafiles
int maxloaded = 0; // number of objects loaded, including the planets sun and moon

struct OrbitalElements elements[NUMELEMENTS];

double __stdcall GetOrbitalParam(int planetno, int what)
{
	planetno = (planetno > maxloaded ? maxloaded : planetno);

	if (what == 'N' || what =='n')
		return LongOfAscNode(planetno, 0);
	if (what == 'i' || what =='I')
		return Inclination(planetno, 0);
	if (what == 'w' || what =='W')
		return ArgOfPerihelion(planetno, 0);
	if (what == 'a' || what == 'A')
		return elements[planetno].a[0] *(planetno == MOON ? ERAD : AU);
	if (what == 'e' || what=='E')
		return Eccentricity(planetno,0);
	if (what == 'M' || what =='m')
		return MeanAnomaly(planetno, 0);
	if (what == 'G' || what =='g')
		return elements[planetno].mag[0];
	if (what == 's' || what == 'S')
		return elements[planetno].siz *(planetno == MOON ? ERAD*60 : AU) / (3600.0 * RAD2DEG);
	if (what == 'T' ||  what =='t')
		return (double)(elements[planetno].epoch[0] * 10000+ elements[planetno].epoch[1] * 10 + elements[planetno].epoch[2]);
	return 0.0;
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
	if (e0==0) return 0;

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
//	if (planetno > maxloaded) return 0;
	double tt = TimeofTransit(planetno, dtval, lat, longi);
	double dv = dtval + tt / 24.0;
	double dd = AstroDaysFromDt(dv);
	double lst = LSTFromDt(dtval + tt/24.0, longi)/24.0;
	return PlanetXYZ(planetno, dd, 8, lst, lat, temp, pres);
}

double __stdcall PlanetXYZ(int planetno, double dd, int xyz, double lst, double lat, double temp, double pres)
{
	double lonecl, latecl, r, v;
	double xh, yh, zh, ra, decl, azi, alti;

//	if (planetno == 11)
//		printf("%f %d %f %f %f %f \n", dd, xyz, lst, lat, temp, pres);

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

//		if (planetno == 11)
//			printf("%f %f %f %f %f %f %f %f \n", e, w, a, N, m, xh, yh, zh);

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
	//printf("%.2f %.2f %.2f\n", dd, lst, lat);
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
		alti = alti /RAD2DEG;
		if (planetno == MOON)
			alti = alti - cos(alti) * asin(1 / distearth);
		else
			alti = alti - (8.794 / 3600.0) / RAD2DEG / distearth;

		alti = alti * RAD2DEG;

		// now correct RA and DEC to topographic coords
		double par = (planetno==MOON? asin(1 / distearth): (8.794 / 3600)/distearth/RAD2DEG); // parallax of planet
		double gclat = lat - 0.1924 * sin(2 * lat/RAD2DEG); //geocentric latitude of observer allowing for earth being oblate
		double rho = 0.99833 + 0.00167 * cos(2 * lat/ RAD2DEG); // distance of observer from centre of earth
		double HA = (lst * 360 - ra)/RAD2DEG; // planet's hour angle in radians
		double g = atan(tan(gclat/RAD2DEG) / cos(HA)); // auxiliary angle 
		double racorr, declcorr;

		// formula breaks down close to the celestial poles though planets should never be there
		if (fabs(decl) > 89.9999)
			racorr = 0;
		else 
			racorr = (par * rho * cos(gclat / RAD2DEG) * sin(HA) / cos(decl / RAD2DEG))*RAD2DEG;

		if (fabs(gclat) < 0.00001) // breaks down near earth's equator
			declcorr = (par * rho * sin(-decl / RAD2DEG) * cos(HA)) * RAD2DEG;
		else
			declcorr = (par * rho * sin(gclat / RAD2DEG) * sin(g - decl / RAD2DEG) / sin(g))*RAD2DEG;
		ra -= racorr ;
		decl -= declcorr;
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


void trim(char *str)
{
	int i = 0;
	char outstr[64] = { 0 };
	while (str[i] == ' ')i++;
	strcpy(outstr, str + i);
	i = (int)strlen(outstr) - 1;
	while (outstr[i] == ' ')i--;
	outstr[i + 1] = 0;
	strcpy(str, outstr);
}

void cleanup_name(char* str)
{
	size_t i, j;
	char outstr[64] = { 0 };
	trim(str);
	std::string s = str;
	std::replace(s.begin(),s.end(),' ','-');
	std::replace(s.begin(),s.end(),'/','-');
	strcpy(str, s.c_str());
	for (i = 0, j=0; i < strlen(str); i++)
	{
			if (str[i]=='(' || str[i]==')')
					i++;
			outstr[j++] = str[i];
	}
	trim(outstr);
	strcpy(str, outstr);
}

char* fmt_hours(double ra)
{
	static char ret[6] = { 0 };
	ra /= 15.0;
	int hr = (int)ra;
	int min = (int)(60 * (ra - hr));
	sprintf(ret, "%02d:%02d", hr, min);
	return ret;
}
