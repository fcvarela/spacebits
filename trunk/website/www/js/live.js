var refreshRate=3*1000;
var map = false;
var markerslayer = false;
var start=false;
var img=false;
var sw_follow_cb = false;
var sw_sms_cb = false;
var sw_radio_cb = false;
var sw_demo_cb = false;
var useGauges = true;
var gauges={
  altitude:{max:40000,redFrom:30000,yellowFrom:20000,alt:'Balloon altitude<br/>(in meters)'},
  pressure:{min:10,max:1100,redFrom:10,redTo:100,yellowFrom:100,yellowTo:200,alt:'Pressure<br/>(in hPa)'},
  temp:{min:-60,max:20,redFrom:-60,redTo:-40,yellowFrom:-40,yellowTo:-20,alt:'External temperature<br/>(in ºC)'},
  humidity:{max:100,redFrom:50,yellowFrom:30,alt:'Humidity<br/>(% of water in air)'},
  dust:{max:100,redFrom:50,yellowFrom:30,alt:'Dust<br/>(whatever?)'}
  };

if(useGauges===true) {
  google.load('visualization', '1', {packages:['gauge']});
  }

function analogNumber(msg,n,divid,alt,asis) {
  if(asis===true) {
    var n=n.toString();
    } else var n=parseFloat(n).toString();

  var o=msg+': ';
  $(divid).className='measure';
  for(i=0;i<n.length;i++) {
    if(n[i]=='.') c='p';
      else if(n[i]==':') c='c';
      else if(n[i]=='-') c='h';
      else c=n[i];
    if(n[i]!==' ') o+='<img src="/images/dg'+c+'.gif">';
      else o+='&nbsp;&nbsp;';
    }
  $(divid).innerHTML=o;
  setTooltip(divid,alt);
  }

function setTooltip(divid,alt) {
  $(divid).onmouseover=function() {tooltip.show(alt, 200);setTimeout(function(){tooltip.hide();},5000);};
  $(divid).onmouseout=function () {tooltip.hide();};
  }

function initSwitches() {
  document.observe("dom:loaded", function() {
    new iPhoneStyle('.on_off input[type=checkbox]',{checkedLabel: 'YES', uncheckedLabel: 'NO'});
    setInterval(function checkSwitches() {
      sw_follow_cb = $$('.on_off input[type=checkbox]')[0].checked;
      sw_sms_cb = $$('.on_off input[type=checkbox]')[1].checked;
      sw_radio_cb = $$('.on_off input[type=checkbox]')[2].checked;
      sw_demo_cb = $$('.on_off input[type=checkbox]')[3].checked;
      }, 1000);
    });
  }

function initMap() {
  start=new OpenLayers.LonLat(-8.0919,37.7616);
  img={markerImage: '/images/balloon.png', markerAnchor: new OpenLayers.Pixel(-8, -49), size: new OpenLayers.Size(32, 42)};
  map = new SAPO.Maps.Map('map');
  map.addControl(new SAPO.Maps.Control.MapType());
  map.addControl(new SAPO.Maps.Control.Navigation());
  map.setMapCenter(start,10);

  markerslayer = new SAPO.Maps.Markers('trackpoints');
  map.addMarkers(markerslayer);

  balloon = new SAPO.Maps.Marker(start, {draggable: false}, img);
  map.addOverlay(balloon);
  }

function initGauges() {
  drawGauge('Altitude',0,'sensors_alt',gauges.altitude);
  drawGauge('Pressure',0,'sensors_pressure',gauges.pressure);
  drawGauge('Temp',0,'sensors_temperature',gauges.temp);
  drawGauge('Humidity',0,'sensors_humidity',gauges.humidity);
  drawGauge('Dust',0,'sensors_dust',gauges.dust);
  }

function drawGauge(n,v,d,o,alt) {
  var params = Object.extend({ width: 400, height: 120, redTo: o.max, yellowTo: o.redFrom, minorTicks: 5},o || {});
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Label');
  data.addColumn('number', 'Value');
  data.addRows(1);
  data.setValue(0, 0, n);
  data.setValue(0, 1, parseFloat(v));
  var chart = new google.visualization.Gauge(document.getElementById(d));
  chart.draw(data, params);
  setTooltip(d,params.alt);
  }

