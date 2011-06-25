var refreshRate=10*1000;
var map = false;
var ctimer=15;
var markerslayer = false;
var start=false;
var img=false;
var sw_follow_cb = false;
var sw_sms_cb = true;
var sw_radio_cb = true;
var sw_demo_cb = false;
var sw_twitter_cb = false;
var sw_startpos=[-8.6837,37.1082];
var useGauges = true;
var twitter_width = 250;
var twitter_last = false;
var sw_balloon_id = 6;
var dg=[];
dg['0']='data:image/gif;base64,R0lGODlhEAAYAKIAABISEjMzAGYzAGZmAJlmAMyZAP/MAAAAACH5BAkKAAcALAAAAAAQABgAAANgCLrc/i8UQ2sljJRpqWYTMYwkYRSggTUhQw3O6xpCTC8BFTS5sSsCimMgXBBXjB5jgOQ1B7WHALaY/BjBpqyxBfSuii9OxyMriIZh8bw2tkOk0glk6pwKzbr9BIb4/wwJADs=';
dg['1']='data:image/gif;base64,R0lGODlhEAAYAKIAABISEjMzAGYzAGZmAJlmAMyZAP/MAAAAACH5BAkKAAcALAAAAAAQABgAAAM5CLrc/jACIZkoVQmDczFEBhjGIJJUFZBBNpDiaohAEIqCmW13ReoSWUvFcsGMswyBQyP0MkOalJEAADs=';
dg['2']='data:image/gif;base64,R0lGODlhEAAYALMAAAgICBISEjMzAGYzAGZmAJlmAMyZAP/MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAkKAAgALAAAAAAQABgAAARSMMhJq70XmMN7L5XmfYYFEGhaHCVGbaA7cYQ8H4MdCJygExxTahikiEaHQkxCWCEPvtdhmIpSaDoeNNv7FW3Agy4AoxIAFoNzZEBT1sj2eI6JAAA7';
dg['3']='data:image/gif;base64,R0lGODlhEAAYALMAAAgICBISEjMzAGYzAGZmAJlmAMyZAP/MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAkKAAgALAAAAAAQABgAAARPMMhJq70XmMN7L5XmfYYFEGhaHCVGbaA7cYQ8H4MdCJygExxTahikiEaHQmzCQyZrFcEQtVrKaLqmz6b9FW3Ag0U6XbWYToPB2kxvdXBMBAA7';
dg['4']='data:image/gif;base64,R0lGODlhEAAYAKIAABISEjMzAGYzAGZmAJlmAMyZAP/MAAAAACH5BAkKAAcALAAAAAAQABgAAANXCLrc/i9AJRghUBRWDG6CsS3dx3mMYQyOyi5q1cRLoEqMbeBAaDgDFSNYGBiPwd9iUOionh6TQgdVEV45pJGAmsBW3uktDNDxJkkyIE3mjtrS8FlNZyQAADs=';
dg['5']='data:image/gif;base64,R0lGODlhEAAYAKIAABISEjMzAGYzAGZmAJlmAMyZAP/MAAAAACH5BAkKAAcALAAAAAAQABgAAANOCLrc/i8UQ2sNjJRpKWYTMYzkBxoE1FCDylCCqwSeDAiUDQxGQZaOwaYDc9CIqFYj8BsRUDqW7WhSUW08gy5rbA6ehSVyk1ocx1WdmpEAADs=';
dg['6']='data:image/gif;base64,R0lGODlhEAAYAKIAABISEjMzAGYzAGZmAJlmAMyZAP/MAAAAACH5BAkKAAcALAAAAAAQABgAAANZCLrc/i8UQ2sNjJRpKWYTMYzkBxoE1FCDylCCqwSeDAiUDQxGQZaOwaYDawwIRIro9CMhUwuWQzqrMWgGE9ZUzS54hmDuO2aAT6ImsgBCEjdQhTtZ4OrujAQAOw==';
dg['7']='data:image/gif;base64,R0lGODlhEAAYALMAAAgICBISEjMzAGYzAGZmAJlmAMyZAP/MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAkKAAgALAAAAAAQABgAAAREMMhJq70XmMN7L5XmfYYFEGhaHCVGbaA7cYQ8H4MdCJygExzdLigUxHSDGvJwlNF0vIPPFp3KgAchVrhq6QpNqnCMiQAAOw==';
dg['8']='data:image/gif;base64,R0lGODlhEAAYAKIAABISEjMzAGYzAGZmAJlmAMyZAP/MAAAAACH5BAkKAAcALAAAAAAQABgAAANiCLrc/i8UQ2sljJRpqWYTMYwkYRSggTUhQw3O6xpCTC8BFTS5sSsCimMgXBALpGRRMdh0PCum6amCLSbJkmpm5eJ0PLCi9/v6jEu0gUFcN9opUdaEuk47myjg/iyUIYCBDQkAOw==';
dg['9']='data:image/gif;base64,R0lGODlhEAAYALMAAAAAABISEjMzAGYzAGZmAJlmAMyZAP/MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAkKAAgALAAAAAAQABgAAARoMMhJq71XmMN7L1RhbB4nUltBrGxxGOhRAHSdUhxR1zl+DDtAgDOYCDiC4PEgmAw4gSABOpkaWFiqhDAqmUCUpVdlEWBXLjBm0ltLlk13AC4PTA91u9Z4JrhgYV4vBmpzghwGcXmLFhEAOw==';
dg['am']='data:image/gif;base64,R0lGODlhEAAYAKIAAAAAABISEsyZAP/MAAAAAAAAAAAAAAAAACH5BAkKAAQALAAAAAAQABgAAAMpGLrc/jDKSau9OOvNu/9DE4YiQAZCEKaLALzsqDJDXSvnOeN875M5TQIAOw==';
dg['c']='data:image/gif;base64,R0lGODlhCAAYAKIAABISEjMzAJlmAP/MAAAAAAAAAAAAAAAAACH5BAkKAAQALAAAAAAIABgAAAMdCLrc/hAKF8ZcYuircnBcJEqUxWTbOXxNOL7wmwAAOw==';
dg['h']='data:image/gif;base64,R0lGODlhEAAYAKIAAAgICBISEjMzAGZmAMyZAP/MAAAAAAAAACH5BAkKAAYALAAAAAAQABgAAAMkGLrc/jDKSau9OE8wuh+CAxBFaZYhI5wnATjC571abd94rl8JADs=';
dg['p']='data:image/gif;base64,R0lGODlhCAAYAKIAABISEjMzAJlmAP/MAAAAAAAAAAAAAAAAACH5BAkKAAQALAAAAAAIABgAAAMXCLrc/jDKSau9eAoXxl7CIH5KGDhklAAAOw==';
dg['pm']='data:image/gif;base64,R0lGODlhEAAYAKIAAAAAABISEsyZAP/MAAAAAAAAAAAAAAAAACH5BAkKAAQALAAAAAAQABgAAAMsGLrc/jDKSau9OOvNu+/CMAgMSTIDoJ7kEJxKqrqBa6MifNOxw/M13wJoSQAAOw==';

