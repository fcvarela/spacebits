<?php
require_once('classes/api.php');

$post=file_get_contents('php://input');
$api=new Spacebits_API;

header("Expires: " . date("D, j M Y H:i:s", time() - (5 * 60)) . " UTC");
header("Cache-Control: no-cache");
header("Pragma: no-cache");

switch($_GET['action']) {
  case "put":
    header("Content-Type: text/xml; charset=utf-8");
    file_put_contents("/tmp/spacepost.xml",print_r($post,true),FILE_APPEND);
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
    echo json_encode($api->get());
    break;
  case "track":
    header("Content-Type: application/json; charset=utf-8");
    echo json_encode($api->track());
    break;
  case "twits":
    header("Content-Type: application/json; charset=utf-8");
    echo json_encode($api->getTwits());
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
