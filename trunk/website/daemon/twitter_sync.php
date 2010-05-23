#!/servers/php/bin/php -q
<?php
require_once('classes/simplepie/simplepie_113.inc');
require_once 'classes/api.php';
require_once 'config/site.php';

set_time_limit(0);
error_reporting(1); 

$api=new Spacebits_API;

$pie = new SimplePie();
echo "Getting $url\n";
$xml=file_get_contents("http://twitter.com/statuses/user_timeline/102699759.rss");
$pie->set_raw_data($xml);
$pie->init();
$pie->handle_content_type();
if($pie->get_items()) {
  foreach ($pie->get_items() as $item) {
    echo $item->get_description()."\n";
    echo $item->get_link()."\n";
    echo strtotime($item->get_date())."\n\n";
    $api->putTwit(array('description'=>$item->get_description(),'change'=>strtotime($item->get_date()),'link'=>$item->get_link()));
    }
  }

