*
* Generic triode model: 12AX7
* Copyright 2003--2008 by Ayumi Nakabayashi, All rights reserved.
* Version 3.10, Generated on Sat Mar  8 22:41:09 2008
*             Plate
*             | Grid
*             | | Cathode
*             | | |
.SUBCKT 12AX7 A G K
BGG   GG   0 V=V(G,K)+0.59836683
BM1   M1   0 V=(0.0017172334*(URAMP(V(A,K))+1e-10))**-0.2685074
BM2   M2   0 V=(0.84817287*(URAMP(V(GG)+URAMP(V(A,K))/88.413802)+1e-10))**1.7685074
BP    P    0 V=0.001130216*(URAMP(V(GG)+URAMP(V(A,K))/104.24031)+1e-10)**1.5
BIK   IK   0 V=U(V(GG))*V(P)+(1-U(V(GG)))*0.00071211506*V(M1)*V(M2)
BIG   IG   0 V=0.000565108*URAMP(V(G,K))**1.5*(URAMP(V(G,K))/(URAMP(V(A,K))+URAMP(V(G,K)))*1.2+0.4)
BIAK  A    K I=URAMP(V(IK,IG)-URAMP(V(IK,IG)-(0.00058141055*URAMP(V(A,K))**1.5)))+1e-10*V(A,K)
BIGK  G    K I=V(IG)
* CAPS
CGA   G    A 1.7p
CGK   G    K 1.6p
CAK   A    K 0.5p
.ENDS
