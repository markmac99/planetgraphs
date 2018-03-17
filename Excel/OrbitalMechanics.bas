Attribute VB_Name = "OrbitalMechanics"

Function MeanAnomaly(planetno As Integer, d As Double) As Double

    k = Sheets("Orbital Elements").Cells(8, 5 + (2 * planetno))
    p = Sheets("Orbital Elements").Cells(8, 6 + (2 * planetno))
    
    MeanAnomaly = (k + p * d) * pi / 180
    While MeanAnomaly > twopi
        MeanAnomaly = MeanAnomaly - twopi
    Wend
    While MeanAnomaly < 0
        MeanAnomaly = MeanAnomaly + twopi
    Wend
End Function

Function eccentricity(planetno As Integer, d As Double) As Double
    e1 = Sheets("Orbital Elements").Cells(7, 5 + (2 * planetno))
    e2 = Sheets("Orbital Elements").Cells(7, 6 + (2 * planetno))
    eccentricity = e1 + e2 * d
End Function

Function inclination(planetno As Integer, d As Double) As Double
    e1 = Sheets("Orbital Elements").Cells(4, 5 + (2 * planetno))
    e2 = Sheets("Orbital Elements").Cells(4, 6 + (2 * planetno))
    inclination = (e1 + e2 * d) * pi / 180
End Function

