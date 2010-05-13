<div id="article">
{if $action=='edit'}
<form method="POST" name="savearticle" action="{$page}?save">
<input type="text" name="articletitle" value="{$title}" style="width:600px;"/>
{else}
   <h2>{$title}<span></span></h2>
{/if}

{if $action=='edit'}
   <textarea name="articlecontent">
{else}
   <div id="articlecontent">
{/if}

{$body}

{if $action=='edit'}
  </textarea>
{literal}
<script type="text/javascript">
CKEDITOR.replace( 'articlecontent',
    {
	height: 400,
        filebrowserImageBrowseUrl : '/page/0?browser'
    });
</script>
{/literal}
</form>
{else}
  </div>
{/if}

</div>

