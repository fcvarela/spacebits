#!/servers/php/bin/php -q
<?php

if($radio=fopen("/dev/modem","r")) {
  if($log=fopen("spacebits.log","a")) {
    while($l=fgets($radio,4096)) {
      fflush($radio);
      ob_flush();
      preg_match_all('/\$\$[0-9,]+\#\#/',$l,$matches,PREG_SET_ORDER);
      if($matches) { 
        echo $l."\n";
        fwrite($log,$l);
        }
      }
    fclose($log);
    }
  fclose($radio);
  }

?>
