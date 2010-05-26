{include file="header.tpl"}

  {include file="main_container_begin.tpl"}

  <div id="article">
    <div id="articlecontent">

      <div id="files" style="float:left;">
    {foreach from=$files item=file name=f}
      {if $smarty.foreach.f.index>$offset && $smarty.foreach.f.index<=$offset+10}
        <a href="javascript:select_image('/uploads/{$file}');return(false);"><img style="width:150px;" src="/uploads/{$file}"></a>
      {/if}
    {/foreach}
      </div>

    <br/>

    {if $offset>0}
      <a href="/page/0?browser&{$suffix}">|<</a>
      <a href="/page/{$offset-10}?browser&{$suffix}"> << </a>
    {/if}

    {if $offset+10<$count}
      <a href="/page/{$offset+10}?browser&{$suffix}"> >> </a>
      <a href="/page/{$count-10/10*10}?browser&{$suffix}">>|</a>
    {/if}

    </div>
  </div>

  {include file="main_container_end.tpl"}

{include file="footer.tpl"}
