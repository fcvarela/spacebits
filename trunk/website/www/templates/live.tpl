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
<!--
    <div id='chart_div'></div>

    <script type='text/javascript' src='http://www.google.com/jsapi'></script>
    <script type='text/javascript'>
      google.load('visualization', '1', {packages:['gauge']});
      google.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Label');
        data.addColumn('number', 'Value');
        data.addRows(3);
        data.setValue(0, 0, 'Memory');
        data.setValue(0, 1, 80);
        data.setValue(1, 0, 'CPU');
        data.setValue(1, 1, 55);
        data.setValue(2, 0, 'Network');
        data.setValue(2, 1, 68);

        var chart = new google.visualization.Gauge(document.getElementById('chart_div'));
        var options = {width: 400, height: 120, redFrom: 90, redTo: 100,
            yellowFrom:75, yellowTo: 90, minorTicks: 5};
        chart.draw(data, options);
      }
    </script>
-->
{/literal}

      {include file="services.tpl"}

  {include file="main_container_end.tpl"}

{include file="footer.tpl"}
