//sets up the html for the "user interface"
function ballon_setup(){
    var dropdown = document.getElementById('balloon_model');
    for (var model in specs){
        var option = document.createElement('option');
        option.text = model;
        option.value = model;
        dropdown.add(option, null);
    }
    dropdown.onchange = function(){return balloon_set_model(dropdown);};
    dropdown.options[dropdown.options.length-3].selected = 'selected';
    balloon_set_model(dropdown);
}

//called when the dropdown with the balloon models changes
function balloon_set_model(select){
    var option = select.options[select.selectedIndex];
    var balloon = specs[option.value];
    
    //fill in recommended values
    var rec_payload = document.getElementById('balloon_rec_payload');
    rec_payload.readOnly = true;
    rec_payload.value = balloon['Payload (gr)'];

    var rec_speed = document.getElementById('balloon_rec_speed');
    rec_speed.readOnly = true;
    rec_speed.value = (balloon['Rate of Ascent (m.min)']/60.).toPrecision(2); //convert to m/s

    var rec_speed = document.getElementById('balloon_rec_lift'); //nozzle lift
    rec_speed.readOnly = true;
    rec_speed.value = balloon['Nozzle Lift (gr)'];

    var rec_speed = document.getElementById('balloon_rec_burst'); //nozzle lift
    rec_speed.readOnly = true;
    rec_speed.value = balloon['Bursting Altitude (km)'];

    var button = document.getElementById('balloon_calculate');
    var payload = document.getElementById('balloon_payload');
    var speed = document.getElementById('balloon_speed');
    var lift = document.getElementById('balloon_lift');
    var burst = document.getElementById('balloon_burst');

    button.onclick = function(){calculate(balloon, payload, speed, lift, burst); return false;};

    return;
}

function calculate(balloon, payload, speed, lift, burst){
    clear_error();
    _payload = parseFloat(payload.value);
    _speed = parseFloat(speed.value);
    _lift = parseFloat(lift.value);
    _burst = parseFloat(burst.value);

    if( isNaN(_payload)){
        report_error('Must specify the payload.');
    }else{
        //first see whether there is only one unknown value
        if( !isNaN(_lift) && isNaN(_speed) && isNaN(_burst)){
            _speed = speed_from_nozzle_lift(balloon, _payload, _lift);
            _burst = burst_altitude_from_nozzle_lift(balloon, _lift);
        }else if( isNaN(_lift) && !isNaN(_speed) && isNaN(_burst)){
            _lift = nozzle_lift_from_speed(balloon, _payload, _speed);
            _burst = burst_altitude_from_nozzle_lift(balloon, _lift);
        }else if( isNaN(_lift) && isNaN(_speed) && !isNaN(_burst)){
            _lift = nozzle_lift_from_burst_altitude(balloon, _burst);
            _speed = speed_from_nozzle_lift(balloon, _payload, _lift);
        }else{
            report_error('Overdetermined. Only one of speed, lift or burst may be defined');
        }
        speed.value = _speed.toFixed(2);
        lift.value = _lift.toFixed(0);
        burst.value = _burst.toPrecision(3);
    }
}

function clear_error(){
    var error_dom = document.getElementById('balloon_error');

    if(error_dom){
        if(error_dom.childNodes.length > 0){
            error_dom.childNodes[0].nodeValue = '';
        }else{
            var text_dom = document.createTextNode('');
            error_dom.appendChild(text_dom);
        }
    }
}

function report_error(error){
    var error_dom = document.getElementById('balloon_error');

    if(error_dom){
        error_dom.childNodes[0].nodeValue = error;
    }else{
        alert(error);
    }
}
