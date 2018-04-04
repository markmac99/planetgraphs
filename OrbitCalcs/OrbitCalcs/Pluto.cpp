#include <Windows.h>
#include <math.h>
#include "OrbitCalcs.h"

// crazy speshul calcs for pluto - nonanalytic, only accurate till 2100
void DoPluto(double dd, double& lonecl, double& latecl, double& r )
{
	double  s = (50.03 + 0.033459652 * dd)/RAD2DEG;
	double p = (238.95 + 0.003968789 * dd)/RAD2DEG;

	lonecl = 238.9508 + 0.00400703 * dd
		- 19.799 * sin(p) + 19.848 * cos(p) 
		+ 0.897 * sin(2 * p) - 4.956 * cos(2 * p) 
		+ 0.61 * sin(3 * p) + 1.211 * cos(3 * p) 
		- 0.341 * sin(4 * p) - 0.19 * cos(4 * p) 
		+ 0.128 * sin(5 * p) - 0.034 * cos(5 * p) 
		- 0.038 * sin(6 * p) + 0.031 * cos(6 * p) 
		+ 0.02 * sin(s - p) - 0.01 * cos(s - p);

	latecl = -3.9082 
		- 5.453 * sin(p) - 14.975 * cos(p)
		+ 3.527 * sin(2 * p) + 1.673 * cos(2 * p)
		- 1.051 * sin(3 * p) + 0.328 * cos(3 * p)
		+ 0.179 * sin(4 * p) - 0.292 * cos(4 * p)
		+ 0.019 * sin(5 * p) + 0.1 * cos(5 * p)
		- 0.031 * sin(6 * p) - 0.026 * cos(6 * p)
		+ 0.011 * cos(s - p);

	r = 40.72
		+ 6.68 * sin(p) + 6.9 * cos(p)
		- 1.18 * sin(2 * p) - 0.03 * cos(2 * p)
		+ 0.15 * sin(3 * p) - 0.14 * cos(3 * p);

	lonecl = lonecl / RAD2DEG;
	latecl = latecl / RAD2DEG;

	return;
}