var gauges={
  altitude:{max:40000,redFrom:30000,yellowFrom:20000,alt:'Balloon altitude<br/>(in meters)'},
  pressure:{min:10,max:1100,redFrom:10,redTo:100,yellowFrom:100,yellowTo:200,alt:'Pressure<br/>(in hPa)'},
  temp:{min:-60,max:20,redFrom:-60,redTo:-40,yellowFrom:-40,yellowTo:-20,alt:'Internal temperature<br/>(in ºC)'},
  temp_ext:{min:-60,max:20,redFrom:-60,redTo:-40,yellowFrom:-40,yellowTo:-20,alt:'External temperature<br/>(in ºC)'},
  humidity:{max:100,redFrom:50,yellowFrom:30,alt:'Humidity<br/>(% of water in air)'},
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
    if(n[i]!==' ') o+='<img src="'+dg[c]+'">';
      else o+='&nbsp;&nbsp;';
    }
  $(divid).innerHTML=o;
  setTooltip(divid,alt);
  }

function setTooltip(divid,alt) {
  $(divid).onmouseover=function() {tooltip.show(alt, 200);setTimeout(function(){tooltip.hide();},10000);};
  $(divid).onmouseout=function () {tooltip.hide();};
  }

function initMap() {
  resizeMap();
  start=new OpenLayers.LonLat(sw_startpos[0],sw_startpos[1]);
  img={markerImage: '/images/balloon.png', markerAnchor: new OpenLayers.Pixel(-8, -49), size: new OpenLayers.Size(32, 42)};
  map = new SAPO.Maps.Map('map');
  map.addControl(new SAPO.Maps.Control.MapType());
  map.addControl(new SAPO.Maps.Control.Navigation());
  var ccoords = new SAPO.Maps.Control.MapCenter({isMinimized: false});
  map.addControl(ccoords);

  map.setMapCenter(start,10);

  markerslayer = new SAPO.Maps.Markers('trackpoints');
  map.addMarkers(markerslayer);

  balloon = new SAPO.Maps.Marker(start, {draggable: false}, img);
  map.addOverlay(balloon);
  }

