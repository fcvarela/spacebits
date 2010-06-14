<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
{if $title}
<title>Spacebits: {$title}</title>
{else}
<title>Spacebits</title>
{/if}
<link href="/stylesheets/common.css?v=8" rel="stylesheet" type="text/css"/>
<link rel="alternate" type="application/rss+xml" title="Spacebits" href="http://spacebits.eu/api/rss"/>
<script type="text/javascript" src="/js/base.js"></script>
<script type="text/javascript" src="/js/prototype.js"></script>
<script src="/js/scriptaculous.js?load=effects" type="text/javascript"></script>
<script src="/js/transition.js" type="text/javascript"></script>
{if $uri=="/live"}
<script type='text/javascript' src='http://www.google.com/jsapi'></script>
<script type="text/javascript" src="http://js.sapo.pt/Bundles/SAPOMapsAPI.js"></script>
<script src="/js/iphone-style-checkboxes.js" type="text/javascript"></script>
<script src="/js/live.js?v8" type="text/javascript"></script>
<link rel="stylesheet" href="/stylesheets/buttons.css?v4" type="text/css" media="screen"/>
{/if}
{if $action=='edit' && $uid!=""}
<script type="text/javascript" src="/ckeditor/ckeditor.js"></script>
{/if}
{if $action=='browser'}
{literal}
<script type="text/javascript">
function select_image(f) {
var CKEditorFuncNum = {/literal}{$func}{literal};
window.parent.opener.CKEDITOR.tools.callFunction( CKEditorFuncNum, f, '' );
self.close();
}
</script>
{/literal}
{/if}
{if $ipad}
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
{/if}
</head>
{if $uri=="/live"}
<body onload="initDashboard();" {if $ipad}ontouchmove="BlockMove(event);"{/if}>
{else}
<body>
{/if}
