{include file="header.tpl"}

  {include file="main_container_begin.tpl"}

  <div id="article">
    <h2>Login</h2>
    <div id="articlecontent">

<p>
Use this form log into the system.
<p>

<form method="post" action="/login">
Login<br/>
<input name="login" type="text" style="width:80px;" value=""/><br/>
Password<br/>
<input name="password" type="password" style="width:80px;" value=""/><br/>
<br/>
<input name="go" type="submit" value="Enter"/>
</form>

    </div>
  </div>

  {include file="main_container_end.tpl"}

{include file="footer.tpl"}