Function EccentricAnomaly(m As Double, e As Double) As Double
    deltae = 1
    ma = m
    e0 = ma + e * Sin(ma) * (1# + e * Cos(ma))
    While deltae > 0.00000001
        e1 = e0 - (e0 - e * Sin(e0) - ma) / (1 - e * Cos(e0))
        deltae = Abs(e1 - e0)
        e0 = e1
    Wend
    e0 = e0
    EccentricAnomaly = e0
End Function
Function ArgOfPerihelion(planetno As Integer, dd As Double) As Double
    k = Sheets("Orbital Elements").Cells(5, 5 + (2 * planetno))
    d = Sheets("Orbital Elements").Cells(5, 6 + (2 * planetno))
    ArgOfPerihelion = (k + d * dd) * pi / 180
    While ArgOfPerihelion > twopi
        ArgOfPerihelion = ArgOfPerihelion - twopi
    Wend
    While ArgOfPerihelion < 0
        ArgOfPerihelion = ArgOfPerihelion + twopi
    Wend
End Function
Function ObliquityofEcliptic(dd As Double) As Double
    ObliquityofEcliptic = (23.4392911 - 0.000000356115 * d) * pi / 180
End Function

Function PrecessionCorr(epoch As Double, dd As Double) As Double
    PrecessionCorr = 0.0000382394 * (365.2422 * (epoch - 2000#) - dd)
End Function

Function PlanDist(planetno As Integer, dd As Double) As Double
    Dim a As Double, m As Double, e As Double
    a = Sheets("Orbital Elements").Cells(6, 5 + (2 * planetno)) + Sheets("Orbital Elements").Cells(6, 6 + (2 * planetno)) * dd
    m = MeanAnomaly(planetno, dd)
    e = eccentricity(planetno, dd)
    ea = EccentricAnomaly(m, e)
    xv = Cos(ea) - e
    yv = (1# - e ^ 2) ^ 0.5 * Sin(ea)
    PlanDist = a * ((xv ^ 2 + yv ^ 2) ^ 0.5)
End Function

Function PlanTrueAnomaly(planetno As Integer, d As Double) As Double
    Dim m As Double, e As Double, ea As Double
    m = MeanAnomaly(planetno, d)
    e = eccentricity(planetno, d)
    ea = EccentricAnomaly(m, e)
    xv = (Cos(ea) - e)
    yv = (((1# - e ^ 2) ^ 0.5) * Sin(ea))
    pta = Application.Atan2(xv, yv)
    
    While pta > twopi
        pta = pta - twopi
    Wend
    While pta < 0
        pta = pta + twopi
    Wend
    PlanTrueAnomaly = pta
End Function

Function SunLongitude(v As Double, dd As Double) As Double
    w = ArgOfPerihelion(SUN, dd)
    SunLongitude = (v + w)
End Function

Function SunRA(r As Double, v As Double, dd As Double) As Double
    SunRA = PlanetXYZ(SUN, dd, 6, 0, 0, 0, 0)
End Function

Function SunDec(r As Double, v As Double, dd As Double) As Double
    SunDec = PlanetXYZ(SUN, dd, 7, 0, 0, 0, 0)
End Function

Function AzFromRADec(lst, ra, dec, lat, zora, temp, pres)
    Dim ha As Double, x As Double, y As Double
    Dim xhor As Double, yhor As Double, zhor As Double
    
    ha = lst * 360 - ra
    ha = ha * pi / 180
    
    decl = dec * pi / 180
    lati = lat * pi / 180
    
    x = Cos(ha) * Cos(decl)
    y = Sin(ha) * Cos(decl)
    Z = Sin(decl)

    xhor = x * Sin(lati) - Z * Cos(lati)
    yhor = y
    zhor = x * Cos(lati) + Z * Sin(lati)

    az = Application.Atan2(xhor, yhor) * 180 / pi + 180
    If zora = "z" Then
        AzFromRADec = az
    Else
        alti = Application.Atan2((xhor ^ 2 + yhor ^ 2) ^ 0.5, zhor)
        'If alti > 0 Then
            ' correction for atmospheric distortion - after Saemundsson
            r = 1.02 / Tan(alti + 10.3 / (alti * 180 / pi + 5.11) * pi / 180)
            'temperature & pressure correction
            r = r * pres / 1010 * 283 / (temp + 273)
        'End If
        AzFromRADec = alti * 180 / pi + r / 60
    End If
End Function

Function PlanetXYZ(planetno As Integer, dd As Double, xyz As Integer, lst As Double, lat As Double, temp As Double, pres As Double) As Double
    Dim v As Double, r As Double
    Dim a As Double, m As Double, e As Double
    
    a = Sheets("Orbital Elements").Cells(6, 5 + (2 * planetno)) + Sheets("Orbital Elements").Cells(6, 6 + (2 * planetno)) * dd
    n = (Sheets("Orbital Elements").Cells(3, 5 + (2 * planetno)) + Sheets("Orbital Elements").Cells(3, 6 + (2 * planetno)) * dd) * pi / 180
    m = MeanAnomaly(planetno, dd)
    e = eccentricity(planetno, dd)
    ea = EccentricAnomaly(m, e)
    w = ArgOfPerihelion(planetno, dd)
    i = inclination(planetno, dd)
    
    ' location in its own orbit
    xv = Cos(ea) - e
    yv = (1# - e ^ 2) ^ 0.5 * Sin(ea)
    
    ' distance and true anomaly from perihelion
    r = a * ((xv ^ 2 + yv ^ 2) ^ 0.5)
    v = Application.Atan2(xv, yv)
    
    ' calculate heliocentric coords excluding perturbations
    ' nb these coords are *geo*centric for the moon
    xh = r * (Cos(n) * Cos(v + w) - Sin(n) * Sin(v + w) * Cos(i))
    yh = r * (Sin(n) * Cos(v + w) + Cos(n) * Sin(v + w) * Cos(i))
    zh = r * (Sin(v + w) * Sin(i))
    
    r = (xh ^ 2 + yh ^ 2 + zh ^ 2) ^ 0.5
    
    ' coords in ecliptic plane
    lonecl = Application.Atan2(xh, yh)
    latecl = Application.Atan2((xh ^ 2 + yh ^ 2) ^ 0.5, zh)
    
    If planetno = 0 Then ' moon perturbations
        errlon = MoonPerturbations(dd, 1)
        errlat = MoonPerturbations(dd, 2)
        errd = MoonPerturbations(dd, 3)
        lonecl = lonecl + errlon
        latecl = latecl + errlat
        r = r + errd
    ElseIf planetno > 6 Then ' nothing needed for the very outer planets
        ' do nothing
    ElseIf planetno > 3 Then ' jupiter, saturn and uranus
        ' longitude adjustment for all three
        errlon = GasPerturbations(planetno, dd, 1)
        ' latutide correction too for saturn
        errlat = 0
        If planetno = 5 Then errlat = GasPerturbations(planetno, dd, 2)
        lonecl = lonecl + errlon
        latecl = latecl + errlat
    End If
    If lonecl < 0 Then lonecl = lonecl + twopi
    ' convert perturbed ecliptic coords back to heliocentric
    xh = r * Cos(lonecl) * Cos(latecl)
    yh = r * Sin(lonecl) * Cos(latecl)
    zh = r * Sin(latecl)
    
    ' convert to geocentric coordinates - not needed for moon
    If planetno > 0 Then
        'calculate position of sun
        v = PlanTrueAnomaly(SUN, dd)
        r = PlanDist(SUN, dd)
        ls = SunLongitude(v, dd)
        xs = r * Cos(ls)
        ys = r * Sin(ls)
        
        'add to x-y coords of planet; sun is on ecliptic so z adjust is zero
        xh = xh + xs
        yh = yh + ys
    End If
    
    ' work out the distance from the earth
    distearth = (xh ^ 2 + yh ^ 2 + zh ^ 2) ^ 0.5
    If xyz > 5 And xyz < 10 Then ' calculate RA and DEC
        ecl = ObliquityofEcliptic(dd)
        xe = xh
        ye = yh * Cos(ecl) - zh * Sin(ecl)
        ze = yh * Sin(ecl) + zh * Cos(ecl)
        ra = Application.Atan2(xe, ye) * 180 / pi
        decl = Application.Atan2((xe ^ 2 + ye ^ 2) ^ 0.5, ze) * 180 / pi
        While ra < 0
            ra = ra + 360
        Wend
        While ra > 360
            ra = ra - 360
        Wend
        azi = AzFromRADec(lst, ra, decl, lat, "z", temp, pres)
        alti = AzFromRADec(lst, ra, decl, lat, "l", temp, pres)
        
        ' correct to topographic coords (surface of earth)
        ' mainly relevant for the moon but worth doing for the others
        alti = alti * pi / 180
        If planetno = 0 Then
            alti = alti - Cos(alti) * Application.Asin(1 / r)
        Else
            alti = alti - (8.794 / 3600) * pi / 180 / r
        End If
        alti = alti * 180 / pi
    End If
    ' results
    If xyz = 1 Then PlanetXYZ = xh
    If xyz = 2 Then PlanetXYZ = yh
    If xyz = 3 Then PlanetXYZ = zh
    If xyz = 4 Then PlanetXYZ = lonecl * 180 / pi
    If xyz = 5 Then PlanetXYZ = latecl * 180 / pi
    If xyz = 6 Then PlanetXYZ = ra
    If xyz = 7 Then PlanetXYZ = decl
    If xyz = 8 Then PlanetXYZ = alti
    If xyz = 9 Then PlanetXYZ = azi
    If xyz = 10 Then PlanetXYZ = distearth
End Function

Function MoonPerturbations(dd As Double, latlongpos As Integer) As Double
    Dim Ms As Double, Mm As Double, Nm As Double
    Dim ws As Double, wm As Double
    Dim ll As Double
    
    Ms = MeanAnomaly(SUN, dd)
    Mm = MeanAnomaly(0, dd)
    ws = ArgOfPerihelion(SUN, dd)
    wm = ArgOfPerihelion(0, dd)
    Nm = Sheets("Orbital Elements").Cells(3, 5) + Sheets("Orbital Elements").Cells(3, 6) * dd
    Nm = Nm * pi / 180
    ls = Ms + ws
    Lm = Mm + wm + Nm
    Dm = Lm - ls
    f = Lm - Nm
    If latlongpos = 1 Then ' correcting ecliptic longitude
        ll = -1.274 * Sin(Mm - 2 * Dm)
        ll = ll + 0.658 * Sin(2 * Dm)
        ll = ll - 0.186 * Sin(Ms)
        ll = ll - 0.059 * Sin(2 * Mm - 2 * Dm)
        ll = ll - 0.057 * Sin(Mm - 2 * Dm + Ms)
        ll = ll + 0.053 * Sin(Mm + 2 * Dm)
        ll = ll + 0.046 * Sin(2 * Dm - Ms)
        ll = ll + 0.041 * Sin(Mm - Ms)
        ll = ll - 0.035 * Sin(Dm)
        ll = ll - 0.031 * Sin(Mm + Ms)
        ll = ll - 0.015 * Sin(2 * f - 2 * Dm)
        ll = ll + 0.011 * Sin(Mm - 4 * Dm)
        ll = ll * pi / 180
    ElseIf latlongpos = 2 Then ' latitude
        ll = -0.173 * Sin(f - 2 * Dm)
        ll = ll - 0.055 * Sin(Mm - f - 2 * Dm)
        ll = ll - 0.046 * Sin(Mm + f - 2 * Dm)
        ll = ll + 0.033 * Sin(f + 2 * Dm)
        ll = ll + 0.017 * Sin(2 * Mm + f)
        ll = ll * pi / 180
    Else  ' earth-moon distance
        ll = -0.58 * Cos(Mm - 2 * Dm) - 0.46 * Cos(2 * Dm)
    End If
    MoonPerturbations = ll
End Function

Function GasPerturbations(planetno As Integer, dd As Double, latlongpos As Integer) As Double
    Dim Ms As Double, Mm As Double, Nm As Double
    Dim ws As Double, wm As Double
    Dim ll As Double
    
    Mj = MeanAnomaly(4, dd)
    Ms = MeanAnomaly(5, dd)
    Mu = MeanAnomaly(6, dd)
    If planetno = 4 Then ' jupiter
        gp = -0.332 * Sin(2 * Mj - 5 * Ms - 67.6 * pi / 180)
        gp = gp - 0.056 * Sin(2 * Mj - 2 * Ms + 21 * pi / 180)
        gp = gp + 0.042 * Sin(3 * Mj - 5 * Ms + 21 * pi / 180)
        gp = gp - 0.036 * Sin(Mj - 2 * Ms)
        gp = gp + 0.022 * Cos(Mj - Ms)
        gp = gp + 0.023 * Sin(2 * Mj - 3 * Ms + 52 * pi / 180)
        gp = gp - 0.016 * Sin(Mj - 5 * Ms - 69 * pi / 180)
    ElseIf planetno = 5 Then ' saturn
        If latlongpos = 1 Then
            gp = 0.812 * Sin(2 * Mj - 5 * Ms - 67.6 * pi / 180)
            gp = gp - 0.229 * Cos(2 * Mj - 4 * Ms - 2 * pi / 180)
            gp = gp + 0.119 * Sin(Mj - 2 * Ms - 3 * pi / 180)
            gp = gp + 0.046 * Sin(2 * Mj - 6 * Ms - 69 * pi / 180)
            gp = gp + 0.014 * Sin(Mj - 3 * Ms + 32 * pi / 180)
        Else
            gp = 0.02 * Cos(2 * Mj - 4 * Ms - 2 * pi / 180)
            gp = gp + 0.018 * Sin(2 * Mj - 6 * Ms - 49 * pi / 180)
        End If
    ElseIf planetno = 6 Then ' uranus
        gp = 0.04 * Sin(Ms - 2 * Mu + 6 * pi / 180)
        gp = gp + 0.035 * Sin(Ms - 3 * Mu + 33 * pi / 180)
        gp = gp - 0.015 * Sin(Mj - Mu + 20 * pi / 180)
    Else ' do nothing
        gp = 0
    End If
    GasPerturbations = gp * pi / 180
End Function

