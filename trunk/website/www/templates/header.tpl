<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>Spacebits</title>
<link href="/stylesheets/common.css?v=3" rel="stylesheet" type="text/css" />
<link rel="alternate" type="application/rss+xml" title="Spacebits" href="http://friendfeed.com/spacebits?format=atom">
<script type="text/javascript" src="http://js.sapo.pt/SAPO"></script>
<script type="text/javascript" src="http://js.sapo.pt/SAPO/Utility/Dumper"></script>
{if $uri=="/live"}
<script type='text/javascript' src='http://www.google.com/jsapi'></script>
<script type="text/javascript" src="http://js.sapo.pt/Bundles/SAPOMapsAPI.js"></script>
<script type="text/javascript" src="/js/prototype.js"></script>
<script src="/js/scriptaculous.js" type="text/javascript"></script>
<script src="/js/iphone-style-checkboxes.js" type="text/javascript"></script>
<link rel="stylesheet" href="/stylesheets/buttons.css?v2" type="text/css" media="screen" />
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

</head>
<body>

