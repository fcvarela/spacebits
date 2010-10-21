
//fundamental physical constants and specific parameters
R = 8.314472; //J/(K*mol) ideal gas constant
g = 9.80665; // m/(s^2) surface Earth gravitational acceleration
m_He = 4.002602/1000;// kg/mol Helium atomic weight
T0 = -273.15; // degrees celsius absolute zero temperature
m_air = 0.0289644; /// kg/mol dry air mean atomic weight
rho_air = 1.2// kg/m^3

//experimentaly the pressure avries with the altitude as O = P_0 * exp(-A*h)
//where A should be approximately rho_air*g*P_0 where P_0 is the surface pressure 1 atm
////by fitting A = 0.153303 1/km (see pressure.plt for a script that calculates this)

var P_delta = 0.153303; // 1/km
var bar = 100000; //1 bar in Pa

// the basic formula is v = v_0 * (le/le_0)^(1/2) * (lg_0/lg)^(1/3)
// where le is the effective lift (gross lift - payload - balloon weight) and lg is the gross lift (archimede's force)
// _0 means that this value is the value indicated by the manufacter

// nozzle_lift must be in gf and payload in g
function speed_from_nozzle_lift(balloon, payload, nozzle_lift){
    var v_0 = balloon['Rate of Ascent (m.min)']*1.0/60; // m/s
    var le_0 = balloon['Recommended Free Lift (gr)'];
    var lg_0 = balloon['Gross Lift (gr)'];
    var bm = balloon['Average Weight (gr)']; //balloon mass

    var le = nozzle_lift - payload;
    var lg = nozzle_lift + bm;

    return v_0 * Math.pow(le/le_0, 1./2) * Math.pow(lg_0/lg, 1./3);
}

// given ballon specs and an ascent velocity calculate the gross_lift
function nozzle_lift_from_speed(balloon, payload, speed){
    var wrap_fun = function(l){
        return speed_from_nozzle_lift(balloon, payload, l);
    }

    //now find the solution
    return solve(wrap_fun, payload, Number.MAX_VALUE, speed, 0.001);
}

//returns in Km
function burst_altitude_from_nozzle_lift(balloon, nozzle_lift){
    var h_0 = balloon['Bursting Altitude (km)'];
    var lg_0 = balloon['Gross Lift (gr)'];
    var bm = balloon['Average Weight (gr)']; //balloon mass
    var lg = nozzle_lift + bm;

    return h_0 + Math.log(lg_0/lg)/P_delta;
}

function nozzle_lift_from_burst_altitude(balloon, altitude){
    var h_0 = balloon['Bursting Altitude (km)'];
    var lg_0 = balloon['Gross Lift (gr)'];
    var bm = balloon['Average Weight (gr)']; //balloon mass

    var lg = lg_0 * Math.exp((h_0 - altitude)*P_delta);
    return lg - bm;
}
