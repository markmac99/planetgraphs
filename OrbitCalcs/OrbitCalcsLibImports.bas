Attribute VB_Name = "LibDecls"

Declare Function GetObjectNames _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByRef numloaded As Long) As String()

Declare Function AstroDaysFromDt _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dtval As Double) As Double

Declare Function days _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal yy As Long, ByVal mo As Long, ByVal dd As Long, ByVal hh As Long, ByVal mm As Long, ByVal ss As Long) As Double

Declare Function JulianDate _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal yy As Long, ByVal mo As Long, ByVal dd As Long, ByVal hh As Long, ByVal mm As Long, ByVal ss As Long) As Double

Declare Function LocalSiderealTime _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal yy As Long, ByVal mo As Long, ByVal dd As Long, ByVal hh As Long, ByVal mm As Long, ByVal ss As Long, ByVal ll As Double) As Double
    
Declare Function LSTFromDt _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dtval As Double, ByVal longi As Double) As Double
    
Declare Function GetDateFromDtval _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dtval As Double, yy As Long, mo As Long, dd As Long, hh As Long, mm As Long, ss As Long) As Double

Declare Function DtvalToUnixTS _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dtval As Double) As Long

Declare Function GetOrbitalParam _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal what As Long) As Double

Declare Function MeanAnomaly _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function LongOfAscNode _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function Eccentricity _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function VisualMagnitude _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function ApparentSize _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function Inclination _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function ArgOfPerihelion _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function EccentricAnomaly _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal ma As Double, ByVal ee As Double) As Double

Declare Function PlanTrueAnomaly _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function Elongation _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function Phase _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function PlanDist _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double) As Double

Declare Function ObliquityofEcliptic _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dd As Double) As Double
    
Declare Function SunRA _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dd As Double) As Double
    
Declare Function SunDec _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dd As Double) As Double
   
Declare Function PrecessionCorr _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal epoch As Double, ByVal dd As Double) As Double

Declare Function SunLongitude _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal suntrueanomaly As Double, ByVal dd As Double) As Double
    
Declare Function AzFromRADec _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal lst As Double, ByVal ra As Double, ByVal dec As Double, ByVal lat As Double, ByVal zora As Integer, ByVal temp As Double, ByVal pres As Double) As Double

Declare Function PlanetXYZ _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dd As Double, ByVal xyz As Long, ByVal lst As Double, ByVal lat As Double, ByVal temp As Double, ByVal pres As Double) As Double

Declare Function IsVisible _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dtval As Double, ByVal lat As Double, ByVal longi As Double, ByVal vot As Long, ByVal aot As Long, ByVal temp As Double, ByVal pres As Double) As Double

Declare Function RiseSet _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dtval As Double, ByVal lat As Double, ByVal longi As Double, ByVal ros As Long, ByVal h As Double, ByVal temp As Double, ByVal pres As Double) As Double

Declare Function SunRiseSet _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal dtval As Double, ByVal lat As Double, ByVal longi As Double, ByVal ros As Long, ByVal h As Double, ByVal temp As Double, ByVal pres As Double) As Double

Declare Function TimeofTransit _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dtval As Double, ByVal lat As Double, ByVal longi As Double) As Double

Declare Function AltAtTransit _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long, ByVal dtval As Double, ByVal lat As Double, ByVal longi As Double, ByVal temp As Double, ByVal pres As Double) As Double

Declare Function GetMA _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long) As Double

Declare Function GetMADrift _
    Lib "C:/users/mark/documents/projects/planetgraphs/orbitcalcs/Release/orbitcalcs.dll" _
    (ByVal planetno As Long) As Double



'  display decimal degrees in string format DD° MMm SSs
Function Convert_Degree(Decimal_Deg) As Variant
    With Application
        'Set degree to Integer of Argument Passed
        If Decimal_Deg < 0 Then
            Decimal_Deg = Decimal_Deg * -1
            wasneg = "-"
        Else
            wasneg = ""
        End If
        Degrees = Int(Decimal_Deg)
        'Set minutes to 60 times the number to the right
        'of the decimal for the variable Decimal_Deg
        Minutes = (Decimal_Deg - Degrees) * 60
        'Set seconds to 60 times the number to the right of the
        'decimal for the variable Minute
        Seconds = Format(((Minutes - Int(Minutes)) * 60), "0")
        'Returns the Result of degree conversion
       '(for example, 10.46 = 10~ 27  ' 36")
        Convert_Degree = wasneg & Degrees & "° " & Int(Minutes) & "' " _
            & Seconds + Chr(34)
    End With
End Function

Function VBGetObjectNames()
    Dim v() As String
    Dim v2() As String
    Dim numloaded As Long
    v = GetObjectNames(numloaded)
    ReDim v2(numloaded, 1)
    For i = 0 To numloaded - 1
        v2(i, 0) = StrConv(v(0, i), vbUnicode)
    Next i
    VBGetObjectNames = v2
End Function

