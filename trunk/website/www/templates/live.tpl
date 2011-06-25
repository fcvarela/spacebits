<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>Spacebits Live Dashboard</title>
<link rel="alternate" type="application/rss+xml" title="Spacebits" href="http://spacebits.eu/api/rss"/>
<script type="text/javascript" src="/js/base.js"></script>
<script type="text/javascript" src="/js/prototype.js"></script>
<script src="/js/scriptaculous.js?load=effects" type="text/javascript"></script>
<script src="/js/transition.js" type="text/javascript"></script>
<script type='text/javascript' src='http://www.google.com/jsapi'></script>
<script type="text/javascript" src="http://js.sapo.pt/Bundles/SAPOMapsAPI.js"></script>
<script src="/js/iphone-style-checkboxes.js" type="text/javascript"></script>
<script src="/js/live.js?v10" type="text/javascript"></script>
<link rel="stylesheet" href="/stylesheets/live.css?v1" type="text/css" media="screen"/>
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black">
<link rel="apple-touch-icon" href="/images/spacebits.png"/>
<meta name="viewport" content="user-scalable=no, width=device-width" />
<link rel="apple-touch-startup-image" href="http://spacebits.eu/images/spacesplash.png" />
{literal}
<script>
function BlockMove(event) {
event.preventDefault() ;
}
</script>
{/literal}
</head>
<body onload="initDashboard();" ontouchmove="BlockMove(event);">
<div id="counter" style="background-color:#eeeeee;z-index:2000;position:absolute;opacity:0.8;filter:alpha(opacity=80);"></div>
<div id="console" style="background-color:#eeeeee;z-index:1000;position:absolute;opacity:0.8;filter:alpha(opacity=80);">console</div>
<div id="map"></div>
</body>
</html>
