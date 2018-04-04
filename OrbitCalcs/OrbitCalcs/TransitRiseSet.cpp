#include <Windows.h>
#include <math.h>
#include "OrbitCalcs.h"




double __stdcall  SunRiseSet(double dtval, double lat, double longi, int ros, double h)
{
	// sunset can mean many things:
	// h = 0 degrees: Center of Sun's disk touches a mathematical horizon
	// h = -0.25 degrees: Sun's upper limb touches a mathematical horizon
	// h = -0.583 degrees: Center of Sun's disk touches the horizon; atmospheric refraction accounted for
	// h = -0.833 degrees: Sun's upper limb touches the horizon; atmospheric refraction accounted for
	// h = -6 degrees: Civil twilight (one can no longer read outside without artificial illumination)
	// h = -12 degrees: Nautical twilight (navigation using a sea horizon no longer possible)
	// h = -15 degrees: Amateur astronomical twilight (the sky is dark enough for most astronomical observations)
	// h = -18 degrees: Astronomical twilight (the sky is completely dark)

	return RiseSet(SUN, dtval, lat, longi, ros, h);
}

double __stdcall RiseSet(int planetno, double dtval, double  lat, double longi, int ros, double h)
{
	// calculate UT when the planet is due south

	double dd, lst, temp, pres, lha, coslha;
	int yy, mo, dy, hh, mm, ss, tz;
	double ra, sunlong, gmst0, ut_sis;
	double lati, decl;

	GetDateFromDtval(dtval, yy, mo, dy, hh, mm, ss);
	dd = days(yy, mo, dy, 12, 0, 0);
	lst = LocalSiderealTime(yy, mo, dy, 0, 0, 0, lat);
	temp = 10; // default value
	pres = 1010; // default value
	tz = 0; // for GMT

	if(planetno > MOON)
		double ut_sis = TimeofTransit(planetno, dtval, lat, longi);
	else
	{
		ra = PlanetXYZ(SUN, dd, 6, lst, lat, temp, pres);
		sunlong = MeanAnomaly(SUN, dd) + ArgOfPerihelion(SUN, dd);
		gmst0 = sunlong + PI;
		while (gmst0 > TWOPI)
			gmst0 = gmst0 - TWOPI;

		gmst0 = gmst0 * RAD2DEG;
		ut_sis = (ra - gmst0 - longi);
		if (ut_sis < 0)
			ut_sis = ut_sis + 360;
		if (ut_sis > 360)
			ut_sis = ut_sis - 360;
		ut_sis = ut_sis / 15.0; //  not HR2DEG here, gets the maths wrong...
	}

	h = h / RAD2DEG;
	lati = lat / RAD2DEG;
	decl = PlanetXYZ(planetno, dd, 7, lst, lat, temp, pres);
	decl = decl / RAD2DEG;

	// local hour angle is the angle between local south and sunset or rise
	coslha = (sin(h) - sin(decl) * sin(lati)) / (cos(decl) * cos(lati));

	if (coslha > 1) // planet never rises
		return -99;
	else if (coslha < -1) // planet never sets
	{
		if (ros == 1)
			return 0;
		else 
			return 1;
	}
	else // ok planet rises and sets
	{
		double rs;
		lha = acos(coslha);
		lha = lha * RAD2DEG;
		if (ros == 1)
			rs = ut_sis - lha / HR2DEG;
		else
			rs = ut_sis + lha / HR2DEG;
		
		while (rs < 0)
			rs = rs + 24;

		while (rs > 24)
			rs = rs - 24;
		return rs;
	}
}

