// cmdlineTester.cpp : 
//
#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include "orbitcalcs.h"

char* TimeToStr(double t)
{
	static char retval[8] = { 0 };
	int h = (int)t;
	double mins = 60 * (t - h);
	int m = (int)mins;
	double secs = 60 * (mins - m);
	int s = (int)secs;
	sprintf(retval, "%02d:%02d:%02d", h, m, s);
	return retval;
}

int main()
{
	printf("loading data\n"); fflush(stdout);
	int maxloaded = InitialiseStatic(1);
	int pno = 0;
	double dtval = 44141.0; //6th November 2020
	double lat = 51.88;
	double longi = -1.31;
	int ros = 1;
	double h = -0.833;
	double temp = 10;
	double pres = 1010;
	int doiter = 0;

	double rise, set;
	char rt[6] = { 0 }, st[6] = { 0 };

	FILE * f = fopen("c:/temp/sun.csv", "w");
	pno = 0;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	pno = 1;
	dtval = 44141.0; //6th Nov 2020

	f = fopen("c:/temp/moon.csv", "w");
	for(int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/mercury.csv", "w");
	pno = 2;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/venus.csv", "w");
	pno = 3;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/mars.csv", "w");
	pno = 4;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/jupiter.csv", "w");
	pno = 5;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/saturn.csv", "w");
	pno = 6;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/uranus.csv", "w");
	pno = 7;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/neptune.csv", "w");
	pno = 8;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 2;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

	f = fopen("c:/temp/pluto.csv", "w");
	pno = 9;
	dtval = 44141.0; //6th Nov 2020
	for (int i = 0; i < 61; i++)
	{
		double dv = dtval + i;
		doiter = 1;
		rise = RiseSet(pno, dv, lat, longi, 1, h, temp, pres, doiter);
		set = RiseSet(pno, dv, lat, longi, 0, h, temp, pres, doiter);
		strncpy(rt, TimeToStr(rise), 5);
		strncpy(st, TimeToStr(set), 5);
		printf("%.0f, %s, %s\n", dv, rt, st);
		fprintf(f, "%.0f,%s,%s\n", dv, rt, st);
	}
	fclose(f);

}

