<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Spacebits</title>
<link href="/stylesheets/common.css" rel="stylesheet" type="text/css" />
{if $action=='edit'}
<script type="text/javascript" src="/ckeditor/ckeditor.js"></script>
{/if}
{$action}
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

