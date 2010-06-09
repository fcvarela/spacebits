<div id="comments" {if $hidden=="1"}style="display:none;"{/if}>
{literal}
<h2>Comments<span></span></h2>
<div id="commentscontent">
<div id="disqus_thread"></div>
<script type="text/javascript">
  var disqus_developer=0;

  (function() {
   var dsq = document.createElement('script'); dsq.type = 'text/javascript'; dsq.async = true;
   dsq.src = 'http://spacebits.disqus.com/embed.js';
   (document.getElementsByTagName('head')[0] || document.getElementsByTagName('body')[0]).appendChild(dsq);
  })();</script>
<noscript>Please enable JavaScript to view the <a href="http://disqus.com/?ref_noscript=spacebits">comments powered by Disqus.</a></noscript><a href="{$uri}#disqus_thread">Comments</a>
<a href="http://disqus.com" class="dsq-brlink" style="float:right;">blog comments powered by <span class="logo-disqus">Disqus</span></a>
</div>
</div>
{/literal}
