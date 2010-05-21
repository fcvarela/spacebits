{include file="header.tpl"}

  {include file="main_container_begin.tpl"}

      {include file="topmenu.tpl"}

<div id="articlecontent" style="background:#181b0d;">
This will be the Live Dashboard for the official launch of Spacebits which is scheduled for the 30th of May, 2010, 11AM. Now in demo mode.
</div>
{literal}
<div id="map_wrapper" style="margin-left:30px;"><div id="map" style="width:840px;height:400px;"></div></div>

  <style type="text/css">
    label.left {
      float: left;
      padding: 4px;
      padding-right: 15px;
    }
    .css_sized_container .iPhoneCheckContainer {
      width: 250px;
    }
    ol {
      list-style-type: none;
      margin: 0;
      padding: 0;
    }
    ol li {
      float:right;
      padding: 3px;
      margin: 0;
    }
    .onchange input {
      opacity: 1 !important;
      left: 100px;
    }
    .onchange .iPhoneCheckContainer {
      overflow: visible;
    }
  </style>

<div id="sensors" style="margin-right:27px;margin-left:25px;height:100px;">
    <div id='sensors_alt' style="float:left;"></div>
    <div id='sensors_pressure' style="float:left;"></div>
    <div id='sensors_temperature' style="float:left;"></div>
    <div id='sensors_humidity' style="float:left;"></div>
    <div id='sensors_dust' style="float:left;"></div>
    <div id="buttons" style="float:right;width:250px;margin-top:5px;">
      <ol class="on_off">
      <li>
        <label class="left" for="sw_follow">Follow Balloon</label>
        <input type="checkbox" id="sw_follow" />
      </li>
      <li>
        <label class="left" for="sw_sms">Track SMS</label>
        <input type="checkbox" id="sw_sms" />
      </li>
      <li>
        <label class="left" for="sw_radio">Track Radio</label>
        <input type="checkbox" id="sw_radio" />
      </li>
    </ol>
  </div>
</div>

<script type='text/javascript'>
  var map = false;
  var usePolyline = false;
  var polyline = false;
  var polyline2 = false;
  var markerslayer = false;
  var start=new OpenLayers.LonLat(-8.0919,37.7616);
  var img={markerImage: '/images/balloon.png', markerAnchor: new OpenLayers.Pixel(-8, -49), size: new OpenLayers.Size(32, 42)};
  var sw_follow_cb = false;
  var sw_sms_cb = false;
  var sw_radio_cb = false;

  document.observe("dom:loaded", function() {
    new iPhoneStyle('.on_off input[type=checkbox]',{checkedLabel: 'YES', uncheckedLabel: 'NO'});
    setInterval(function checkSwitches() {
      sw_follow_cb = $$('.on_off input[type=checkbox]')[0].checked;
      sw_sms_cb = $$('.on_off input[type=checkbox]')[1].checked;
      sw_radio_cb = $$('.on_off input[type=checkbox]')[2].checked;
      }, 1000);
    });

  map = new SAPO.Maps.Map('map');
  map.addControl(new SAPO.Maps.Control.MapType());
  map.addControl(new SAPO.Maps.Control.Navigation());
  map.setMapCenter(start,10);

  markerslayer = new SAPO.Maps.Markers('trackpoints');
  map.addMarkers(markerslayer);

  balloon = new SAPO.Maps.Marker(start, {draggable: false}, img);
  map.addOverlay(balloon);

  google.load('visualization', '1', {packages:['gauge']});

  function drawGauge(n,v,d,o) {
    var params = Object.extend({ width: 400, height: 120, redTo: o.max, yellowTo: o.redFrom, minorTicks: 5},o || {});
    var data = new google.visualization.DataTable();
    data.addColumn('string', 'Label');
    data.addColumn('number', 'Value');
    data.addRows(1);
    data.setValue(0, 0, n);
    data.setValue(0, 1, parseInt(Math.floor(Math.random()*v)));
    var chart = new google.visualization.Gauge(document.getElementById(d));
    chart.draw(data, params);
    }

  function refreshDashboard(t) {
    var r=t.responseText.evalJSON();
//    SAPO.Utility.Dumper.alertDump(r);
    // s$('coords').innerHTML=r.lon+','+r.lat;
    drawGauge('Altitude',10000,'sensors_alt',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Pressure',10000,'sensors_pressure',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Temp',10000,'sensors_temperature',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Humidity',10000,'sensors_humidity',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Dust',10000,'sensors_dust',{max:40000,redFrom:30000,yellowFrom:20000});
    // Track
    markerslayer.removeMarkers();

    var size=new OpenLayers.Size(16, 16)
    var shift=new OpenLayers.Pixel(-8,-8);
    var green = { markerImage: '/images/green.png', size: size, markerOpacity: 1,markerAnchor: shift };
    var red = { markerImage: '/images/red.png', size: size, markerOpacity: 1,markerAnchor: shift };

    if(sw_radio_cb) {
      for(var i=0;i<r['track']['radio'].length;i++) {
        var m=new SAPO.Maps.Marker(new OpenLayers.LonLat(r['track']['radio'][i][1],r['track']['radio'][i][0]), {draggable: false});
        m.setStyle(red);
        markerslayer.addMarker(m);
        }
      }
    if(sw_sms_cb) {
      for(var i=0;i<r['track']['sms'].length;i++) {
        var m=new SAPO.Maps.Marker(new OpenLayers.LonLat(r['track']['sms'][i][1],r['track']['sms'][i][0]), {draggable: false});
        m.setStyle(green);
        markerslayer.addMarker(m);
        }
      }
    // Balloon
    var pos=new OpenLayers.LonLat(r['last'].lon,r['last'].lat);
    if(sw_follow_cb) map.setMapCenter(pos,10);
    balloon.setLonLat(pos);
    }

  function getData() {
    var s=new Ajax.Request('/api/all',{method: 'post',onSuccess:refreshDashboard});
    setTimeout (getData, 3000);
    }

  window.onload = getData();
</script>

{/literal}

      {include file="services.tpl"}

  {include file="main_container_end.tpl"}

{include file="footer.tpl"}
