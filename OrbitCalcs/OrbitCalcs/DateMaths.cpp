#include "OrbitCalcs.h"
#include <time.h>

double __stdcall AstroDaysFromDt(double dtval)
{
	int yy, mo, dd, hh, mm, ss;
	GetDateFromDtval(dtval, yy, mo, dd, hh, mm, ss);
	return days(yy, mo, dd, hh, mm, ss);
}


double __stdcall days(int yy, int mo, int dd, int hh, int mm, int ss)
{
	double dy;
	dy = 367.00*yy;
	int mv = (mo + 9) / 12;
	int dv = (7*(yy + mv)) / 4;
	dy = dy - dv;
	mv = (275 * mo) / 9;
	dy = dy + mv + dd - 730530;
	dy = dy + (hh + mm / 60.0 + ss / 3600.0) / 24.0;
	return dy;
}

double __stdcall JulianDate(int yy, int mo, int dd, int hh, int mm, int ss)
{
	double JD;

	if(mo < 3)
	{
		yy = yy - 1;
		mo = mo + 12;
	}
	int A = yy / 100;
	int B  = 2 - A + A / 4;
	int C = (int)(365.25 * yy);
	int e = (int)(30.6001 * (mo + 1));
	JD = B + C + dd + e + 1720994.5;
	double dayfrac = hh / 24.0 + mm / (24 * 60.0) + ss / (24 * 3600.0);
	JD = JD + dayfrac;
	return JD;
}

int __stdcall GetDateFromDtval(double dtval, int& yy, int& mo, int& dd, int& hh, int& mm, int& ss )
{
	yy = (int)(dtval / 10000);
	mo = (int)((dtval - yy * 10000) / 100);
	dd = (int)((dtval - yy * 10000 - mo * 100));
	double dyfrac = dtval - yy * 10000 - mo * 100 - dd;
	hh = (int)(dyfrac * 24.0);
	mm = (int)((dyfrac * 24.0 - hh) * 60);
	ss = (int)(((dyfrac * 24.0 - hh) * 60 -mm)*60);
	return (int)(dyfrac*1000);
}

double __stdcall LSTFromDt(double dtval, double longi)
{
	int yy, mo, dd, hh, mm, ss;
	GetDateFromDtval(dtval, yy, mo, dd, hh, mm, ss);
	return LocalSiderealTime(yy, mo, dd, hh, mm, ss, longi);
}
double __stdcall LocalSiderealTime(int yy, int mo, int dd, int hh, int Mm, int ss, double ll)
{
	double JD = JulianDate(yy, mo, dd, 0, 0, 0);
	double t = (JD - 2451545.0) / 36525.0;
	double ut = hh + Mm / 60.0 + ss / 3600.0;
	double t0 = 6.697374558 + (2400.051336 * t) + (0.000025862 * t * t) + (ut * 1.0027379093);
	while (t0 > 24)
		t0 = t0 - 24;
	while (t0 < 0)
		t0 = t0 + 24;

	double lst = t0 + ll / HR2DEG;
	while (lst > 24)
		lst = lst - 24;
	while (lst < 0)
		lst = lst + 24;
	return lst;
}
long __stdcall DtvalToUnixTS(double dtval)
{
	time_t date;
	int yy, mo, dd, hh, mm, ss;
	GetDateFromDtval(dtval, yy, mo, dd, hh, mm, ss);
	struct tm timeinfo;

	timeinfo.tm_year = yy - 1900;
	timeinfo.tm_mon = mo - 1;    //months since January - [0,11]
	timeinfo.tm_mday = dd;          //day of the month - [1,31] 
	timeinfo.tm_hour = hh;         //hours since midnight - [0,23]
	timeinfo.tm_min = mm;          //minutes after the hour - [0,59]
	timeinfo.tm_sec = ss;          //seconds after the minute - [0,59]
	timeinfo.tm_isdst = 0;			// always want GMT
	date = mktime(&timeinfo);

	return (long)date;
}
