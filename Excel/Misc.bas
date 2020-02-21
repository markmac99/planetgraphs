Attribute VB_Name = "Misc"

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