double __stdcall TimeofTransit(int planetno, double dtval, double lat, double longi)
{
	// transit time T  = (RA - lat -Me  - PIe)/15 - Tz
	// where Tz is the number of hours to be added to local clock to get UT
	// lat = Observers Longitude
	// Me = mean anomaly of earth
	// PIe = Long of Asc Node of Earth + argument of perihelion

	double dd, lst, temp, pres;
	int yy, mo, dy, hh, mm, ss, tz;
	double ra, MA, N, aop, tt;

	temp = 10; // default value
	pres = 1010; // default value
	tz = 0; // for GMT

	GetDateFromDtval(dtval, yy, mo, dy, hh, mm, ss);
	dd = days(yy, mo, dy, 0, 0, 0);
	lst = LocalSiderealTime(yy, mo, dy, 0, 0, 0, lat);
	ra = PlanetXYZ(planetno, dd, 6, lst, lat, temp, pres);
	MA = MeanAnomaly(EARTH, dd) * 180 / PI;
	N = elements[EARTH].N[0] + elements[EARTH].N[1] * dd;

	while (N < 0)
		N = N + 360;

	aop = ArgOfPerihelion(EARTH, dd) * 180 / PI;
	N = N + aop;
	if (N > 360)
		N = N - 360;
	tt = (ra - longi - MA - N) / 15 - tz;
	while (tt < 0)
		tt = tt + 24;
	while(tt > 24)
		tt = tt - 24;

	return tt;
}


	// if the planet is visible, this returns its best altitude, otherwise zero
	// two different cases are considered :
	// - visual / widefield - planet must be above horizon, sun must be below, planet elong > 20 degs
	// - telescopic / imaging - planet must be more than 20 degs above, sun more than 10 degs below horizon

double __stdcall IsVisible(int planetno, double dtval, double lat, double longi, int vis_or_tele, int a_or_t)
{
	int yy, mo, dy, hh, mm, ss;
	double talt, tt;
	double sunalt, planalt, sunrise, sunset0, planrise, planset0;
	double dd, lst, temp, pres;

	GetDateFromDtval(dtval, yy, mo, dy, hh, mm, ss);
	if (vis_or_tele = 1)
	{
		//visual
		sunalt = -0.8333;   // civil sunset when centre of sun is 0.8333 degrees below horizon
		planalt = 5;         // planet high enough to clear low obstructions
	}
	else
	{
		sunalt = -10;        // amateur astronomical dark; use -15 for true dark
		planalt = 25;        // too much distortion below this
	}

	// effective rise and set times
	sunrise = RiseSet(SUN, dtval, lat, longi, 1, sunalt);
	sunset0 = RiseSet(SUN, dtval, lat, longi, 2, sunalt);

	// effective rise and set times
	planrise = RiseSet(planetno, dtval, lat, longi, 1, planalt);
	planset0 = RiseSet(planetno, dtval, lat, longi, 2, planalt);

	// if the planet only meets the criteria after sunrise AND before sunset, return zero
	// the planet might set after midnight, so adjust the time to cater for that.
	if (planset0 < planrise)
		planset0 = planset0 + 24;

	if (planrise > sunrise && planrise < sunset0 && planset0 < sunset0 && planset0 > sunrise)
	{
		return 0;
	}
	else if (planrise == -99 || planset0 == -99)
		return 0;
	else
	{
		tt = TimeofTransit(planetno, dtval, lat, longi);
		temp = 10;
		pres = 1010;
		// if the transit is in daylight, calculate the best height in dark
		// if transit is before noon, use dawn, otherwise sunset
		if (tt > sunrise && tt < sunset0)
		{
			double talt1, talt2;
			// get altitude at sunrise and sunset to see which is best
			GetDateFromDtval(dtval + sunrise / 24, yy, mo, dy, hh, mm, ss);
			dd = days(yy, mo, dy, hh, mm, ss);
			lst = LocalSiderealTime(yy, mo, dy, hh, mm, ss, longi) / 24;
			talt1 = PlanetXYZ(planetno, dd, 8, lst, lat, temp, pres);

			GetDateFromDtval(dtval + sunset0 / 24, yy, mo, dy, hh, mm, ss);
			dd = days(yy, mo, dy, hh, mm, ss);
			lst = LocalSiderealTime(yy, mo, dy, hh, mm, ss, longi) / 24;
			talt2 = PlanetXYZ(planetno, dd, 8, lst, lat, temp, pres);

			talt = talt2;
			tt = sunset0;
			if (talt1 > talt2)
			{
				talt = talt1;
				tt = sunrise;
			}
		}
		else
		{
			GetDateFromDtval(dtval + tt / 24, yy, mo, dy, hh, mm, ss);
			dd = days(yy, mo, dy, hh, mm, ss);
			lst = LocalSiderealTime(yy, mo, dy, hh, mm, ss, longi) / 24;
			talt = PlanetXYZ(planetno, dd, 8, lst, lat, temp, pres);
		}
		if (a_or_t == 1)
			return talt;
		else
			return tt / 24;
	}
}
