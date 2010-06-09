
{if $fbc_channel==''}

{include file="header.tpl" title=$title}
  {include file="main_container_begin.tpl"}

      {include file="topmenu.tpl"}

      {include file="article.tpl"}

      {include file="services.tpl"}

 {include file="main_container_end.tpl"}
{include file="footer.tpl"}

{else}
<html><head></head><body>
Please wait... validating authentication.
{include file="disqus.tpl" hidden="1"}
{/if}

