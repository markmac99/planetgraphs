Attribute VB_Name = "Main"
Global Const pi = 3.141592654
Global Const twopi = 6.283185307
Global Const erad = 6378.14
Global Const au = 149597870.7
Global Const rad2deg = 57.29577951
Global Const HR2DEG = 15.04107

Global Const SUN = -1
Global Const MOON = 0
Global Const MERCURY = 1
Global Const VENUS = 2
Global Const MARS = 3
Global Const JUPITER = 4
Global Const SATURN = 5
Global Const URANUS = 6
Global Const NEPTUNE = 7
Global Const EARTH = 8

Function sunriseset(dt As Date, lat As Double, longi As Double, ros As Integer, h As Double) As Double
    ' sunset can mean many things:
    ' h = 0 degrees: Center of Sun's disk touches a mathematical horizon
    ' h = -0.25 degrees: Sun's upper limb touches a mathematical horizon
    ' h = -0.583 degrees: Center of Sun's disk touches the horizon; atmospheric refraction accounted for
    ' h = -0.833 degrees: Sun's upper limb touches the horizon; atmospheric refraction accounted for
    ' h = -6 degrees: Civil twilight (one can no longer read outside without artificial illumination)
    ' h = -12 degrees: Nautical twilight (navigation using a sea horizon no longer possible)
    ' h = -15 degrees: Amateur astronomical twilight (the sky is dark enough for most astronomical observations)
    ' h = -18 degrees: Astronomical twilight (the sky is completely dark)
        
    sunriseset = RiseSet(-1, dt, lat, longi, ros, h)
End Function
Function RiseSet(planetno As Integer, dt As Date, lat As Double, longi As Double, ros As Integer, ByVal h As Double) As Double
    ' calculate UT when the planet is due south
    
    Dim dd As Double, lst As Double, temp As Double, pres As Double, lha As Double, coslha As Double
    dd = days(Year(dt), Month(dt), Day(dt), 12, 0, 0)
    lst = LocalSiderealTime(Year(dt), Month(dt), Day(dt), 0, 0, 0, lat)
    If planetno > 0 Then
        ut_sis = TimeofTransit(planetno, dt, lat, longi)
    Else
        temp = 10 ' default value
        pres = 1010 ' default value
        tz = 0 ' for GMT
        ra = PlanetXYZ(SUN, dd, 6, lst, lat, temp, pres)
        sunlong = MeanAnomaly(SUN, dd) + ArgOfPerihelion(-1, dd)
        gmst0 = sunlong + pi
        While gmst0 > twopi
            gmst0 = gmst0 - twopi
        Wend
        gmst0 = gmst0 * rad2deg
        ut_sis = (ra - gmst0 - longi)
        If ut_sis < 0 Then ut_sis = ut_sis + 360
        If ut_sis > 360 Then ut_sis = ut_sis - 360
        ut_sis = ut_sis / 15 ' not HR2DEG here, gets the maths wrong...
    End If
       
    h = h / rad2deg
    lati = lat / rad2deg
    decl = PlanetXYZ(planetno, dd, 7, lst, lat, temp, pres)
    decl = decl / rad2deg
    
    ' local hour angle is the angle between local south and sunset or rise
    coslha = (Sin(h) - Sin(decl) * Sin(lati)) / (Cos(decl) * Cos(lati))
    If coslha > 1 Then ' planet never rises
        RiseSet = -99
        Exit Function
    ElseIf coslha < -1 Then ' planet never sets
        If ros = 1 Then
            RiseSet = 0
        Else
            RiseSet = 1
        End If
    Else
        lha = WorksheetFunction.Acos(coslha)
        lha = lha * rad2deg
        If ros = 1 Then
            RiseSet = ut_sis - lha / HR2DEG
        Else
            RiseSet = ut_sis + lha / HR2DEG
        End If
        While RiseSet < 0
            RiseSet = RiseSet + 24
        Wend
        While RiseSet > 24
            RiseSet = RiseSet - 24
        Wend
    End If
End Function


' if the planet is visible, this returns its best altitude, otherwise zero
' two different cases are considered :
' - visual / widefield - planet must be above horizon, sun must be below, planet elong > 20 degs
' - telescopic / imaging - planet must be more than 20 degs above, sun more than 10 degs below horizon

