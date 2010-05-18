<?php
require_once('classes/api.php');

$post=file_get_contents('php://input');

$api=new Spacebits_API;
$payload=$api->parsePayload($post);
$api->put($payload);

echo "<result>ok</result>\n";

?>