function refreshDashboard(t) {
  var r=t.responseText.evalJSON();
    //SAPO.Utility.Dumper.alertDump(r);
    // s$('coords').innerHTML=r.lon+','+r.lat;
  if(useGauges) {
    drawGauge('Altitude',r['last'].alt,'sensors_alt',gauges.altitude);
    drawGauge('Pressure',r['last'].pressure,'sensors_pressure',gauges.pressure);
    drawGauge('Temp',r['last'].temperature,'sensors_temperature',gauges.temp);
    drawGauge('Humidity',r['last'].humidity,'sensors_humidity',gauges.humidity);
    drawGauge('Dust',r['last'].dust_density,'sensors_dust',gauges.dust);
    }
    else
    {
    analogNumber('Altitude',r['last'].alt,'sensors_alt',gauges.altitude.alt);
    analogNumber('Pressure',r['last'].pressure,'sensors_pressure',gauges.pressure.alt);
    analogNumber('Temperature',r['last'].temperature,'sensors_temperature',gauges.temp.alt);
    analogNumber('Humidity',r['last'].humidity,'sensors_humidity',gauges.humidity.alt);
    analogNumber('Dust',r['last'].dust_density,'sensors_dust',gauges.dust.alt);
    }
  // Track
  markerslayer.removeMarkers();

  var size=new OpenLayers.Size(16, 16)
  var shift=new OpenLayers.Pixel(-8,-8);
  var green = { markerImage: '/images/green.png', size: size, markerOpacity: 1,markerAnchor: shift };
  var red = { markerImage: '/images/red.png', size: size, markerOpacity: 1,markerAnchor: shift };

  if(sw_radio_cb) {
    for(var i=0;i<r['track']['radio'].length;i++) {
      var co=new OpenLayers.LonLat(r['track']['radio'][i][1],r['track']['radio'][i][0]);
      var m=new SAPO.Maps.Marker(co, {draggable: false});
      m.setStyle(red);
      markerslayer.addMarker(m);
      }
    }
  if(sw_sms_cb) {
    for(var i=0;i<r['track']['sms'].length;i++) {
      var co=new OpenLayers.LonLat(r['track']['sms'][i][1],r['track']['sms'][i][0]);
      var m=new SAPO.Maps.Marker(co, {draggable: false});
      m.setStyle(green);
      markerslayer.addMarker(m);
      }
    }
  // Balloon
  var pos=new OpenLayers.LonLat(r['last'].lon,r['last'].lat);
  if(sw_follow_cb) map.setMapCenter(pos);
  balloon.setLonLat(pos);
  // Other measures
  analogNumber('Bear',r['last'].bear,'bear','Number of GPS satelites<br/>(integer)');
  analogNumber('Ax',r['last'].imu_ax,'ax','Acceleration X axis<br/>(in Gs)');
  analogNumber('Ay',r['last'].imu_ay,'ay','Acceleration Y axis<br/>(in Gs)');
  analogNumber('Az',r['last'].imu_az,'az','Acceleration Z axis<br/>(in Gs)');
  analogNumber('Gx',r['last'].imu_gx,'gx','Angular Speed X axis<br/>(in rad/sec)');
  analogNumber('Gy',r['last'].imu_gy,'gy','Angular Speed Y axis<br/>(in rad/sec)');
  analogNumber('Elapsed Now Last',r['last'].elapsed,'time','Time elapsed from flight start<br/>Current time<br/>Last measurement',true);
  analogNumber('Current',r['last'].power_current,'amps','Power current<br/>(in amps)');
  analogNumber('Voltage',r['last'].power_voltage,'volts','Power voltage<br/>(in volts)');
  }

function getData() {
  var params=[];
  if(sw_demo_cb) params.push("demo=true");
  var s=new Ajax.Request('/api/all',{method: 'post',postBody: params.join('&'),onSuccess:refreshDashboard});
  setTimeout (getData, refreshRate);
  }

function initDashboard() {
  initMap();
  if(useGauges) {
    initGauges();
    }
  initSwitches();
  getData();
  }

var tooltip=function(){
 var id = 'tt';
 var top = 3;
 var left = 3;
 var maxw = 300;
 var speed = 10;
 var timer = 20;
 var endalpha = 95;
 var alpha = 0;
 var tt,t,c,b,h;
 var ie = document.all ? true : false;
 return{
  show:function(v,w){
   if(tt == null){
    tt = document.createElement('div');
    tt.setAttribute('id',id);
    t = document.createElement('div');
    t.setAttribute('id',id + 'top');
    c = document.createElement('div');
    c.setAttribute('id',id + 'cont');
    b = document.createElement('div');
    b.setAttribute('id',id + 'bot');
    tt.appendChild(t);
    tt.appendChild(c);
    tt.appendChild(b);
    document.body.appendChild(tt);
    tt.style.opacity = 0;
    tt.style.filter = 'alpha(opacity=0)';
    document.onmousemove = this.pos;
   }
   tt.style.display = 'block';
   c.innerHTML = v;
   tt.style.width = w ? w + 'px' : 'auto';
   if(!w && ie){
    t.style.display = 'none';
    b.style.display = 'none';
    tt.style.width = tt.offsetWidth;
    t.style.display = 'block';
    b.style.display = 'block';
   }
  if(tt.offsetWidth > maxw){tt.style.width = maxw + 'px'}
  h = parseInt(tt.offsetHeight) + top;
  clearInterval(tt.timer);
  tt.timer = setInterval(function(){tooltip.fade(1)},timer);
  },
  pos:function(e){
   var u = ie ? event.clientY + document.documentElement.scrollTop : e.pageY;
   var l = ie ? event.clientX + document.documentElement.scrollLeft : e.pageX;
   tt.style.top = (u - h) + 'px';
   tt.style.left = (l + left) + 'px';
  },
  fade:function(d){
   var a = alpha;
   if((a != endalpha && d == 1) || (a != 0 && d == -1)){
    var i = speed;
   if(endalpha - a < speed && d == 1){
    i = endalpha - a;
   }else if(alpha < speed && d == -1){
     i = a;
   }
   alpha = a + (i * d);
   tt.style.opacity = alpha * .01;
   tt.style.filter = 'alpha(opacity=' + alpha + ')';
  }else{
    clearInterval(tt.timer);
     if(d == -1){tt.style.display = 'none'}
  }
 },
 hide:function(){
  clearInterval(tt.timer);
   tt.timer = setInterval(function(){tooltip.fade(-1)},timer);
  }
 };
}();
