		<div id="navigation_wrapper">
			<ul id="navigation">
				<li><a class="mit" href="/" title="Home">Home</a></li>
				<li><a class="mit" href="/page/project.html" title="Project">Project</a></li>
				<li><a class="mit" href="/page/team.html" title="Team">Team</a></li>
				<li><a class="mit" href="/page/probe.html" title="Payload">Payload</a></li>
				<li><a class="mit" href="/page/balloon.html" title="Balloon">Balloon</a></li>
				<li><a class="mit" href="/multimedia" title="Multimedia">Multimedia</a></li>
				<li><a class="mit" href="/live" title="Dashboard">Dashboard</a></li>
			</ul>
			<ul id="account_wrapper">
{if $uid!=""}
  {if $editable}
    {if $action=='edit'}
				<li><a href="{$page}">Cancel</a></li>
    {else}
				<li><a onclick="return(confirm('Do you wan\'t to delete this page?'));" href="{$page}?delete&rand={$rand}">Delete</a></li>
				<li><a href="{$page}?edit&rand={$rand}">Edit</a></li>
    {/if}
  {/if}
				<li><a href="/logout">Logout</a></li>
{else}
				<li><a href="/login">Login</a></li>
{/if}
			</ul>
<!--
			<fieldset id="search_wrapper">
				<form action="#">
					<input type="text" value="Search" class="search" name="search" />
					<input type="submit" value="Search" class="hide" />
				</form>
			</fieldset>
-->
		</div>
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