Function IsVisible(planetno As Integer, dt As Date, lat As Double, longi As Double, vis_or_tele As Integer, a_or_t As String) As Double
    
    Dim sunalt As Double, planalt As Double
    If vis_or_tele = 1 Then ' visual
        sunalt = -0.8333   ' civil sunset when centre of sun is 0.8333 degrees below horizon
        planalt = 5         ' planet high enough to clear low obstructions
    Else
        sunalt = -10        ' amateur astronomical dark; use -15 for true dark
        planalt = 25        ' too much distortion below this
    End If
    
    ' effective rise and set times
    sunrise = RiseSet(SUN, dt, lat, longi, 1, sunalt)
    sunset0 = RiseSet(SUN, dt, lat, longi, 2, sunalt)
    
    ' effective rise and set times
    planrise = RiseSet(planetno, dt, lat, longi, 1, planalt)
    planset0 = RiseSet(planetno, dt, lat, longi, 2, planalt)
    
    ' if the planet only meets the criteria after sunrise AND before sunset, return zero
    ' the planet might set after midnight, so adjust the time to cater for that.
    If planset0 < planrise Then planset0 = planset0 + 24
    If planrise > sunrise And planrise < sunset0 And planset0 < sunset0 And planset0 > sunrise Then
        IsVisible = 0
    ElseIf planrise = -99 Or planset = -99 Then
        IsVisible = 0
    Else
        Dim dd As Double, lst As Double, temp As Double, pres As Double
        tt = TimeofTransit(planetno, dt, lat, longi)
        temp = 10
        pres = 1010
        ' if the transit is in daylight, calculate the best height in dark
        ' if transit is before noon, use dawn, otherwise sunset
        If tt > sunrise And tt < 12 Then
            tt = sunrise
        ElseIf tt > 12 And tt < sunset0 Then
            tt = sunset0
        End If
        lst = LSTFromDt(dt + tt / 24, longi) / 24
        dd = AstroDaysFromDt(dt)
        If a_or_t = "a" Then
            talt = PlanetXYZ(planetno, dd, 8, lst, lat, temp, pres)
            IsVisible = talt
        Else
            IsVisible = tt
        End If
    End If
End Function



Function TimeofTransit(planetno As Integer, dt As Date, lat As Double, longi As Double) As Double
' transit time T  = (RA - lat -Me  - PIe)/15 - Tz
' where Tz is the number of hours to be added to local clock to get UT
' lat = Observers Longitude
' Me = mean anomaly of earth
' PIe = Long of Asc Node of Earth + argument of perihelion

    Dim dd As Double, lst As Double, temp As Double, pres As Double
    temp = 10 ' default value
    pres = 1010 ' default value
    tz = 0 ' for GMT
    dd = days(Year(dt), Month(dt), Day(dt), 0, 0, 0)
    lst = LocalSiderealTime(Year(dt), Month(dt), Day(dt), 0, 0, 0, lat)
    ra = PlanetXYZ(planetno, dd, 6, lst, lat, temp, pres)
    ma = MeanAnomaly(EARTH, dd) * 180 / pi
    n = (Sheets("Orbital Elements").Cells(3, 21) + Sheets("Orbital Elements").Cells(3, 22) * dd)
    While n < 0
        n = n + 360
    Wend
    aop = ArgOfPerihelion(8, dd) * 180 / pi
    n = n + aop
    If (n > 360) Then n = n - 360
    tt = (ra - longi - ma - n) / 15 - tz
    While tt < 0
        tt = tt + 24
    Wend
    While tt > 24
        tt = tt - 24
    Wend
    TimeofTransit = tt
End Function


Sub CreateOutputFiles()
Dim planetno As Integer, dt As Date
Dim lati As Double, longi As Double, typ As Integer
Dim alti As Double, ret As String
lati = 51.883
longi = -1.308
typ = 1 ' visual
ret = "a"

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
    
    pname = Sheets("Orbital Elements").Cells(1, 5 + (2 * planetno))
    
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
'    Print #f, "  element: '" & pname & "-" & typ & "',"
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
