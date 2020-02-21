Attribute VB_Name = "SizeAndBrightness"
Function SunEarthPhaseAngle(planetno As Integer, dd As Double) As Double
    spa = 0
    r = PlanDist(planetno, dd)
    r1 = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0)
    s = PlanDist(SUN, dd)
    If planetno = MOON Then
        r = s
        r1 = r1 * ERAD / AU
    End If
    argv = (r ^ 2 + r1 ^ 2 - s ^ 2) / (2 * r * r1)
    spa = WorksheetFunction.Acos(argv)
    
    SunEarthPhaseAngle = spa * 180 / PI ' phase is given by  (1 + Cos(spa)) / 2
End Function

Function VisualMagnitude(planetno As Integer, dd As Double) As Double
    If planetno = PLUTO Then
        magBase = -1
        magPhaseFactor = 0
    Else
        magBase = elements(planetno).mag(1)
        magPhaseFactor = elements(planetno).mag(2)
    End If
    magNonlinearFactor = 0
    magNonlinearExponent = 0
    
    distearth = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0)
    distsun = PlanDist(planetno, dd)
    phase = SunEarthPhaseAngle(planetno, dd)
    
    If planetno = 0 Then
        distearth = distearth * ERAD / AU
        distsun = PlanDist(SUN, dd)
    End If
    mag = magBase + (5 * WorksheetFunction.Log10(distsun * distearth))
    mag = mag + (magPhaseFactor * phase)
    If magNonlinearExponent > 0 Then
        mag = mag + magNonlinearFactor * phase ^ magNonlinearExponent
    End If
    VisualMagnitude = mag
End Function

Function ApparentSize(planetno As Integer, dd As Double)
    r = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0)
    If planetno = PLUTO Then
        d0 = 3.4
    Else
        d0 = elements(planetno).siz
    End If
    Fact = 1
    If planetno = 0 Then Fact = 60
    ApparentSize = d0 * Fact / r
End Function

