{include file="header.tpl" title=$title}
  {include file="main_container_begin.tpl"}

      {include file="topmenu.tpl"}

<div id="blog">
   <h2>Blog<span></span></h2>
   <div id="blogcontent">

      <a href="http://spacebits.eu/api/rss"><img style="float:right;border:0;" src="/images/bigfeedicon.png"/></a>

      {foreach from=$posts item=post name=outerpost}
      <div class="blogpost">
      <div class="blogposttitle"><a href="/page/{$post.id}">{$post.title}</a></div>
      <div class="blogpostteaser">{$post.teaser} (<a href="/page/{$post.id}">more</a>)</div>
      <div class="blogpostby">(by {$post.user} on {$post.date}. tags: {foreach from=$post.tags item=tag name=outertag}{if $tag!='blog'}{$tag} {/if}{/foreach})</div>
      </div>
      {/foreach}

   </div>
</div>

      {include file="services.tpl"}

 {include file="main_container_end.tpl"}
{include file="footer.tpl"}
