<?php
/**
 * @author Celso Martinho
 * @version 1.0
 * @package API
 */
include('config/site.php');

setlocale(LC_ALL, 'en_US');

class Spacebits_API {

  function __construct() {
    $this->api_secret=API_SECRET;
    $this->db=DB_DATA;
    $this->sms_db=DB_SMS;
    list($uri)=explode("?",$_SERVER["REQUEST_URI"]);
    list($uri)=explode("&",$uri);
    }

  function parsePayload($xml) {
    if($arr=@simplexml_load_string($xml)) {
      if($this->api_secret!=$arr->token) return(false);
      $p=array();
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
    return(false);
    }

  function put($values=array()) {
    GLOBAL $sconfig;
    if($db = new PDO($this->db)) {
      $sql="INSERT INTO data (change,power_current,power_voltage,pressure,temperature,humidity,dust_density,lat,lon,alt,bear,imu_gx,imu_gy,imu_ax,imu_ay,imu_az) VALUES(".$values['change'].",".$values['power_current'].",".$values['power_voltage'].",".$values['pressure'].",".$values['temperature'].",".$values['humidity'].",".$values['dust_density'].",".$values['lat'].",".$values['lon'].",".$values['alt'].",".$values['bear'].",".$values['imu_gx'].",".$values['imu_gy'].",".$values['imu_ax'].",".$values['imu_ay'].",".$values['imu_az'].")";
      $q = $db->prepare($sql);
      $q->execute();
      }
    }

  function saveSMS($source,$message) {
    if($db = new PDO($this->sms_db)) {
      $sql="INSERT INTO sms (change,source,message) VALUES(".time().",".$db->quote($source).",".$db->quote($message).")";
      $q = $db->prepare($sql);
      $q->execute();
      }
    }

  function lastSMS() {
    $p=array();
    if($db = new PDO($this->sms_db)) {
      $sql="select change,message from sms order by change desc limit 1";
      $q = $db->prepare($sql);
      $q->execute();
      if($r=$q->fetch(PDO::FETCH_ASSOC)) {
        $p['change']=$r['change'];
        list($p['lat'],$p['lon'],$p['alt'],$p['nsats'])=split(",",$r['message']);
        }
      }
    return($p);
    }

  function get($demo=false) {
    $p=array();
    if($demo) {
      $p['change']=time();
      $p['power_voltage']=sprintf("%.1f",(float)(rand(0,50)/10));
      $p['power_current']=sprintf("%.1f",(float)(rand(0,20)/10));
      $p['imu_ax']=sprintf("%.1f",(float)(rand(0,10)/10));
      $p['imu_ay']=sprintf("%.1f",(float)(rand(0,10)/10));
      $p['imu_az']=sprintf("%.1f",(float)(rand(0,10)/10));
      $p['imu_gx']=rand(0,90);
      $p['imu_gy']=rand(0,90);
      $p['temperature']=rand(-50,10);
      $p['alt']=rand(0,40000);
      $p['humidity']=rand(0,100);
      $p['dust_density']=rand(0,100);
      $p['pressure']=rand(10,1000);
      $p['lon']=-8.0919+(float)(rand(-500,500)/1000);
      $p['lat']=37.7616+(float)(rand(-500,500)/1000);
    }
    else
    {
      if($db = new PDO($this->db)) {
        $sql="SELECT change,power_current,power_voltage,pressure,temperature,humidity,dust_density,lat,lon,alt,bear,imu_gx,imu_gy,imu_ax,imu_ay,imu_az FROM data ORDER BY change DESC LIMIT 1";
        $q = $db->prepare($sql);
        $q->execute();
        if($r=$q->fetch(PDO::FETCH_ASSOC)) {
          foreach(array_keys($r) as $key) {
            $p[$key]=$r[$key];
            }
          $sms=$this->lastSMS();
          if(intval($sms['change'])>intval($p['change'])) {$p['lat']=$sms['lat']; $p['lon']=$sms['lon'];}
          }
       }
    }
    $p['elapsed']='00:00:01';
    return($p);
    }

  function track() {
    $p=array();
    if($db = new PDO($this->db)) {
      $sql="select distinct lat,lon  from data where lat>20 and lat<60 and lon<-1 and lon>-12 order by change desc,lat,lon limit 100";
      $q = $db->prepare($sql);
      $q->execute();
      $t=array(); 
      while($r=$q->fetch(PDO::FETCH_ASSOC)) {
        array_push($t,array($r['lat'],$r['lon']));
        }
      }
    if($db = new PDO($this->sms_db)) {
      $sql="select message,change from sms order by change desc limit 100";
      $q = $db->prepare($sql);
      $q->execute();
      $t2=array(); 
      while($r=$q->fetch(PDO::FETCH_ASSOC)) {
        list($lat,$lon)=split(",",$r['message']);
        if($lat>20&&$lat<60&&$lon<-1&&$lon>-12) {
          array_push($t2,array($lat,$lon));
          }
        }
      }
    return(array('radio'=>$t,'sms'=>$t2));
    }

}