function initGauges() {
  drawGauge('Altitude',0,'sensors_alt',gauges.altitude);
  drawGauge('Pressure',0,'sensors_pressure',gauges.pressure);
  drawGauge('Int Temp',0,'sensors_temperature',gauges.temp);
  drawGauge('Ext Temp',0,'sensors_temperature_ext',gauges.temp_ext);
  drawGauge('Humidity',0,'sensors_humidity',gauges.humidity);
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
  // s$('coords').innerHTML=r.lon+','+r.lat;
  var last=r['last'][0];
  for(var i=0;i<r['last'].length;i++) {
    if(r['last'][i].id==sw_balloon_id) last=r['last'][i];
    }
  // Track
  markerslayer.removeMarkers();

  var size=new OpenLayers.Size(16, 16)
  var shift=new OpenLayers.Pixel(-8,-8);
  var green = { markerImage: '/images/green.png', size: size, markerOpacity: 1,markerAnchor: shift };
  var red = { markerImage: '/images/red.png', size: size, markerOpacity: 1,markerAnchor: shift};

  if(sw_radio_cb) {
    red.markerOpacity=0;
    for(var i=0;i<r['track'][0]['radio'].length;i++) {
      var co=new OpenLayers.LonLat(r['track'][0]['radio'][i][1],r['track'][0]['radio'][i][0]);
      var m=new SAPO.Maps.Marker(co, {draggable: false});
      red.markerOpacity+=1/r['track'][0]['radio'].length;
      red.label=i;
      m.setStyle(red);
      markerslayer.addMarker(m);
      }
    }
  if(sw_sms_cb) {
    for(var i=0;i<r['track'][0]['sms'].length;i++) {
      var co=new OpenLayers.LonLat(r['track'][0]['sms'][i][1],r['track'][0]['sms'][i][0]);
      var m=new SAPO.Maps.Marker(co, {draggable: false});
      m.setStyle(green);
      markerslayer.addMarker(m);
      }
    }

  // Console
  var ct='<p><b>Last radio packet ('+last.change_txt+')</b></p>';
  ct+='<p>Balloon id: '+last.balloon+"</p>";
  var patmp=last.pressure/101325;
  var pbarp=last.pressure/100000;
  ct+='<p>Pressure: '+last.pressure+' Pa, '+patmp.toPrecision(2)+' Atm, '+pbarp.toPrecision(2)+' Bar</p>';
  ct+='<p>Internal Temperature: '+(last.temperature/100)+" C</p>";
  ct+='<p>External Temperature: '+(last.temperature_ext/100)+" C</p>";

  var humidity=161*(last.humidity*4.88/5000)-25.8;
  humidity=Math.round(humidity/(1.0546-0.0026*(last.temperature_ext/100)));
  ct+='<p>Humidity: '+humidity+" %</p>";
  ct+='<p>Latitude: '+last.lat+"</p>";
  ct+='<p>Longitude: '+last.lon+"</p>";
  ct+='<p>Altitude: '+Math.round(last.alt)+" m</p>";
  $('console').innerHTML=ct;

  // Balloon
  var pos=new OpenLayers.LonLat(last.lon,last.lat);
  map.setMapCenter(pos);
  balloon.setLonLat(pos);
  }

