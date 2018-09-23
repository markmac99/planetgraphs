#include <math.h>
#include "OrbitCalcs.h"

double __stdcall MoonPerturbations(double dd, int latlongpos)
{
	double Ms, Mm, Nm, ws, wm, ll;

	Ms = MeanAnomaly(SUN, dd);
	Mm = MeanAnomaly(MOON, dd);
	ws = ArgOfPerihelion(SUN, dd);
	wm = ArgOfPerihelion(MOON, dd);
	Nm = elements[MOON].N[0] + elements[MOON].N[1] * dd;

	Nm = Nm / RAD2DEG;
	double ls = Ms + ws;
	double Lm = Mm + wm + Nm;
	double Dm = Lm - ls;
	double f = Lm - Nm;
	if (latlongpos == 1)
	{
		// correcting ecliptic longitude
		ll = -1.274 * sin(Mm - 2 * Dm);
		ll = ll + 0.658 * sin(2 * Dm);
		ll = ll - 0.186 * sin(Ms);
		ll = ll - 0.059 * sin(2 * Mm - 2 * Dm);
		ll = ll - 0.057 * sin(Mm - 2 * Dm + Ms);
		ll = ll + 0.053 * sin(Mm + 2 * Dm);
		ll = ll + 0.046 * sin(2 * Dm - Ms);
		ll = ll + 0.041 * sin(Mm - Ms);
		ll = ll - 0.035 * sin(Dm);
		ll = ll - 0.031 * sin(Mm + Ms);
		ll = ll - 0.015 * sin(2 * f - 2 * Dm);
		ll = ll + 0.011 * sin(Mm - 4 * Dm);
		ll = ll / RAD2DEG;
	}
	else if (latlongpos == 2)
	{
		//latitude
		ll = -0.173 * sin(f - 2 * Dm);
		ll = ll - 0.055 * sin(Mm - f - 2 * Dm);
		ll = ll - 0.046 * sin(Mm + f - 2 * Dm);
		ll = ll + 0.033 * sin(f + 2 * Dm);
		ll = ll + 0.017 * sin(2 * Mm + f);
		ll = ll / RAD2DEG;
	}
	else
	{
		//earth-moon distance
		ll = -0.58 * cos(Mm - 2 * Dm) - 0.46 * cos(2 * Dm);
	}
	return ll;
}

double __stdcall GasPerturbations(int planetno, double dd, int latlongpos)
{
	double gp;

	double Mj = MeanAnomaly(4, dd);
	double Ms = MeanAnomaly(5, dd);
	double Mu = MeanAnomaly(6, dd);

	if (planetno == JUPITER)
	{
		//jupiter
		gp = -0.332 * sin(2 * Mj - 5 * Ms - 67.6 / RAD2DEG);
		gp = gp - 0.056 * sin(2 * Mj - 2 * Ms + 21 / RAD2DEG);
		gp = gp + 0.042 * sin(3 * Mj - 5 * Ms + 21 / RAD2DEG);
		gp = gp - 0.036 * sin(Mj - 2 * Ms);
		gp = gp + 0.022 * cos(Mj - Ms);
		gp = gp + 0.023 * sin(2 * Mj - 3 * Ms + 52 / RAD2DEG);
		gp = gp - 0.016 * sin(Mj - 5 * Ms - 69 / RAD2DEG);
	}
	else if (planetno == SATURN)
	{
		//saturn
		if (latlongpos == 1)
		{
			gp = 0.812 * sin(2 * Mj - 5 * Ms - 67.6 / RAD2DEG);
			gp = gp - 0.229 * cos(2 * Mj - 4 * Ms - 2 / RAD2DEG);
			gp = gp + 0.119 * sin(Mj - 2 * Ms - 3 / RAD2DEG);
			gp = gp + 0.046 * sin(2 * Mj - 6 * Ms - 69 / RAD2DEG);
			gp = gp + 0.014 * sin(Mj - 3 * Ms + 32 / RAD2DEG);
		}
		else
		{
			gp = 0.02 * cos(2 * Mj - 4 * Ms - 2 / RAD2DEG);
			gp = gp + 0.018 * sin(2 * Mj - 6 * Ms - 49 / RAD2DEG);
		}
	}
	else if (planetno == URANUS)
	{
		//uranus
		gp = 0.04 * sin(Ms - 2 * Mu + 6 / RAD2DEG);
		gp = gp + 0.035 * sin(Ms - 3 * Mu + 33 / RAD2DEG);
		gp = gp - 0.015 * sin(Mj - Mu + 20 / RAD2DEG);
	}
	else
	{
		gp = 0;
	}
	return gp / RAD2DEG;
}




