#!/servers/php/bin/php -q
<?php
require_once('classes/api.php');

$xml='<balloon>
	<token>29v856792b29##/++9</token>
        <id>6</id>
	<power>
		<current>1024</current>
		<voltage>1024</voltage>
	</power>
	<atmosphere>
		<pressure>2048</pressure>
		<temp>2048</temp>
		<humidity>1024</humidity>
		<dust_density>1024</dust_density>
	</atmosphere>
	<rtc>'.date("h:i:s").'</rtc>
	<geo>
		<lat>40.321161</lat>
		<lon>-7.61367</lon>
		<alt>2000</alt>
		<bear>1024</bear>
	</geo>
	<imu>
		<gx>1024</gx>
		<gy>1024</gy>
		<ax>1024</ax>
		<ay>1024</ay>
		<az>1024</az>
	</imu>
</balloon>
';


  $login='http://spacebits.eu/api/put';
  $ch = curl_init($login);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
  curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
  ob_start();
  $output=curl_exec($ch);
  curl_close($ch);
  ob_end_clean();
  echo $output;

?>
