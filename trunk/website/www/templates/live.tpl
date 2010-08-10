{include file="header.tpl"}

  {include file="main_container_begin.tpl"}

{if $ipad==""}
      {include file="topmenu.tpl"}

      {include file="message.tpl"}

<!--
<div id="articlecontent" style="background:#181b0d;">
This will be the Live Dashboard for the official launch of <a href="/page/spacebits1live.html">Spacebits 1</a> which is scheduled for the 30th of May, 2010, 11AM UTC/GMT+1. Now in demo mode.
</div>
-->

{/if}

{if $ipad}
<div id="map_wrapper" style="float:left;width:840px;height:530px;margin-left:30px;"><div style="width:100%;height:530px;" id="map"></div></div>
<div id="twitter_wrapper" style="height:522px;">Twitter</div>
{literal}
<script type="text/javascript">
useGauges = false;
</script>
{/literal}
<div id="sensors" style="margin-right:27px;margin-left:25px;height:100px;">
  <div id="buttons" style="float:right;width:250px;margin-top:5px;">
      <ol class="on_off">
      <li>
        <label class="left" for="sw_follow">Follow Balloon</label>
        <input type="checkbox" id="sw_follow" />
      </li>
      <li>
        <label class="left" for="sw_sms">Track SMS</label>
        <input type="checkbox" id="sw_sms" />
      </li>
      <li>
        <label class="left" for="sw_radio">Track Radio</label>
        <input type="checkbox" id="sw_radio" />
      </li>
      <li>
        <label class="left" for="sw_twitter">Twitter feed</label>
        <input type="checkbox" id="sw_twitter" />
      </li>
<!--
      <li>
        <label class="left" for="sw_demo">Demo mode</label>
        <input type="checkbox" id="sw_demo" />
      </li>
-->
      </ol>
  </div>
  <div id="ipads" style="float:left;width:600px;">
    <div id='sensors_alt' style="float:left;"></div>
    <div id='sensors_pressure' style="float:left;"></div>
    <div id='sensors_temperature' style="float:left;"></div>
    <div id='sensors_temperature_ext' style="float:left;"></div>
    <div id='sensors_humidity' style="float:left;"></div>
    <div id='bear' style="float:left;"></div>
    <br clear="all"/>
    <div id="ax" style="float:left;"></div>
    <div id="ay" style="float:left;"></div>
    <div id="az" style="float:left;"></div>
    <div id="gx" style="float:left;"></div>
    <div id="gy" style="float:left;"></div>
    <br clear="all"/>
    <div id="time" style="float:left;"></div>
    <br clear="all"/>
    <div id="volts" style="float:left;"></div>
    <div id="amps" style="float:left;"></div>
  </div>
</div>
{else}
<div id="map_wrapper" style="float:left;width:840px;margin-left:30px;"><div id="map" style="width:100%;height:400px;"></div></div>
<div id="twitter_wrapper" style="height:392px;">Twitter</div>
<br clear="all"/>
<div id="buttons" style="float:right;width:250px;margin-top:5px;margin-right:27px;">
  <ol class="on_off">
  <li>
    <label class="left" for="sw_follow">Follow Balloon</label>
    <input type="checkbox" id="sw_follow" />
  </li>
  <li>
    <label class="left" for="sw_sms">Track SMS</label>
    <input type="checkbox" id="sw_sms" />
  </li>
  <li>
    <label class="left" for="sw_radio">Track Radio</label>
    <input type="checkbox" id="sw_radio" />
  </li>
  <li>
    <label class="left" for="sw_twitter">Twitter feed</label>
    <input type="checkbox" id="sw_twitter" />
  </li>
<!--
   <li>
    <label class="left" for="sw_demo">Demo mode</label>
    <input type="checkbox" id="sw_demo" />
  </li>
-->
  </ol>
<ol>
<li>
<label class="left" for="blid">Balloon</label>
<select id="blid" name="balloons" onChange="swapBalloon();return(false);">
{foreach from=$balloons item=b name=f}
<option value="{$b.id}" {if $b.id==2}selected{/if}>{$b.name}</option>
{/foreach}
</select>
</li>

</ol>
</div>
<div id="sensors" style="float:left;width:600px;margin-left:25px;height:210px;">
  <div id='sensors_alt' style="float:left;"></div>
  <div id='sensors_pressure' style="float:left;"></div>
  <div id='sensors_temperature' style="float:left;"></div>
  <div id='sensors_temperature_ext' style="float:left;"></div>
  <div id='sensors_humidity' style="float:left;"></div>
  <div class="measure" id="ax"></div>
  <div class="measure" id="ay"></div>
  <div class="measure" id="az"></div>
  <div class="measure" id="gx"></div>
  <div class="measure" id="gy"></div>
  <br/>
  <div class="measure" id="time"></div>
  <br/>
  <div id="volts" style="float:left;"></div>
  <div id="amps" style="float:left;"></div>
  <div id='bear' style="float:left;"></div>
</div>
{/if}


<br clear="all"/>

  {include file="main_container_end.tpl"}

{include file="footer.tpl"}
