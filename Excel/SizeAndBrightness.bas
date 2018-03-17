Attribute VB_Name = "SizeAndBrightness"
Function SunEarthPhaseAngle(planetno As Integer, dd As Double) As Double
    spa = 0
    r = PlanDist(planetno, dd)
    r1 = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0)
    s = PlanDist(-1, dd)
    If planetno = 0 Then
        r = s
        r1 = r1 * erad / au
    End If
    argv = (r ^ 2 + r1 ^ 2 - s ^ 2) / (2 * r * r1)
    spa = WorksheetFunction.Acos(argv)
    
    SunEarthPhaseAngle = spa * 180 / pi ' phase is given by  (1 + Cos(spa)) / 2
End Function

Function VisualMagnitude(planetno As Integer, dd As Double) As Double
    magBase = Sheets("Orbital Elements").Cells(9, 5 + (2 * planetno))
    magPhaseFactor = Sheets("Orbital Elements").Cells(9, 6 + (2 * planetno))
    magNonlinearFactor = 0
    magNonlinearExponent = 0
    
    distearth = PlanetXYZ(planetno, dd, 10, 0, 0, 0, 0)
    distsun = PlanDist(planetno, dd)
    phase = SunEarthPhaseAngle(planetno, dd)
    
    If planetno = 0 Then
        distearth = distearth * erad / au
        distsun = PlanDist(-1, dd)
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
    d0 = Sheets("Orbital Elements").Cells(10, 5 + (2 * planetno))
    Fact = 1
    If planetno = 0 Then Fact = 60
    ApparentSize = d0 * Fact / r
End Function

