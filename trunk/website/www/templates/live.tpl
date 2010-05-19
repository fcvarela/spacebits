{include file="header.tpl"}

  {include file="main_container_begin.tpl"}

      {include file="topmenu.tpl"}

<div id="articlecontent" style="background:#181b0d;">
This will be the Live Dashboard for the official launch of Spacebits which is scheduled for the 30th of May, 2010, 11AM. This page is still incomplete though. It'll have a real time map with the Balloon's position and live values from all sensors, and more.
</div>
{literal}
<div id="map_wrapper" style="margin-left:30px;"><div id="map" style="width:840px;height:400px;"></div></div>

<div id="sensors" style="margin-left:25px;height:100px;">
    <div id='sensors_alt' style="float:left;"></div>
    <div id='sensors_pressure' style="float:left;"></div>
    <div id='sensors_temperature' style="float:left;"></div>
    <div id='sensors_humidity' style="float:left;"></div>
    <div id='sensors_dust' style="float:left;"></div>
</div>

<script type='text/javascript'>
  var map = false;
  var marker = false;
  var polyline = false;
  var start=new OpenLayers.LonLat(-8.0919,37.7616);
  var img={markerImage: '/images/balloon.png', markerAnchor: new OpenLayers.Pixel(-8, -49), size: new OpenLayers.Size(32, 42)};
  map = new SAPO.Maps.Map('map');
  map.setMapCenter(start,10);

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

  function printBalloon(t) {
    var r=t.responseText.evalJSON();
//    SAPO.Utility.Dumper.alertDump(r);
    map.setMapCenter(new OpenLayers.LonLat(r.lon,r.lat),10);
    if(marker) map.removeOverlay(marker);
    marker = new SAPO.Maps.Marker(new OpenLayers.LonLat(r.lon,r.lat), {draggable: false}, img);
    map.addOverlay(marker);
    // s$('coords').innerHTML=r.lon+','+r.lat;
    drawGauge('Altitude',10000,'sensors_alt',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Pressure',10000,'sensors_pressure',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Temp',10000,'sensors_temperature',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Humidity',10000,'sensors_humidity',{max:40000,redFrom:30000,yellowFrom:20000});
    drawGauge('Dust Density',10000,'sensors_dust',{max:40000,redFrom:30000,yellowFrom:20000});
    }

  function printTrack(t) {
    var r=t.responseText.evalJSON();
    var pols=[];
    pols[0]=start;
    for(var i=0;i<r.length;i++) {
      pols[i+1]=new OpenLayers.LonLat(r[i][1],r[i][0]);
      }
    if(polyline) map.removeOverlay(polyline);
    polyline = new SAPO.Maps.Polyline(pols, { strokeColor: '#ff0000', strokeOpacity: 1, strokeWidth: 5, strokeDashstyle: 'dot' });
    map.removeOverlay(polyline);
    map.addOverlay(polyline);
    }

  function getData() {
    var s=new Ajax.Request('/api/get',{method: 'post',onSuccess:printBalloon});
    var s=new Ajax.Request('/api/track',{method: 'post',onSuccess:printTrack});
    setTimeout (getData, 3000);
    }

  window.onload = getData();
</script>

{/literal}

      {include file="services.tpl"}

  {include file="main_container_end.tpl"}

{include file="footer.tpl"}
