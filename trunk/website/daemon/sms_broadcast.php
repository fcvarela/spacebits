#!/servers/php/bin/php -q
<?php
require_once 'classes/XMPPHP/XMPP.php';
require_once 'classes/api.php';
require_once 'config/site.php';

set_time_limit(0);
error_reporting(1); 

$api=new Spacebits_API;

$balloon_id=2;


if($sms=$api->lastSMS($balloon_id)) {
  $stamp=md5($sms['lon'].$sms['lat']);
  if(@file_get_contents("/tmp/lastspacesms".$balloon_id)!=$stamp) {
    echo "There's an update. Broadcasting...\n";
    $body="BalloonId: ".$balloon_id." Lat: ".$sms['lat']." Lon: ".$sms['lon']." alt: ".$sms['alt']." nsats: ".$sms['nsats']." http://maps.google.com/maps?q=".$sms['lat'].",".$sms['lon'];
    foreach(split(" ",RECEPTORS) as $n) {
      echo "Broadcasting to ".$n."\n";
      exec("/servers/spacebits/daemon/send_sms.pl ".$n." '".$body."'");
      }
    file_put_contents("/tmp/lastspacesms".$balloon_id,$stamp);
    }
  }
