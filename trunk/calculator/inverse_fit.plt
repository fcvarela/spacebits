h1(p) = -1/a * log(p/b)

a=0.153303
b=1165.39

h2(p) = A*(1-(p/B)**C)

A=44330
B=101325
C=0.190295

#now fit
fit h1(x) "pressure.data" using 2:1 via a,b
fit h2(x) "pressure.data" using 2:1 via A,B,C

set ylabel "Altitude [Km]"
set xlabel "Pressure (hPa)"

plot h1(x) title "h = A*log(p/B)",h2(x) title "h = A*(1-(p/B)^C)", "pressure.data" using 2:1 title "Specs"
pause -1  "Hit return to continue"
