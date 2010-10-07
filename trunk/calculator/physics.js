
//fundamental physical constants and specific parameters
R = 8.314472; //J/(K*mol) ideal gas constant
g = 9.80665; // m/(s^2) surface Earth gravitational acceleration
m_He = 4.002602/1000;// kg/mol Helium atomic weight
T0 = -273.15; // degrees celsius absolute zero temperature
m_air = 0.0289644; /// kg/mol dry air mean atomic weight


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
    return solve(wrap_fun, payload, 100*payload, speed, 0.001);
}
