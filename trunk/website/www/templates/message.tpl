{if $message}
<div id="message_wrapper" style="text-align:center;">
  <div id="message"></div>
<script type="text/javascript">
var left={$left}
{literal}
function showCounter() {
  left--;
  var lh=Math.floor(left/3600);
  var lm=Math.floor((left%3600)/60);
  var ls=Math.floor(left%60);
  $('message').innerHTML='<p><a href="/page/spacebits1live.html">Time to launch Spacebits 1: '+zp(lh)+':'+zp(lm)+':'+zp(ls)+'</a></p>';
  }
function zp(n) { n = n.toString(); while (n.length < 2) { n = '0' + n; } return n; }
setInterval(showCounter,1000);
</script>
{/literal}
 <div id="message_right">&nbsp;</div>
</div>
{/if}
