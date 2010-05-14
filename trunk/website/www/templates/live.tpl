{include file="header.tpl"}

  {include file="main_container_begin.tpl"}

      {include file="topmenu.tpl"}

<div id="articlecontent" style="background:#181b0d;">
This will be the Live Dashboard for the official launch of Spacebits which is scheduled for the 30th of May, 2010, 11AM. This page is still incomplete though. It'll have a real time map with the Balloon's position and live values from all sensors, and more.
</div>
{literal}
<div id="map_wrapper" style="margin-left:30px;"><div id="map" style="width:840px;height:400px;"></div></div>
<script type='text/javascript'>
  var map = false;
  function init(){
    map = new SAPO.Maps.Map('map');
    }
			
  function move(){
    map.pan(-100, 100, {animate: true, dragging: true});
    }
init();
</script>
<!--
<input type="button" onclick="move();" value="Move" />
-->

{/literal}


      {include file="services.tpl"}

  {include file="main_container_end.tpl"}

{include file="footer.tpl"}
