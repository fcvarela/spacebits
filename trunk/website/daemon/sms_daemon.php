#!/servers/php/bin/php -q
<?php
require_once 'classes/XMPPHP/XMPP.php';
require_once 'classes/api.php';
require_once 'config/site.php';

set_time_limit(0);
error_reporting(1); 

list($uname)=explode(".local",php_uname('n'));
$conn = new XMPPHP_XMPP(XMPP_SERVER, 5222, XMPP_USER, XMPP_PASSWORD, $uname, XMPP_DOMAIN, $printlog=true, $loglevel=XMPPHP_Log::LEVEL_ERROR);
$conn->autoSubscribe();
$api=new Spacebits_API;


try {
  $conn->connect();
  while(!$conn->isDisconnected()) {
    $payloads = $conn->processUntil(array('message', 'presence', 'end_stream', 'session_start', 'vcard'));
    foreach($payloads as $event) {
      $pl = $event[1];
      switch($event[0]) {
        case 'message': 
          echo "Message from: {$pl['from']}\n";
          $api->saveSMS($pl['from'],$pl['body']);
          break;
        case 'session_start':
          print "Session Start\n";
          $conn->presence($status="Balloon is up");
          break;
      }
    }
  }
} catch(XMPPHP_Exception $e) {
    die($e->getMessage());
}
