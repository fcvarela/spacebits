#!/servers/php/bin/php -q
<?php
require_once 'classes/XMPPHP/XMPP.php';
require_once 'classes/api.php';
require_once 'config/site.php';

set_time_limit(0);
error_reporting(1); 

$api=new Spacebits_API;


$track=$api->track();

foreach($active_balloons as $id) {
  foreach($balloons as $b) {
    if($b['id']==$id) {
      foreach($sms_list[$id] as $phone) {
        echo "Sending coordinate for balloon $id to phone $phone\n";
        $t=$api->get(false,$id);
        if($t['lat']) {
          $stamp=md5($phone.$t['lat'].$t['lon'].$t['alt']);
          $stamp_file="/tmp/spacebits".$stamp.".stamp";
          if(!@file_exists($stamp_file)) {
            $body="BalloonId: ".$id." Lat: ".$t['lat']." Lon: ".$t['lon']." alt: ".$t['alt']." http://maps.google.com/maps?q=".$t['lat'].",".$t['lon'];
            echo "  ".$body."\n";
            exec("/servers/spacebits/daemon/send_sms.pl ".$phone." '".$body."'");
            touch($stamp_file);
            }
          }
        }
      }
    }
  }

exit;
