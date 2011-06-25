#!/servers/php/bin/php -q
<?php
require_once 'classes/XMPPHP/XMPP.php';
require_once 'classes/api.php';
require_once 'classes/tools.php';
require_once 'config/site.php';

set_time_limit(0);
error_reporting(1); 

$api=new Spacebits_API;


$track=$api->track();
$tools=new SAPO_Tools;

foreach($active_balloons as $id) {
  foreach($balloons as $b) {
    if($b['id']==$id) {
      foreach($sms_list[$id] as $phone) {
        echo "Sending coordinate for balloon $id to phone $phone\n";
        $t=$api->get(false,$id);
        if($t['lat']) {
          $stamp=md5($phone.$t['lat'].$t['lon'].$t['alt'].$t['change']);
          $stamp_file="/tmp/spacebits".$stamp.".stamp";
          $apicall="http://services.sapo.pt/GIS/GetLocationByCoordinates?latitude=".$t['lat']."&longitude=".$t['lon'];

          $mun=null;
          $par=null;
          if($out=$tools->FileGetContents($apicall,false)) {
            if($xout=simplexml_load_string($out)) {
              $mun=$tools->Strip($xout->GetLocationByCoordinatesResult->MunicipalityName);
              $par=$tools->Strip($xout->GetLocationByCoordinatesResult->ParishName);
              }
            }

          if(!@file_exists($stamp_file)) {
            $body=substr($t['lat'].",".$t['lon']." ".intval($t['alt'])."m http://maps.google.com/maps?q=".$t['lat'].",".$t['lon']." ".$mun.", ".$par,0,139);
            echo "  ".$body." (".strlen($body).")\n";
            exec("/servers/spacebits/daemon/send_sms.pl ".$phone." '".$body."'");
            touch($stamp_file);
            }
          }
        }
      }
    }
  }

exit;
