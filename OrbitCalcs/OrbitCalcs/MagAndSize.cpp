#include "OrbitCalcs.h"
#include <math.h>

// model taken from http://www.stjarnhimlen.se/comp/ppcomp.html

double __stdcall PhaseAngle(long planetno, double dd)
{
	return PhaseOrElong(planetno, dd, 0);
}

double __stdcall Phase(long planetno, double dd)
{
	return PhaseOrElong(planetno, dd, 1);
}

double __stdcall Elongation(long planetno, double dd)
{
	return PhaseOrElong(planetno, dd, 2);
}

// simpler formula for planets than for the moon
double __stdcall PhaseOrElong(long planetno, double dd, short poe)
{
	return PhaseOrElongGeneral(planetno, dd, poe, 0, 0);
}

double __stdcall PhaseOrElongGeneral(long planetno, double dd, short poe, double lst, double lat)
{
	double phangle, elong, phase;
	if (planetno == MOON)
	{
		double mlong = PlanetXYZ(MOON, dd, 4, lst, lat, 10, 1010);
		double mlat = PlanetXYZ(MOON, dd, 5, lst, lat, 10, 1010);
		double slong = PlanetXYZ(SUN, dd, 4, lst, lat, 10, 1010);
		elong = acos(cos((slong - mlong) / RAD2DEG) * cos(mlat / RAD2DEG));
		phangle = PI - elong;
		phase = (1.0 + cos(phangle)) / 2.0;
	}
	else
	{
		double r = PlanDist(planetno, dd);   //heliocentric distance of planet
		double r1 = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0); // geocentric distance of planet
		double s = PlanDist(SUN, dd); //distance of sun from earth
		double argv = 0;

		// phase angle is given by acos(argv)
		argv = (r * r + r1 * r1 - s * s) / (2 * r * r1);
		phangle = acos(argv);

		if (phangle > 180) phangle = 360.0 - phangle;
		phase = ((1.0 + argv) / 2.0);

		// elongation is given by 
		argv = (s * s + r1 * r1 - r * r) / (2 * s * r1); 
		elong = (acos(argv) * RAD2DEG);
	}
	if (poe == 0) return phangle;
	if (poe == 1) return phase;
	else return elong;
}


// brightness is given by
// m = M0 * 5 log10(r*R) +M1 FV + M2 FV^n
// where r is the distance from the planet to the Sun
// R is the distance from the planet to the Earth
// FV is the phase, M1,2,3 are constants and n is an exponent that varies by planet

double __stdcall VisualMagnitude(long planetno, double dd)
{
	double magBase = -1, magPhaseFactor = 0;
	magBase = elements[planetno].mag[0];
	magPhaseFactor = elements[planetno].mag[1];

	double magNonlinearFactor = elements[planetno].mag[2];
	double magNonlinearExponent = elements[planetno].mag[3];

	double distearth = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0);
	double distsun = PlanDist(planetno, dd);

	double phase = PhaseAngle(planetno, dd)*180/PI;
	if (planetno == MOON)
	{
		distearth = distearth * ERAD / AU;
		distsun = PlanDist(SUN, dd);
	}
	double mag = magBase + (5 * log10(distsun * distearth));
	mag += (magPhaseFactor * phase);
	if (magNonlinearExponent > 0)
		mag += magNonlinearFactor * pow(phase,magNonlinearExponent);

	if (planetno == SATURN)
		mag += SaturnRingMag(dd);

	return mag;
}

// for saturn there's an addition due to the rings:
// ring_magn = -2.6 * sin(abs(B)) + 1.2 * (sin(B))**2
// where B = tilt of the rings:
// B = asin( sin(las) * cos(ir) - cos(las) * sin(ir) * sin(los-Nr) )
// where las, los are the ecliptic lat and long of Saturn
// and ir = 28.06_deg
// Nr = 169.51_deg + 3.82E-5_deg * dd

double __stdcall SaturnRingMag(double dd)
{
	double ring_mag = 0;

	double las= PlanetXYZ(SATURN, dd, 5, 0, 0, 0, 0)/RAD2DEG; 
	double los= PlanetXYZ(SATURN, dd, 4, 0, 0, 0, 0)/RAD2DEG;

	double ir = 28.06/RAD2DEG;
	double Nr = (169.51 + 0.0000382*dd)/RAD2DEG;

	double B = asin(sin(las)*cos(ir) - cos(las)*sin(ir)*sin(los - Nr));
	ring_mag = -2.6 * sin(fabs(B)) + 1.2 * sin(B)*sin(B);
	return ring_mag;
}

double __stdcall ApparentSize(long planetno, double dd)
{
	double d0 = 0.0;
	double r = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0);
	if (planetno == PLUTO)
		d0 = 3.4;
	else
		d0 = elements[planetno].siz;

	double Fact = 1.0;
	if (planetno == MOON)
		Fact = 60;
	return (d0 * Fact / r);
}
