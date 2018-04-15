Attribute VB_Name = "InitPlanetaryElements"
Sub vbLoadOrbitalElements()
   elements(0).name = "Sun"
   elements(0).N(1) = 0
   elements(0).N(2) = 0
   elements(0).incl(1) = 0
   elements(0).incl(2) = 0
   elements(0).omega(1) = 282.9404
   elements(0).omega(2) = 0.0000470935
   elements(0).a(1) = 1.000001018
   elements(0).a(2) = 0
   elements(0).e(1) = 0.016709
   elements(0).e(2) = -1.15091033538672E-09
   elements(0).ma(1) = 356.047
   elements(0).ma(2) = 0.9856002585
   elements(0).mag(1) = -26.76
   elements(0).mag(2) = 0
   elements(0).siz = 1919.26
   
   elements(1).name = "Moon"
   elements(1).N(1) = 125.1228
   elements(1).N(2) = -0.0529538083
   elements(1).incl(1) = 5.1454
   elements(1).incl(2) = 0
   elements(1).omega(1) = 318.0634
   elements(1).omega(2) = 0.1643573223
   elements(1).a(1) = 60.2666
   elements(1).a(2) = 0
   elements(1).e(1) = 0.0549
   elements(1).e(2) = 0
   elements(1).ma(1) = 115.3654
   elements(1).ma(2) = 13.0649929509
   elements(1).mag(1) = 0.23
   elements(1).mag(2) = 0.026
   elements(1).siz = 1873.7
   
   elements(2).name = "Mercury"
   elements(2).N(1) = 48.3313
   elements(2).N(2) = 0.0000324587
   elements(2).incl(1) = 7.0047
   elements(2).incl(2) = 0.00000005
   elements(2).omega(1) = 29.1241
   elements(2).omega(2) = 0.0000101444
   elements(2).a(1) = 0.387098
   elements(2).a(2) = 0
   elements(2).e(1) = 0.205635
   elements(2).e(2) = 0.000000000559
   elements(2).ma(1) = 168.6562
   elements(2).ma(2) = 4.0923344368
   elements(2).mag(1) = -0.36
   elements(2).mag(2) = 0.027
   elements(2).siz = 6.74
   
   elements(3).name = "Venus"
   elements(3).N(1) = 76.6799
   elements(3).N(2) = 0.000024659
   elements(3).incl(1) = 3.3946
   elements(3).incl(2) = 0.0000000275
   elements(3).omega(1) = 54.891
   elements(3).omega(2) = 0.0000138374
   elements(3).a(1) = 0.72333
   elements(3).a(2) = 0
   elements(3).e(1) = 0.006773
   elements(3).e(2) = -0.000000001302
   elements(3).ma(1) = 48.0052
   elements(3).ma(2) = 1.6021302244
   elements(3).mag(1) = -4.34
   elements(3).mag(2) = 0.013
   elements(3).siz = 16.92
   
   elements(4).name = "Mars"
   elements(4).N(1) = 49.5574
   elements(4).N(2) = 0.0000211081
   elements(4).incl(1) = 1.8497
   elements(4).incl(2) = -0.0000000178
   elements(4).omega(1) = 286.5016
   elements(4).omega(2) = 0.0000292961
   elements(4).a(1) = 1.523688
   elements(4).a(2) = 0
   elements(4).e(1) = 0.093405
   elements(4).e(2) = 0.000000002516
   elements(4).ma(1) = 18.6021
   elements(4).ma(2) = 0.5240207766
   elements(4).mag(1) = -1.51
   elements(4).mag(2) = 0.016
   elements(4).siz = 9.36
   
   elements(5).name = "Jupiter"
   elements(5).N(1) = 100.4542
   elements(5).N(2) = 0.0000276854
   elements(5).incl(1) = 1.303
   elements(5).incl(2) = -0.0000001557
   elements(5).omega(1) = 273.8777
   elements(5).omega(2) = 0.0000164505
   elements(5).a(1) = 5.20256
   elements(5).a(2) = 0
   elements(5).e(1) = 0.048498
   elements(5).e(2) = 0.000000004469
   elements(5).ma(1) = 19.895
   elements(5).ma(2) = 0.0830853001
   elements(5).mag(1) = -9.25
   elements(5).mag(2) = 0.014
   elements(5).siz = 196.34
   
   elements(6).name = "Saturn"
   elements(6).N(1) = 113.6634
   elements(6).N(2) = 0.000023898
   elements(6).incl(1) = 2.4886
   elements(6).incl(2) = -0.0000001081
   elements(6).omega(1) = 339.3939
   elements(6).omega(2) = 0.0000297661
   elements(6).a(1) = 9.55475
   elements(6).a(2) = 0
   elements(6).e(1) = 0.055546
   elements(6).e(2) = -0.000000009499
   elements(6).ma(1) = 316.967
   elements(6).ma(2) = 0.0334442282
   elements(6).mag(1) = -9
   elements(6).mag(2) = 0.044
   elements(6).siz = 165.6
   
   elements(7).name = "Uranus"
   elements(7).N(1) = 74.0005
   elements(7).N(2) = 0.000013978
   elements(7).incl(1) = 0.7733
   elements(7).incl(2) = 0.000000019
   elements(7).omega(1) = 96.6612
   elements(7).omega(2) = 0.000030565
   elements(7).a(1) = 19.18171
   elements(7).a(2) = -0.0000000155
   elements(7).e(1) = 0.047318
   elements(7).e(2) = 0.00000000745
   elements(7).ma(1) = 142.5905
   elements(7).ma(2) = 0.011725806
   elements(7).mag(1) = -7.15
   elements(7).mag(2) = 0.001
   elements(7).siz = 65.8
   
   elements(8).name = "Neptune"
   elements(8).N(1) = 131.7806
   elements(8).N(2) = 0.000030173
   elements(8).incl(1) = 1.77
   elements(8).incl(2) = -0.000000255
   elements(8).omega(1) = 272.8461
   elements(8).omega(2) = -0.000006027
   elements(8).a(1) = 30.05826
   elements(8).a(2) = 0.00000003313
   elements(8).e(1) = 0.008606
   elements(8).e(2) = 0.00000000215
   elements(8).ma(1) = 260.2471
   elements(8).ma(2) = 0.005995147
   elements(8).mag(1) = -6.9
   elements(8).mag(2) = 0.001
   elements(8).siz = 62.2
   
   elements(9).name = "Earth"
   elements(9).N(1) = 174.873
   elements(9).N(2) = 0
   elements(9).incl(1) = 0
   elements(9).incl(2) = 0
   elements(9).omega(1) = 288.064
   elements(9).omega(2) = 0.0000470935
   elements(9).a(1) = 1.000001018
   elements(9).a(2) = 0
   elements(9).e(1) = 0.016709
   elements(9).e(2) = -1.15091033538672E-09
   elements(9).ma(1) = 356.047
   elements(9).ma(2) = 0.9856002585
   elements(9).mag(1) = 0
   elements(9).mag(2) = 0
   elements(9).siz = 17.59
   
End Sub
