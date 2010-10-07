//sets up the html for the "user interface"
function ballon_setup(){
    var dropdown = document.getElementById('balloon_model');
    for (var model in specs){
        var option = document.createElement('option');
        option.text = model;
        option.value = model;
        dropdown.add(option, null);
    }
    dropdown.options[0].selected = 'selected';
    dropdown.onchange = function(){return balloon_set_model(dropdown);};
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

    var button = document.getElementById('balloon_calculate');
    var payload = document.getElementById('balloon_payload');
    var speed = document.getElementById('balloon_speed');
    var lift = document.getElementById('balloon_lift');

    button.onclick = function(){return calculate(balloon, payload, speed, lift)};

    return;
}

function calculate(balloon, payload, speed, lift){
    _payload = parseFloat(payload.value);
    _speed = parseFloat(speed.value);
    _lift = parseFloat(lift.value);

    //first see whether there is only one unknown value
    if( !isNaN(_payload) && !isNaN(_lift) && isNaN(_speed) ){
        speed.value = speed_from_nozzle_lift(balloon, _payload, _lift).toFixed(2);
    }else if(!isNaN(_payload) && isNaN(_lift) && !isNaN(_speed) ){
        lift.value = nozzle_lift_from_speed(balloon, _payload, _speed).toFixed(1);
    }else{
        alert('Can\'t solve.');
    }
}
