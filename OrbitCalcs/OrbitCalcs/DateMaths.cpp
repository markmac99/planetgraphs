#define _CRT_SECURE_NO_WARNINGS 1

#include "OrbitCalcs.h"
#include <time.h>
#include <stdio.h>

// model taken from http://www.stjarnhimlen.se/comp/ppcomp.html

char* TimeToStr(double t) 
{
	static char retval[9] = { 0 };
	int h = (int)t;
	double mins = 60 * (t - h);
	int m = (int)mins;
	double secs = 60 * (mins - m);
	int s = (int)secs;
	sprintf(retval, "%02d:%02d:%02d", h, m, s);
	return retval;
}
char* TimeToHHMM(double t)
{
	static char retval[8] = { 0 };
	int h = (int)t;
	double mins = 60 * (t - h);
	int m = (int)mins;
	sprintf(retval, "%02d:%02d", h, m);
	return retval;
}

char* AngleToDDMM(double t)
{
	static char retval[12] = { 0 };

	// need to handle -ve angles carefully
	int sign = 1;
	if (t < 0) 
	{ 
		// temporarily remove the sign; we'll add it back later
		sign = -1;
		t *= sign;
	}
	int h = (int)t;
	double mins = 60 * (t - h);
	int m = (int)mins;

	// put the sign back
	h *= sign; 
	sprintf(retval, "%+3.3d&deg;%2.2d", h, m);
	return retval;
}


double __stdcall AstroDaysFromDt(double dtval)
{
	int yy, mo, dd, hh, mm, ss;
//	FILE* f = fopen("c:/temp/log.txt", "w");
//	fprintf(f, "%f\n", dtval);
	GetDateFromDtval(dtval, yy, mo, dd, hh, mm, ss);
//	fprintf(f, "%d %d %d %d %d %d\n", yy, mo, dd, hh, mm, ss);
//	fclose(f);
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
	int e = (int)(30.6001 * ((double)mo + 1));
	JD = (double)B + (double)C + (double)dd + (double)e + 1720994.5;
	double dayfrac = hh / 24.0 + mm / (24 * 60.0) + ss / (24 * 3600.0);
	JD = JD + dayfrac;
	return JD;
}

double __stdcall GetDtvalFromDate(int yy, int mo, int dd, int hh, int mm, int ss)
{
	double dtval;
	struct tm tmval;

	tmval.tm_year = yy-1900;
	tmval.tm_mon =mo-1;
	tmval.tm_mday = dd;
	tmval.tm_hour =hh;
	tmval.tm_min =mm;
	tmval.tm_sec=ss;
#ifndef _WIN32
	time_t unixdt;
	tmval.tm_isdst=0;
	unixdt = mktime(&tmval);
#else
	__time64_t unixdt;
	unixdt = _mkgmtime64(&tmval);
#endif
	dtval = unixdt / 86400.0;
	dtval += 25569;

	return dtval;
}

double __stdcall GetDateFromDtval(double dtval, int& yy, int& mo, int& dd, int& hh, int& mm, int& ss )
{
	time_t unixdt= (time_t)((dtval - 25569.0) * 86400.0);
	struct tm tmval;
#ifndef _WIN32
	gmtime_r(&unixdt, &tmval);
#else
	errno_t err = _gmtime64_s(&tmval, &unixdt);
#endif
	yy = tmval.tm_year+1900;
	mo = tmval.tm_mon+1;
	dd = tmval.tm_mday;
	hh = tmval.tm_hour;
	mm = tmval.tm_min;
	ss = tmval.tm_sec;

	return (double)(unixdt);
//	return dtval;
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

long __stdcall AstroDtToUnixTS(double dd)
{
	return  (long)((dd +36525-25569.0) * 86400.0);
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
