f(x) = P_0 * exp(-A*x)

g = 9.8
rho = 1.3943

#first guesses
P_0 = 101325/100 #(surface pressure)
A= rho*g/100

#now fit
fit f(x) "pressure.data" via P_0,A

set xlabel "Altitude [Km]"
set ylabel "Pressure (hPa)"

plot f(x) title "fit P_0*expt(-A*h)", "pressure.data" title "Specs"
pause -1  "Hit return to continue"
