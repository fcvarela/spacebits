<html>
<body>
<pre>
<?php

$lines=explode("\n",file_get_contents("sms.log"));
$j=0;
for($i=sizeof($lines)-1;$i--;$i>0) {
  echo $lines[$i]."\n";
  $j++;
  if($j>50 && $_GET['showall']=='') break;
  }

?>
</pre>
<a href="/logs/?showall=1">show all lines</a>
</body>
</html>
