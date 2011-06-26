<?php
require_once('classes/api.php');
require_once('classes/spacebits.php');

$post=file_get_contents('php://input');
$api=new Spacebits_API;
$spacebits=new Spacebits;

header("Expires: " . date("D, j M Y H:i:s", time() - (5 * 60)) . " UTC");
header("Cache-Control: no-cache");
header("Pragma: no-cache");

list($method,$value)=split("/",$_GET['action'],2);

switch($method) {
  case "sms":
    $b=urldecode($_POST['body']);
    if($b[0]=='R') {
      list(,$gid,$id,$lat,$lon,$alt,$nsats,$pressure,$int_temp,$ext_temp,$humidity)=split(",",$b);
      $xml='<?xml version="1.0" encoding="UTF-8"?><balloon><id>'.$id.'</id><token>29v856792b29##/++9</token><atmosphere><pressure>'.$pressure.'</pressure><temp>'.$int_temp.'</temp><temp_int>'.$int_temp.'</temp_int><temp_ext>'.$ext_temp.'</temp_ext><light>0</light><humidity>'.$humidity.'</humidity></atmosphere><rtc>0</rtc><geo><lat>'.$lat.'</lat><lon>'.$lon.'</lon><alt>'.$alt.'</alt><bear>0</bear></geo><imu><gx>0</gx><gy>0</gy><ax>0</ax><ay>0</ay><az>0</az></imu><gsm><registered>0</registered><ready>0</ready></gsm></balloon>';
      $b=date("Y/m/d H:i:s",time())." radio sms from gs: ".$id.",".$lat.",".$lon.",".$alt.",".$nsats.",".$pressure.",".$int_temp.",".$ext_temp.",".$humidity;
      if($payload=$api->parsePayload($xml)) {
        $api->put($payload);
        }
      }
      else // normal ground station id
      {
      list($id,$lat,$lon,$alt,$nsats)=split(",",$b);
      $b=date("Y/m/d H:i:s",time())." payload sms: ".$id.",".$lat.",".$lon.",".$alt.",".$nsats;
      $api->saveSMS(urldecode($_POST['from']),$b);
      }
    file_put_contents("/servers/spacebits/www/logs/sms.log",$b."\n",FILE_APPEND);
    break;
  case "put":
    header("Content-Type: text/xml; charset=utf-8");
    file_put_contents("/tmp/spacepost.xml",print_r($post,true)."\n\n",FILE_APPEND);
    if($payload=$api->parsePayload($post)) {
      $api->put($payload);
      echo "<result>ok</result>\n";
      }
      else
      {
      echo "<result>fail</result>\n";
      }
    break;
  case "get":
    header("Content-Type: application/json; charset=utf-8");
    echo json_encode($api->get(false,$value));
    break;
  case "balloons":
    header("Content-Type: application/json; charset=utf-8");
    echo json_encode($api->getBalloons());
    break;
  case "track":
    header("Content-Type: application/json; charset=utf-8");
    echo json_encode($api->track());
    break;
  case "twits":
    header("Content-Type: application/json; charset=utf-8");
    echo json_encode($api->getTwits());
    break;
  case "rss":
    header('Content-type: text/xml; charset=utf-8');
    echo $api->makerss($spacebits->getblogposts());
    break;
  case "all":
    header("Content-Type: application/json; charset=utf-8");
    if($value) {
      $active_balloons=array($value);
      }
    echo json_encode(array('last'=>$api->get($_POST['demo']),'track'=>$api->track(),'twitter'=>$api->getTwits()));
    break;
  default:
    header("Content-Type: text/xml; charset=utf-8");
    echo "<result>unknown method</result>\n";
    break;
 }

?>
