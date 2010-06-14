<div id="main_image" style="height:408px;">
	<a href="/page/spacebits1live.html">
<img id="bigimage1" src="/images/bigpic1.jpg" width="860" height="408" alt="Main Image" />
<img id="bigimage2" style="display:none;" src="/images/bigpic2.jpg" width="860" height="408" alt="Main Image" />
</a>
	<div id="frame_image_top"></div><div id="frame_image_bottom"></div>
	<hr />
</div>
<script type="text/javascript">
{literal}
start_slideshow(1, 2, 7000);

function start_slideshow(start_frame, end_frame, delay) {
  setTimeout(switch_slides(start_frame,start_frame,end_frame, delay), delay);
  }
                            
function switch_slides(frame, start_frame, end_frame, delay) {
  return (function() {
    Effect.Fade('bigimage' + frame);
    if (frame == end_frame) { frame = start_frame; } else { frame = frame + 1; }
    setTimeout("Effect.Appear('bigimage" + frame + "');", 850);
    setTimeout(switch_slides(frame, start_frame, end_frame, delay), delay + 850);
    })
  }
{/literal}
</script>
