		<div id="navigation_wrapper">
			<ul id="navigation">
				<li><a href="/" title="Home">Home</a></li>
				<li><a href="/page/project.html" title="Project">Project</a></li>
				<li><a href="/page/team.html" title="Team">Team</a></li>
			</ul>
			<ul id="account_wrapper">
{if $uid!=""}
  {if $editable}
    {if $action=='edit'}
				<li><a href="{$uri}">Cancel</a></li>
    {else}
				<li><a href="{$uri}?edit&rand={$rand}">Edit</a></li>
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
