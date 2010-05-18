<?php
/**
 * @author Celso Martinho
 * @version 1.0
 * @package API
 */
include('config/site.php');

setlocale(LC_ALL, 'pt_PT');

class Spacebits_API {

  function __construct() {
    $this->api_secret=API_SECRET;
    $this->db='sqlite:../db/site.db';
    list($uri)=explode("?",$_SERVER["REQUEST_URI"]);
    list($uri)=explode("&",$uri);
    }

  function parsePayload($xml) {
    $p=array();
    $arr=simplexml_load_string($xml);
    $p['power_current']=floatval($arr->power->current);
    $p['power_voltage']=floatval($arr->power->voltage);
    $p['pressure']=floatval($arr->atmosphere->pressure);
    $p['temperature']=floatval($arr->atmosphere->temp);
    $p['humidity']=floatval($arr->atmosphere->humidity);
    $p['dust_density']=floatval($arr->atmosphere->dust_density);
    $p['lat']=floatval($arr->geo->lat);
    $p['lon']=floatval($arr->geo->lon);
    $p['alt']=floatval($arr->geo->alt);
    $p['bear']=floatval($arr->geo->bear);
    $p['imu_gx']=floatval($arr->imu->gx);
    $p['imu_gy']=floatval($arr->imu->gy);
    $p['imu_ax']=floatval($arr->imu->ax);
    $p['imu_ay']=floatval($arr->imu->ay);
    $p['imu_az']=floatval($arr->imu->az);
    $p['change']=strtotime($arr->rtc);
    return($p);
    }

  function put($values=array()) {
    GLOBAL $sconfig;
    if($db = new PDO($this->db)) {
      $sql="INSERT INTO data (change,power_current,power_voltage,pressure,temperature,humidity,dust_density,lat,lon,alt,bear,imu_gx,imu_gy,imu_ax,imu_ay,imu_az) VALUES(".$values['change'].",".$values['power_current'].",".$values['power_voltage'].",".$values['pressure'].",".$values['temperature'].",".$values['humidity'].",".$values['dust_density'].",".$values['lat'].",".$values['lon'].",".$values['alt'].",".$values['bear'].",".$values['imu_gx'].",".$values['imu_gy'].",".$values['imu_ax'].",".$values['imu_ay'].",".$values['imu_az'].")";
      $q = $db->prepare($sql);
      $q->execute();
      }
    }
}
