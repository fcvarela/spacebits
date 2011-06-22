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
    list($id,$lat,$lon,$alt,$nsats)=split(",",$b);
    $b=$id.",".$lat.",".$lon.",".$alt.",".$nsats;
    file_put_contents("/servers/spacebits/www/logs/sms.log",$b."\n",FILE_APPEND);
    $api->saveSMS(urldecode($_POST['from']),$b);
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
    echo json_encode(array('last'=>$api->get($_POST['demo']),'track'=>$api->track(),'twitter'=>$api->getTwits()));
    break;
  default:
    header("Content-Type: text/xml; charset=utf-8");
    echo "<result>unknown method</result>\n";
    break;
 }

?>
