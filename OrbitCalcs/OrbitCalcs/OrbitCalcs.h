#pragma once
#define PI			3.141592654
#define TWOPI		6.283185307
#define ERAD		6378.14
#define AU			149597870.7
#define RAD2DEG		57.29577951
#define HR2DEG		15.04107

#define SUN			0
#define MOON		1
#define MERCURY		2
#define VENUS		3
#define MARS		4
#define JUPITER		5
#define SATURN		6
#define URANUS		7
#define NEPTUNE		8
#define EARTH		9
#define PLUTO		10

typedef struct OrbitalElements
{
	char	name[64];
	double	N[2];
	double	incl[2];
	double	omega[2];
	double	a[2];
	double	e[2];
	double	MA[2];
	double	mag[2];
	double	siz;
}_OrbitalElements;

extern struct OrbitalElements elements[10];

void LoadOrbitalElements(OrbitalElements* elements);

int __stdcall GetDateFromDtval(double dtval, int& yy, int& mo, int& dd, int& hh, int& mm, int& ss);

double __stdcall AstroDaysFromDt(double dtval);
double __stdcall days(int yy, int mo, int dd, int hh, int mm, int ss);
double __stdcall JulianDate(int yy, int mo, int dd, int hh, int mm, int ss);
double __stdcall LocalSiderealTime(int yy, int mo, int dd, int hh, int Mm, int ss, double ll);
double __stdcall LSTFromDt(double dtval, double longi);
long __stdcall DtvalToUnixTS(double dtval);

double __stdcall MeanAnomaly(int planetno, double dd);


