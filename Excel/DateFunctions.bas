Attribute VB_Name = "DateFunctions"

Function vbAstroDaysFromDt(dt As Date)
    vbAstroDaysFromDt = vbDays(Year(dt), Month(dt), Day(dt), Hour(dt), Minute(dt), Second(dt))
End Function

Function vbDays(yy As Integer, mo As Integer, dd As Integer, hh As Integer, mm As Integer, ss As Integer)
    Dim dy As Double
    dy = 367# * yy
    dy = dy - 7 * (yy + (mo + 9) \ 12) \ 4
    dy = dy + 275 * mo \ 9 + dd - 730530
    vbDays = dy + (hh + mm / 60 + ss / 3600) / 24
End Function

Function vbJulianDate(yy, mo, dd, hh, mm, ss)
    If mo < 3 Then
        yy = yy - 1
        mo = mo + 12
    End If
    a = yy \ 100
    B = 2 - a + a \ 4
    C = Int(365.25 * yy)
    e = Int(30.6001 * (mo + 1))
    JD = B + C + dd + e + 1720994.5
    JD = JD + hh / 24 + mm / 24 / 60 + ss / 24 / 3600
    vbJulianDate = JD
End Function

Function vbLSTFromDt(dt As Date, longi As Double)
    vbLSTFromDt = vbLocalSiderealTime(Year(dt), Month(dt), Day(dt), Hour(dt), Minute(dt), Second(dt), longi)
End Function
Function vbLocalSiderealTime(yy, mo, dd, hh, mm, ss, ll)
    JD = vbJulianDate(yy, mo, dd, 0, 0, 0)
    t = (JD - 2451545#) / 36525#
    ut = hh + mm / 60 + ss / 3600
    t0 = 6.697374558 + (2400.051336 * t) + (0.000025862 * t ^ 2) + (ut * 1.0027379093)
    While t0 > 24
        t0 = t0 - 24
    Wend
    While t0 < 0
        t0 = t0 + 24
    Wend
    lst = t0 + ll / HR2DEG
    While lst > 24
        lst = lst - 24
    Wend
    While lst < 0
        lst = lst + 24
    Wend
    vbLocalSiderealTime = lst
End Function

Function vbtoUnixTS(dt As Date)
    ' unix time is number of seconds since 1/1/1970
    vbtoUnixTS = DateDiff("s", "1/1/1970", dt) * 1000
End Function


