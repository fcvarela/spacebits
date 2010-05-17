#!/usr/bin/env php
<?php

$broker = broker_init("127.0.0.1", 3323, 0, 1);
$ret = broker_subscribe_topic($broker, "/spacebits/telemetry");

// consume without auto-ack
while (1) {
	if (($msg = broker_receive($broker, 1000, true)) !== false) {
    	post_msg($msg["payload"]);
    }
}

function post_msg($xml) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, "http://spacebits.eu/api");
    curl_setopt ($ch, CURLOPT_HTTPHEADER, array("Content-Type: text/xml"));
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
    $result = curl_exec($ch);
    curl_close($ch);
}
?>