function getData() {
  var params=[];
  $('console').innerHTML='Getting new data...';
  // if(sw_demo_cb) params.push("demo=true");
  var s=new Ajax.Request('/api/all/'+sw_balloon_id,{method: 'post',postBody: params.join('&'),onSuccess:refreshDashboard});
  }

function printTimer() {
  $('counter').innerHTML=ctimer;
  ctimer--;
  if(ctimer==0) {
    getData();
    ctimer=15;
    }
  setTimeout (printTimer, 1000);
  }

function initDashboard() {
  switch(BrowserDetect.browser+parseInt(BrowserDetect.version)) {
    case "Safari4":
    case "Safari5":
    case "SafariNaN":
    case "Firefox3":
    case "Firefox4":
    case "Opera9":
    case "Opera10":
    case "Chrome5":
    case "Chrome6":
      break;
    case "Explorer8":
    case "Explorer9":
      alert(BrowserDetect.browser+parseInt(BrowserDetect.version)+" will work with this page\nbut some elements have been degraded.\n\nPlease use one of these browsers for the best experience:\nSafari 4, Firefox 3, Opera 9, Chrome 5");
      useGauges=false;
      break;
    default:
      alert("Your browser "+BrowserDetect.browser+parseInt(BrowserDetect.version)+"\nis not supported for this page.\nSome elements may fail to render.\n\nPlease use one of these browsers for the best experience:\nSafari 4, Firefox 3, Opera 9, Chrome 5");
      break;
    }
  initMap();
  setTimeout (printTimer, 1000);
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

var BrowserDetect = {
	init: function () {
		this.browser = this.searchString(this.dataBrowser) || "An unknown browser";
		this.version = this.searchVersion(navigator.userAgent)
			|| this.searchVersion(navigator.appVersion)
			|| "an unknown version";
		this.OS = this.searchString(this.dataOS) || "an unknown OS";
	},
	searchString: function (data) {
		for (var i=0;i<data.length;i++)	{
			var dataString = data[i].string;
			var dataProp = data[i].prop;
			this.versionSearchString = data[i].versionSearch || data[i].identity;
			if (dataString) {
				if (dataString.indexOf(data[i].subString) != -1)
					return data[i].identity;
			}
			else if (dataProp)
				return data[i].identity;
		}
	},
	searchVersion: function (dataString) {
		var index = dataString.indexOf(this.versionSearchString);
		if (index == -1) return;
		return parseFloat(dataString.substring(index+this.versionSearchString.length+1));
	},
	dataBrowser: [
		{
			string: navigator.userAgent,
			subString: "Chrome",
			identity: "Chrome"
		},
		{ 	string: navigator.userAgent,
			subString: "OmniWeb",
			versionSearch: "OmniWeb/",
			identity: "OmniWeb"
		},
		{
			string: navigator.vendor,
			subString: "Apple",
			identity: "Safari",
			versionSearch: "Version"
		},
		{
			prop: window.opera,
			identity: "Opera"
		},
		{
			string: navigator.vendor,
			subString: "iCab",
			identity: "iCab"
		},
		{
			string: navigator.vendor,
			subString: "KDE",
			identity: "Konqueror"
		},
		{
			string: navigator.userAgent,
			subString: "Firefox",
			identity: "Firefox"
		},
		{
			string: navigator.vendor,
			subString: "Camino",
			identity: "Camino"
		},
		{		// for newer Netscapes (6+)
			string: navigator.userAgent,
			subString: "Netscape",
			identity: "Netscape"
		},
		{
			string: navigator.userAgent,
			subString: "MSIE",
			identity: "Explorer",
			versionSearch: "MSIE"
		},
		{
			string: navigator.userAgent,
			subString: "Gecko",
			identity: "Mozilla",
			versionSearch: "rv"
		},
		{ 		// for older Netscapes (4-)
			string: navigator.userAgent,
			subString: "Mozilla",
			identity: "Netscape",
			versionSearch: "Mozilla"
		}
	],
	dataOS : [
		{
			string: navigator.platform,
			subString: "Win",
			identity: "Windows"
		},
		{
			string: navigator.platform,
			subString: "Mac",
			identity: "Mac"
		},
		{
			   string: navigator.userAgent,
			   subString: "iPhone",
			   identity: "iPhone/iPod"
	    },
		{
			string: navigator.platform,
			subString: "Linux",
			identity: "Linux"
		}
	]

};

SAPO.Maps.Control.MapCenter = OpenLayers.Class(OpenLayers.Control, {
    
    /** 
     * Property: element
     * {DOMElement} 
     */
    element: null,
    
    /** 
     * APIProperty: prefix
     * {String}
     */
    prefix: '',
    
    /** 
     * APIProperty: separator
     * {String}
     */
    separator: ', ',
    
    /** 
     * APIProperty: suffix
     * {String}
     */
    suffix: '',
    
    /** 
     * APIProperty: numDigits
     * {Integer}
     */
    numDigits: 6,
    
    /** 
     * APIProperty: granularity
     * {Integer} 
     */
    granularity: 10,
    
    /** 
     * Property: lastXy
     * {<OpenLayers.LonLat>}
     */
    lastXy: null,

    /**
     * APIProperty: displayProjection
     * {<OpenLayers.Projection>} A projection that the 
     * mousecontrol will display.
     */
    displayProjection: null, 

    /** 
     * API Property: isMinimized
     * Indicates if the control is or not minimized
     */	
	isMinimized: true,
	
	/**
	 * Property: minRightPosition {Integer}
	 */
	minRightPosition: -135,

	/**
	 * Property: nrSteps {Integer}
	 * The number of steps to minimize the element
	 */	
	nrSteps: 5,
	
    
    /**
     * Constructor: OpenLayers.Control.MousePosition
     * 
     * Parameters:
     * options - {DOMElement} Options for control.
     */
    initialize: function(options) {
		this.displayProjection = new OpenLayers.Projection('EPSG:4326');
        OpenLayers.Control.prototype.initialize.apply(this, arguments);
    },
	
	/**
	 * API Method: toggle
	 * @param {Boolean} minimize
	 * @param {Boolean} amimation
	 */
	toggle: function(minimize, animation){
		if(this.isMinimized === minimize){ return; }

		this.isMinimized = minimize;
		if(!animation){
			this.div.style.right = (minimize? this.minRightPosition : 0) + 'px';
			//return;
		}else{
		
			var current_x = minimize? 0 : this.minRightPosition;
			var dx = this.minRightPosition/this.nrSteps;
			
			if(!minimize){
				dx *= -1;
			}
			var _this = this;
			var interval = window.setInterval(function(){
				
				current_x += dx;
				if((minimize && current_x <= _this.minRightPosition) || 
					(!minimize && current_x >= 0)){
						
						_this.div.style.right = (minimize? _this.minRightPosition : 0) + 'px';
						clearInterval(interval);
				}else{
				
					_this.div.style.right = current_x + 'px';
				}
				
			}, 5);
		}
		
		if(minimize){
			this.unregisterListeners();
		}else{
			this.registerListeners();
		}
		
	},
	
	/**
	 * Return type {Boolean}
	 * Returns true if the coordinates control is opened, false otherwise
	 */
	isOpened: function(){
		return !this.isMinimized;
	},

    /**
     * Method: destroy
     */
     destroy: function() {
         if (this.map) {
             this.unregisterListeners();
         }
         OpenLayers.Control.prototype.destroy.apply(this, arguments);
     },

    /**
     * Method: draw
     * {DOMElement}
     */    
    draw: function(px) {
        OpenLayers.Control.prototype.draw.apply(this, arguments);
		
		var holder = document.createElement('div');
		this.element = document.createElement('p');
		
		OpenLayers.Event.observe(holder, 'click', 
				OpenLayers.Function.bindAsEventListener(this.toggleControl,
                                                        this));
		OpenLayers.Event.observe(this.element, 'click', 
				OpenLayers.Function.bindAsEventListener(this.stopBublingEvent,
                                                        this));												
		
		//classNames
		holder.className = 'coordinates';
		if(SAPO.Maps.Utils.checkIE6()){
			holder.className += ' coordinates_ie6';
		}
		
		holder.appendChild(this.element);
		this.div.appendChild(holder);
        
        this.redraw();
		
		if (!px) {
			this.div.style.right = (this.isMinimized? this.minRightPosition : 0) + 'px';
			this.div.style.bottom = '30px';
		}else{
			//this.div.style.left = px.x + 'px';
			this.div.style.right = (this.isMinimized? this.minRightPosition : 0) + 'px';
			this.div.style.top = px.y + 'px';
		}
		
        return this.div;
    },
	
	toggleControl: function(){
		this.toggle(!this.isMinimized, true);
	},
	
	stopBublingEvent: function(evt){
		OpenLayers.Event.stop(evt);
	},
   
    /**
     * Method: redraw  
     */
    redraw: function(evt){
		var lonLat;
		
		lonLat = this.map.getMapCenter();
		
		var newHtml = this.formatOutput(lonLat);
		
		if (newHtml != this.element.innerHTML) {
			this.element.innerHTML = newHtml;
		}
	},

    /**
     * Method: formatOutput
     * Override to provide custom display output
     *
     * Parameters:
     * lonLat - {<OpenLayers.LonLat>} Location to display
     */
    formatOutput: function(lonLat) {
        var digits = parseInt(this.numDigits, 10);
        var newHtml =
            this.prefix +
			lonLat.lat.toFixed(digits) +
            this.separator + 
			lonLat.lon.toFixed(digits) +
            this.suffix;
        return newHtml;
     },

    /** 
     * Method: setMap
     */
    setMap: function() {
        OpenLayers.Control.prototype.setMap.apply(this, arguments);
		if (!this.isMinimized) {
			this.registerListeners();
		}
    },  
	
	registerListeners: function(){
		this.map.events.register('move', this, this.redraw);
	},
	
	unregisterListeners: function(){
		this.map.events.unregister('move', this, this.redraw);
	},

    CLASS_NAME: "SAPO.Maps.Control.MapCenter"
});


/**
 * @author SAPO Maps
 */

//Create the spacebits namespace
var Spacebits = {};

//Global variables
var map = null;
var navigationControl = null;
var mapTypeControl = null;
var windowControls = [];

//Global functions

function resizeMap(){
	var mapDiv = document.getElementById('map');
	var consoleDiv = document.getElementById('console');
	var counterDiv = document.getElementById('counter');
	
	mapDiv.style.width = getClientWidth() + 'px';
	mapDiv.style.height = getClientHeight() + 'px';

	consoleDiv.style.left = 0;
	consoleDiv.style.top = (getClientHeight()-200) + 'px';
	consoleDiv.style.height = '200px';
	consoleDiv.style.width = '300px';

	counterDiv.style.left = '280px';
	counterDiv.style.top = (getClientHeight()-200) + 'px';
	counterDiv.style.height = '15px';
	counterDiv.style.width = '15px';
}

function getClientWidth() {
	return document.compatMode=='CSS1Compat' && !window.opera?document.documentElement.clientWidth:document.body.clientWidth;
};

function getClientHeight() {
	return document.compatMode=='CSS1Compat' && !window.opera?document.documentElement.clientHeight:document.body.clientHeight;
};

function saveWindowPosition(name, position){
  var cookieName = 'spacebits_' + name;
  var cookieValue = position.left + ',' + position.top;
  SAPO.Utility.Cookie.set(cookieName, cookieValue);
}

function setWindowPosition(name, window){
  var cookies = SAPO.Utility.Cookie.get();
  if(cookies && cookies['spacebits_' + name]){
    var cookie = cookies['spacebits_' + name];
    var cookiePosition = cookie.split(',');
    var windowPosition = new OpenLayers.Pixel(Number(cookiePosition[0]), Number(cookiePosition[1]));
    
    window.setWindowPosition(windowPosition);
  }
}


BrowserDetect.init();
