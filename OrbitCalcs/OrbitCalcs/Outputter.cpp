#include "OrbitCalcs.h"

// shaddap with the fopen warnings
#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include <math.h>
#include <stdio.h>


void SaveOrbitalElements(void)
{
	FILE* f = fopen("c:/temp/orbitalelements.csv", "w");
	for (int i = 0; i < 10; i++)
	{
		fprintf(f, "%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
			elements[i].name, 
			elements[i].N[0], elements[i].N[1],
			elements[i].incl[0], elements[i].incl[1],
			elements[i].omega[0], elements[i].omega[1],
			elements[i].a[0], elements[i].a[1],
			elements[i].e[0], elements[i].e[1],
			elements[i].MA[0], elements[i].MA[1],
			elements[i].mag[0], elements[i].mag[1],
			elements[i].siz);
	}
	fclose(f);
}

#if 0
Sub CreateOutputFiles()
Dim planetno As Integer, dt As Date
Dim lati As Double, longi As Double, typ As Integer
Dim alti As Double, ret As String
lati = 51.883
longi = -1.308
typ = 1 ' visual
ret = "a"

Call LoadOrbitalElements
For planetno = MERCURY To NEPTUNE
f1 = FreeFile
f2 = f1 + 1
f3 = f1 + 2
f4 = f1 + 3

If planetno < 3 Then
	intvl = 7
	maxiters = 104 ' two years of weekly dat
	Else
	intvl = 31
	maxiters = 71 ' six years roughly
	End If
	dt = DateSerial(2018, 1, 1)

	pname = elements(planetno).name

	Open "c:\temp\" & pname & "Altitude.js" For Output As f1
	Open "c:\temp\" & pname & "Magnitude.js" For Output As f2
	Open "c:\temp\" & pname & "Size.js" For Output As f3
	Open "c:\temp\" & pname & "Data.csv" For Output As f4

	Call addHeader(f1, pname, "Altitude")
	Call addHeader(f2, pname, "Magnitude")
	Call addHeader(f3, pname, "Size")
	minsiz = 100
	maxsiz = 0
	maxbri = -20
	minbri = 200
	minalt = 0
	maxalt = 0
	Print #f4, "Date,Altitude,Magnitude,size"

	For i = 1 To maxiters
	alti = IsVisible(planetno, dt + intvl * i, lati, longi, typ, "a")
	best = IsVisible(planetno, dt + intvl * i, lati, longi, typ, "t") / 24
	brig = VisualMagnitude(planetno, AstroDaysFromDt(dt + intvl * i))
	siz = ApparentSize(planetno, AstroDaysFromDt(dt + intvl * i))

	If siz > maxsiz Then maxsiz = siz
	If siz < minsiz Then minsiz = siz
	If brig > maxbri Then maxbri = brig
	If brig < minbri Then minbri = brig
	If alti > maxalt Then maxalt = alti
	tmst = toUnixTS(dt + intvl * i)
	tms2 = toUnixTS(dt + intvl * i + best)

	If i < maxiters Then
	Print #f1, "{time: " & tms2 & ", altitude: " & Round(alti, 2) & "},"
	Print #f2, "{time: " & tms2 & ", magnitude: " & Round(brig, 2) & "},"
	Print #f3, "{time: " & tms2 & ", size: " & Round(siz, 2) & "},"
	Else
	Print #f1, "{time: " & tms2 & ", altitude: " & Round(alti, 2) & "}],"
	Print #f2, "{time: " & tms2 & ", magnitude: " & Round(brig, 2) & "}],"
	Print #f3, "{time: " & tms2 & ", size: " & Round(siz, 2) & "}],"
	End If
	Print #f4, Format(dt + intvl * i + best, "yyyy-mm-dd hh:mm:ss") & "," & alti & "," & brig & "," & siz
	Next i
	If maxbri < 0 Then
	maxbri = Application.RoundDown(Abs(maxbri), 0)
	maxbri = maxbri * -1
	Else
	maxbri = Application.RoundUp(maxbri, 0)
	End If
	If minbri < 0 Then
	minbri = Application.RoundUp(Abs(minbri), 0)
	minbri = minbri * -1
	Else
	minbri = Application.RoundDown(minbri, 0)
	End If
	maxsiz = Application.RoundUp((maxsiz * 12) \ 10, 2)
	minsiz = Application.RoundDown((minsiz * 12) \ 10, 2)
	maxalt = Application.RoundUp((maxalt * 12) \ 10, 2)

	Call addFooter(f1, "\xB0", "Altitude", minalt, maxalt)
	Close f1
	Call addFooter(f2, " mag", "Magnitude", maxbri, minbri)
	Close f2
	Call addFooter(f3, " as", "Size", minsiz, maxsiz)
	Close f3
	Close f4
	Next planetno
	End Sub

	Sub addHeader(f, pname, typ)
	Print #f, "$(function() {"
	Print #f, "Morris.Line({"
	'    Print #f, "  element: '" & pname & " - " & typ & "',"
	Print #f, "  element: 'planet-" & LCase(typ) & "',"
	Print #f, "data: [ "
	End Sub

	Sub addFooter(f, unt, typ, minv, maxv)
	Print #f, "        xkey: 'time',"
	Print #f, "        ykeys: ['" & LCase(typ) & "'],"
	Print #f, "        labels: ['" & typ & "'],"
	Print #f, "        hideHover: 'auto',"
	Print #f, "        xLabelAngle: 45,"
	Print #f, "        ymax: " & Round(maxv, 2) & " ,"
	Print #f, "        ymin: " & Round(minv, 2) & ","
	Print #f, "        postUnits: '" & unt & "',"
	Print #f, "        resize: true"
	Print #f, "    });"
	Print #f, "});"
	End Sub

#endif
