<?php
require_once('classes/api.php');

$post=file_get_contents('php://input');
$api=new Spacebits_API;


switch($_GET['action']) {
  case "put":
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
  default:
    echo "<result>unknown method</result>\n";
    break;
 }

?>
