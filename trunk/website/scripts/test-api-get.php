#!/servers/php/bin/php -q
<?php
require_once('classes/api.php');

$api=new Spacebits_API;
print_r($api->get(false,6));

?>
