<script type="text/javascript">
var uri="{$uri}";
{literal}
elements = document.getElementsByClassName('mit');
for ( var i=0, len=elements.length; i<len; ++i ){
  var parts=(elements[i].href.split("/"));
  var path=elements[i].href.substr(7+parts[2].length);
  if(uri==path) elements[i].className = 'mit active';
  }
</script>
{/literal}

{if $ipad==""}
	<div id="footer_wrapper">
		<div id="footer">
			<div class="logo"><span></span></div>
			<ul>
				<li class="twitter"><a href="http://twitter.com/flyspacebits" title="Twitter">Twitter</a></li>
				<li class="facebook"><a href="http://facebook.com/group.php?gid=241097977251" title="Facebook">Facebook</a></li>
				<li class="rss"><a href="http://friendfeed.com/spacebits?format=atom" title="RSS">RSS</a></li>
			</ul>
		</div>
	</div>
{/if}

{literal}
<script type="text/javascript" src="http://js.sapo.pt/SAPOWebAnalytics/"></script>        
<script> SAPO.WebAnalytics.track({swakt:"305CBF64-3BE3-4DAB-B2C2-DB0001ACA942"}); </script>
{/literal}

{if $action=='page'}
  {include file="disqus_footer.tpl"}
{/if}

</body>
</html>
