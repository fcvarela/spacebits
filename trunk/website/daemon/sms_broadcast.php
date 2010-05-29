#!/servers/php/bin/php -q
<?php
require_once 'classes/XMPPHP/XMPP.php';
require_once 'classes/api.php';
require_once 'config/site.php';

set_time_limit(0);
error_reporting(1); 

$api=new Spacebits_API;

if($sms=$api->lastSMS()) {
  $stamp=md5($sms['change'].$sms['nsats'].$sms['alt'].$sms['lon'].$sms['lat']);
  if(@file_get_contents("/tmp/lastspacesms")!=$stamp) {
    echo "There's an update. Broadcasting...\n";
    $body="Lat: ".$sms['lat']." Lon: ".$sms['lon']." alt: ".$sms['alt']." nsats: ".$sms['nsats']." http://maps.google.com/maps?q=".$sms['lat'].",".$sms['lon'];
    foreach(split(" ",RECEPTORS) as $n) {
      echo "Broadcasting to ".$n."\n";
      exec("/servers/spacebits/daemon/send_sms.pl ".$n." '".$body."'");
      }
    file_put_contents("/tmp/lastspacesms",$stamp);
    }
  }
