Attribute VB_Name = "OrbitalMechanics"

Function MeanAnomaly(planetno As Integer, d As Double) As Double

    k = elements(planetno).MA(1)
    p = elements(planetno).MA(2)
    
    MeanAnomaly = (k + p * d) / RAD2DEG
    While MeanAnomaly > TWOPI
        MeanAnomaly = MeanAnomaly - TWOPI
    Wend
    While MeanAnomaly < 0
        MeanAnomaly = MeanAnomaly + TWOPI
    Wend
End Function

Function eccentricity(planetno As Integer, d As Double) As Double
    e1 = elements(planetno).e(1)
    e2 = elements(planetno).e(2)
    eccentricity = e1 + e2 * d
End Function

Function inclination(planetno As Integer, d As Double) As Double
    incl1 = elements(planetno).incl(1)
    incl2 = elements(planetno).incl(2)
    inclination = (incl1 + incl2 * d) / RAD2DEG
End Function

Function EccentricAnomaly(m As Double, e As Double) As Double
    deltae = 1
    MA = m
    e0 = MA + e * Sin(MA) * (1# + e * Cos(MA))
    While deltae > 0.00000001
        e1 = e0 - (e0 - e * Sin(e0) - MA) / (1 - e * Cos(e0))
        deltae = Abs(e1 - e0)
        e0 = e1
    Wend
    e0 = e0
    EccentricAnomaly = e0
End Function

Function ArgOfPerihelion(planetno As Integer, dd As Double) As Double
    k = elements(planetno).omega(1)
    d = elements(planetno).omega(2)
    ArgOfPerihelion = (k + d * dd) / RAD2DEG
    While ArgOfPerihelion > TWOPI
        ArgOfPerihelion = ArgOfPerihelion - TWOPI
    Wend
    While ArgOfPerihelion < 0
        ArgOfPerihelion = ArgOfPerihelion + TWOPI
    Wend
End Function
Function ObliquityofEcliptic(dd As Double) As Double
    ObliquityofEcliptic = (23.4392911 - 0.000000356115 * d) / RAD2DEG
End Function

Function PrecessionCorr(epoch As Double, dd As Double) As Double
    PrecessionCorr = 0.0000382394 * (365.2422 * (epoch - 2000#) - dd)
End Function

Function PlanDist(planetno As Integer, dd As Double) As Double
    Dim a As Double, m As Double, e As Double
    If planetno = PLUTO Then
        Dim lonecl As Double, latecl As Double, r As Double
        Call DoPluto(dd, lonecl, latecl, r)
        PlanDist = r
    Else
        a = elements(planetno).a(1) + dd * elements(planetno).a(2)
        m = MeanAnomaly(planetno, dd)
        e = eccentricity(planetno, dd)
        ea = EccentricAnomaly(m, e)
        xv = Cos(ea) - e
        yv = (1# - e ^ 2) ^ 0.5 * Sin(ea)
        PlanDist = a * ((xv ^ 2 + yv ^ 2) ^ 0.5)
    End If
End Function

Function PlanTrueAnomaly(planetno As Integer, d As Double) As Double
    Dim m As Double, e As Double, ea As Double
    m = MeanAnomaly(planetno, d)
    e = eccentricity(planetno, d)
    ea = EccentricAnomaly(m, e)
    xv = (Cos(ea) - e)
    yv = (((1# - e ^ 2) ^ 0.5) * Sin(ea))
    pta = Application.Atan2(xv, yv)
    
    While pta > TWOPI
        pta = pta - TWOPI
    Wend
    While pta < 0
        pta = pta + TWOPI
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
    ha = ha / RAD2DEG
    
    decl = dec / RAD2DEG
    lati = lat / RAD2DEG
    
    x = Cos(ha) * Cos(decl)
    y = Sin(ha) * Cos(decl)
    Z = Sin(decl)

    xhor = x * Sin(lati) - Z * Cos(lati)
    yhor = y
    zhor = x * Cos(lati) + Z * Sin(lati)

    az = Application.Atan2(xhor, yhor) * RAD2DEG + 180
    If zora = "z" Then
        AzFromRADec = az
    Else
        alti = Application.Atan2((xhor ^ 2 + yhor ^ 2) ^ 0.5, zhor)
        'If alti > 0 Then
            ' correction for atmospheric distortion - after Saemundsson
            r = 1.02 / Tan(alti + 10.3 / (alti * 180 / PI + 5.11) / RAD2DEG)
            'temperature & pressure correction
            r = r * pres / 1010 * 283 / (temp + 273)
        'End If
        AzFromRADec = alti * RAD2DEG + r / 60
    End If
End Function

Function PlanetXYZ(planetno As Integer, dd As Double, xyz As Integer, lst As Double, lat As Double, temp As Double, pres As Double) As Double
    Dim v As Double, r As Double
    Dim a As Double, m As Double, e As Double
    Dim lonecl As Double, latecl As Double
    
    If planetno < PLUTO Then
        a = elements(planetno).a(1) + dd * elements(planetno).a(2)
        N = (elements(planetno).N(1) + dd * elements(planetno).N(2)) / RAD2DEG
            
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
        xh = r * (Cos(N) * Cos(v + w) - Sin(N) * Sin(v + w) * Cos(i))
        yh = r * (Sin(N) * Cos(v + w) + Cos(N) * Sin(v + w) * Cos(i))
        zh = r * (Sin(v + w) * Sin(i))
        
        r = (xh ^ 2 + yh ^ 2 + zh ^ 2) ^ 0.5
        
        ' coords in ecliptic plane
        lonecl = Application.Atan2(xh, yh)
        latecl = Application.Atan2((xh ^ 2 + yh ^ 2) ^ 0.5, zh)
    Else
    ' crazy speshul calcs for pluto - nonanalytic, only accurate till 2100
        Call DoPluto(dd, lonecl, latecl, r)
    End If
    If planetno = MOON Then ' moon perturbations
        errlon = MoonPerturbations(dd, 1)
        errlat = MoonPerturbations(dd, 2)
        errd = MoonPerturbations(dd, 3)
        lonecl = lonecl + errlon
        latecl = latecl + errlat
        r = r + errd
    ElseIf planetno > URANUS Then ' nothing needed for the very outer planets
        ' do nothing
    ElseIf planetno > MARS Then ' jupiter, saturn and uranus
        ' longitude adjustment for all three
        errlon = GasPerturbations(planetno, dd, 1)
        ' latutide correction too for saturn
        errlat = 0
        If planetno = SATURN Then errlat = GasPerturbations(planetno, dd, 2)
        lonecl = lonecl + errlon
        latecl = latecl + errlat
    End If
    If lonecl < 0 Then lonecl = lonecl + TWOPI
    ' convert perturbed ecliptic coords back to heliocentric
    xh = r * Cos(lonecl) * Cos(latecl)
    yh = r * Sin(lonecl) * Cos(latecl)
    zh = r * Sin(latecl)
    
    ' convert to geocentric coordinates - not needed for moon
    If planetno > MOON Then
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
        ra = Application.Atan2(xe, ye) * RAD2DEG
        decl = Application.Atan2((xe ^ 2 + ye ^ 2) ^ 0.5, ze) * RAD2DEG
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
        alti = alti * PI / 180
        If planetno = MOON Then
            alti = alti - Cos(alti) * Application.Asin(1 / r)
        Else
            alti = alti - (8.794 / 3600) / RAD2DEG / r
        End If
        alti = alti * RAD2DEG
    End If
    ' results
    If xyz = 1 Then PlanetXYZ = xh
    If xyz = 2 Then PlanetXYZ = yh
    If xyz = 3 Then PlanetXYZ = zh
    If xyz = 4 Then PlanetXYZ = lonecl * RAD2DEG
    If xyz = 5 Then PlanetXYZ = latecl * RAD2DEG
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
    Mm = MeanAnomaly(MOON, dd)
    ws = ArgOfPerihelion(SUN, dd)
    wm = ArgOfPerihelion(MOON, dd)
    Nm = elements(MOON).N(1) + elements(MOON).N(1) * dd
    
    Nm = Nm / RAD2DEG
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
        ll = ll / RAD2DEG
    ElseIf latlongpos = 2 Then ' latitude
        ll = -0.173 * Sin(f - 2 * Dm)
        ll = ll - 0.055 * Sin(Mm - f - 2 * Dm)
        ll = ll - 0.046 * Sin(Mm + f - 2 * Dm)
        ll = ll + 0.033 * Sin(f + 2 * Dm)
        ll = ll + 0.017 * Sin(2 * Mm + f)
        ll = ll / RAD2DEG
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
    If planetno = JUPITER Then ' jupiter
        gp = -0.332 * Sin(2 * Mj - 5 * Ms - 67.6 / RAD2DEG)
        gp = gp - 0.056 * Sin(2 * Mj - 2 * Ms + 21 / RAD2DEG)
        gp = gp + 0.042 * Sin(3 * Mj - 5 * Ms + 21 / RAD2DEG)
        gp = gp - 0.036 * Sin(Mj - 2 * Ms)
        gp = gp + 0.022 * Cos(Mj - Ms)
        gp = gp + 0.023 * Sin(2 * Mj - 3 * Ms + 52 / RAD2DEG)
        gp = gp - 0.016 * Sin(Mj - 5 * Ms - 69 / RAD2DEG)
    ElseIf planetno = SATURN Then ' saturn
        If latlongpos = 1 Then
            gp = 0.812 * Sin(2 * Mj - 5 * Ms - 67.6 / RAD2DEG)
            gp = gp - 0.229 * Cos(2 * Mj - 4 * Ms - 2 / RAD2DEG)
            gp = gp + 0.119 * Sin(Mj - 2 * Ms - 3 / RAD2DEG)
            gp = gp + 0.046 * Sin(2 * Mj - 6 * Ms - 69 / RAD2DEG)
            gp = gp + 0.014 * Sin(Mj - 3 * Ms + 32 / RAD2DEG)
        Else
            gp = 0.02 * Cos(2 * Mj - 4 * Ms - 2 / RAD2DEG)
            gp = gp + 0.018 * Sin(2 * Mj - 6 * Ms - 49 / RAD2DEG)
        End If
    ElseIf planetno = URANUS Then ' uranus
        gp = 0.04 * Sin(Ms - 2 * Mu + 6 / RAD2DEG)
        gp = gp + 0.035 * Sin(Ms - 3 * Mu + 33 / RAD2DEG)
        gp = gp - 0.015 * Sin(Mj - Mu + 20 / RAD2DEG)
    Else ' do nothing
        gp = 0
    End If
    GasPerturbations = gp / RAD2DEG
End Function
Sub DoPluto(dd As Double, lonecl As Double, latecl As Double, r As Double)
    s = 50.03 + 0.033459652 * dd
    p = 238.95 + 0.003968789 * dd
    
    s = s / RAD2DEG
    p = p / RAD2DEG
    
    lonecl = 238.9508 + 0.00400703 * d _
        - 19.799 * Sin(p) + 19.848 * Cos(p) _
         + 0.897 * Sin(2 * p) - 4.956 * Cos(2 * p) _
         + 0.61 * Sin(3 * p) + 1.211 * Cos(3 * p) _
         - 0.341 * Sin(4 * p) - 0.19 * Cos(4 * p) _
         + 0.128 * Sin(5 * p) - 0.034 * Cos(5 * p) _
         - 0.038 * Sin(6 * p) + 0.031 * Cos(6 * p) _
         + 0.02 * Sin(s - p) - 0.01 * Cos(s - p)
         
    latecl = -3.9082 _
         - 5.453 * Sin(p) - 14.975 * Cos(p) _
         + 3.527 * Sin(2 * p) + 1.673 * Cos(2 * p) _
         - 1.051 * Sin(3 * p) + 0.328 * Cos(3 * p) _
         + 0.179 * Sin(4 * p) - 0.292 * Cos(4 * p) _
         + 0.019 * Sin(5 * p) + 0.1 * Cos(5 * p) _
         - 0.031 * Sin(6 * p) - 0.026 * Cos(6 * p) _
         + 0.011 * Cos(s - p)

    r = 40.72 _
       + 6.68 * Sin(p) + 6.9 * Cos(p) _
       - 1.18 * Sin(2 * p) - 0.03 * Cos(2 * p) _
       + 0.15 * Sin(3 * p) - 0.14 * Cos(3 * p)
       
    lonecl = lonecl / RAD2DEG
    latecl = latecl / RAD2DEG
End Sub


Sub SaveOrbitalElements()
    Open "c:\temp\IntElementData.bas" For Output As #1
    Print #1, "sub LoadOrbitalElements()"
    For planetno = SUN To EARTH
        Print #1, "   elements(" & planetno & ").name = """ & Sheets("Orbital Elements").Cells(13 + planetno, 2) & """"
        Print #1, "   elements(" & planetno & ").N(1) = " & Sheets("Orbital Elements").Cells(3, 3 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").N(2) = " & Sheets("Orbital elements").Cells(3, 4 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").incl(1) = " & Sheets("Orbital Elements").Cells(4, 3 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").incl(2) = " & Sheets("Orbital elements").Cells(4, 4 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").omega(1) = " & Sheets("Orbital Elements").Cells(5, 3 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").omega(2) = " & Sheets("Orbital elements").Cells(5, 4 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").a(1) = " & Sheets("Orbital Elements").Cells(6, 3 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").a(2) = " & Sheets("Orbital elements").Cells(6, 4 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").e(1) = " & Sheets("Orbital Elements").Cells(7, 3 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").e(2) = " & Sheets("Orbital elements").Cells(7, 4 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").MA(1) = " & Sheets("Orbital Elements").Cells(8, 3 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").MA(2) = " & Sheets("Orbital elements").Cells(8, 4 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").mag(1) = " & Sheets("Orbital Elements").Cells(9, 3 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").mag(2) = " & Sheets("Orbital Elements").Cells(9, 4 + (2 * planetno))
        Print #1, "   elements(" & planetno & ").siz = " & Sheets("Orbital elements").Cells(10, 3 + (2 * planetno))
        Print #1, "   "
    Next
    Print #1, "end sub "
    Close #1
End Sub
