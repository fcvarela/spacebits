{include file="header.tpl" title=$title}
  {include file="main_container_begin.tpl"}

      {include file="topmenu.tpl"}

<div id="bo">
   <h2>Backoffice<span></span></h2>
   <div id="bocontent">
      <b>Articles</b>
      <ul>
      {foreach from=$articles item=a name=outera}
<li><a href="/page/{$a.id}">{$a.title}</a> {foreach from=$a.tags item=tag name=outertag}{$tag} {/foreach}</li>
      {/foreach}
      </ul>

   </div>
</div>

      {include file="services.tpl"}

 {include file="main_container_end.tpl"}
{include file="footer.tpl"}
