<?php
/**
 * @author Celso Martinho
 * @version 1.0
 * @package API
 */
include('config/site.php');

setlocale(LC_ALL, 'en_US');
date_default_timezone_set('Europe/Lisbon');

class Spacebits_API {

  function __construct() {
    $this->api_secret=API_SECRET;
    $this->db=DB_DATA;
    $this->sms_db=DB_SMS;
    $this->twitter_db=DB_TWITTER;
    list($uri)=explode("?",$_SERVER["REQUEST_URI"]);
    list($uri)=explode("&",$uri);
    date_default_timezone_set('Europe/Lisbon');
    }

  function parsePayload($xml) {
    if($arr=@simplexml_load_string($xml)) {
      if($this->api_secret!=$arr->token) return(false);
      $p=array();
      $p['power_current']=floatval($arr->power->current);
      $p['power_voltage']=floatval($arr->power->voltage);
      $p['pressure']=floatval($arr->atmosphere->pressure);
      $p['temperature']=floatval($arr->atmosphere->temp);
      $p['temperature_ext']=floatval($arr->atmosphere->temp_ext);
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

  function putTwit($values=array()) {
    GLOBAL $sconfig;
    if($db = new PDO($this->twitter_db)) {
      $sql="INSERT INTO twits (change,description,link) VALUES(".intval($values['change']).",".$db->quote($values['description']).",".$db->quote($values['link']).")";
      $q = $db->prepare($sql);
      $q->execute();
      }
    }

  function getTwits() {
    $t=array();
    if($db = new PDO($this->twitter_db)) {
      $sql="SELECT change,description,link FROM twits ORDER BY change DESC LIMIT 20";
      $q = $db->prepare($sql);
      $q->execute();
      while($r=$q->fetch(PDO::FETCH_ASSOC)) {
        list(,$d)=explode(": ",$r['description']);
        foreach($this->yfrog($d) as $l) {
          $d=str_replace($l,'',$d);
          $d.='<br/><a href="'.$l.'" target="_blank"><img src="'.$l.'.th.jpg"></a>';
          }
        $d=$this->clickable($d);
        array_push($t,array('change'=>$r['change'],'description'=>$d,'link'=>$r['link']));
        }
     }
    return($t);
    }

  function clickable($text) { 
    $text = preg_replace('#(script|about|applet|activex|chrome):#is', "\\1:", $text); 
    $ret = ' ' . $text; 
    $ret = preg_replace("#(^|[\n ])([\w]+?://[\w\#$%&~/.\-;:=,?@\[\]+]*)#is", "\\1<a href=\"\\2\" target=\"_blank\">\\2</a>", $ret); 
    $ret = preg_replace("#(^|[\n ])((www|ftp)\.[\w\#$%&~/.\-;:=,?@\[\]+]*)#is", "\\1<a href=\"http://\\2\" target=\"_blank\">\\2</a>", $ret); 
    $ret = preg_replace("#(^|[\n ])([a-z0-9&\-_.]+?)@([\w\-]+\.([\w\-\.]+\.)*[\w]+)#i", "\\1<a href=\"mailto:\\2@\\3\">\\2@\\3</a>", $ret); 
    $ret = substr($ret, 1); 
    return $ret; 
    } 

  function yfrog($text) {
    preg_match_all("/http:\/\/yfrog\.com\/[0-9a-zA-Z]+/",$text,$out);
    return($out[0]);
    }

  function put($values=array()) {
    GLOBAL $sconfig;
    if($db = new PDO($this->db)) {
      $sql="INSERT INTO data (change,power_current,power_voltage,pressure,temperature,temperature_ext,humidity,dust_density,lat,lon,alt,bear,imu_gx,imu_gy,imu_ax,imu_ay,imu_az) VALUES(".$values['change'].",".$values['power_current'].",".$values['power_voltage'].",".$values['pressure'].",".$values['temperature'].",".$values['temperature_ext'].",".$values['humidity'].",".$values['dust_density'].",".$values['lat'].",".$values['lon'].",".$values['alt'].",".$values['bear'].",".$values['imu_gx'].",".$values['imu_gy'].",".$values['imu_ax'].",".$values['imu_ay'].",".$values['imu_az'].")";
      $q = $db->prepare($sql);
      $q->execute();
      }
    }

  function saveSMS($source,$message) {
    if($db = new PDO($this->sms_db)) {
      $sql="INSERT INTO sms (change,source,message,broadcast) VALUES(".time().",".$db->quote($source).",".$db->quote($message).",0)";
      $q = $db->prepare($sql);
      $q->execute();
      }
    }

  function lastSMS() {
    $p=array();
    if($db = new PDO($this->sms_db)) {
      $sql="select change,message from sms WHERE message!='0.0000,0.0000,0,0' order by change desc limit 1";
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
      $p['temperature_ext']=rand(-50,10);
      $p['alt']=rand(0,40000);
      $p['humidity']=rand(0,100);
      $p['bear']=rand(0,360);
      $p['dust_density']=rand(0,100);
      $p['pressure']=rand(10,1000);
      $p['lon']=-8.0919+(float)(rand(-500,500)/1000);
      $p['lat']=37.7616+(float)(rand(-500,500)/1000);
    }
    else
    {
      if($db = new PDO($this->db)) {
        $sql="SELECT change,power_current,power_voltage,pressure,temperature,temperature_ext,humidity,dust_density,lat,lon,alt,bear,imu_gx,imu_gy,imu_ax,imu_ay,imu_az FROM data WHERE lat!=0 AND lon!=0 ORDER BY change DESC LIMIT 1";
        $q = $db->prepare($sql);
        $q->execute();
        if($r=$q->fetch(PDO::FETCH_ASSOC)) {
          foreach(array_keys($r) as $key) {
            $p[$key]=$r[$key];
            }
          $sms=$this->lastSMS();
          if(intval($sms['change'])>intval($p['change'])) {
            $p['lat']=$sms['lat'];
            $p['lon']=$sms['lon'];
            $p['alt']=$sms['alt'];
            }
          }
       }
    }
    $p['elapsed']=date("H:i:s",time()-strtotime("2010-05-30 03:00:00 GMT")).' '.date("H:i:s").' '.date("H:i:s",$p['change']);
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


  function makerss($posts) {
    $out="<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<rss version=\"2.0\" xmlns:atom=\"http://www.w3.org/2005/Atom\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:openSearch=\"http://a9.com/-/spec/opensearchrss/1.0/\" xmlns:sapo=\"http://www.sapo.pt/RSS/Modules/Homepage\">\n<channel><link>http://spacebits.eu/</link>\n<language>pt-pt</language>\n<copyright>2007, PT.COM</copyright>\n<description></description>\n<title>Spacebits Blog</title>\n";
    foreach($posts as $item) {
      $guid=md5($item['id']);
      $out.="<item><title><![CDATA[".$item['title']."]]></title><link><![CDATA[http://spacebits.eu/page/".stripslashes($item['id'])."]]></link>\n";
      $out.="<description><![CDATA[<p>".stripslashes($item['body'])."</p>]]></description>\n";
      $out.="<pubDate>".strftime("%a, %d %b %Y %H:%M:%S %z",$item['change'])."</pubDate>\n";
      $out.="<guid isPermaLink=\"false\">".$guid."</guid><author>spacebits@sapo.pt</author><source url=\"http://spacebits.eu/\">Spacebits</source></item>\n\n";
      }
    $out.='<atom:link href="http://spacebits.eu/" rel="self" type="application/rss+xml" />'."\n";
    $out.="</channel></rss>";
    return($out);
    }

}
