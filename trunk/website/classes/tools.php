<?php

/**
 * @author Celso Martinho
 * @version 1.0
 * @package Tools
 */

class SAPO_Tools {
  var $http_timeout=10;
  var $last_cookies=NULL;
  var $tet_months=array('Janeiru','Fevereiru','Marsu','Abr&iacute;l','Maiu','Ju&ntilde;o','Jullo','Agostu','Setembru','Outubru','Novembru','Dezembru');

  function __construct() {
    }

  function ReadWebArgs() {
    if(getenv('REQUEST_METHOD') == 'GET') {
      // suport for lighty mod_rewrite weirdness, read _GET args from REQUEST_URI
      list(,$args)=explode('?',getenv('REQUEST_URI'));
      $gets=$_GET; // use original
      foreach(explode('&',$args) as $arg) { // then override
        list($k,$v)=explode('=',$arg);
        $gets[$k]=html_entity_decode(urldecode($v));
        }
      $queryVars = $gets;
      } else if (getenv('REQUEST_METHOD') == 'POST') {
      $queryVars = array_merge($this->ClearArray($_GET),$this->ClearArray($_POST));
      } else {
      exit;
      }
    return($queryVars);
    }

  function FromRegularPage() {
    $referer=parse_url(getenv('HTTP_REFERER'));
    if(getenv('HTTP_REFERER') &&
      !preg_match('/^\/edit\/.*$/i',$referer['path']) &&
      !preg_match('/^\/post\/.*$/i',$referer['path']) &&
      !preg_match('/^\/login.*$/i',$referer['path']) &&
      !preg_match('/^\/register.*$/i',$referer['path'])
      ) return(getenv('HTTP_REFERER'));
    return(FALSE);
    }

  function UnifyURL($url) {
    if(!strstr($url,'://')) return($url);
    $arr=parse_url($url);
    $r=$arr['scheme'].'://';
    if($arr['user']) $r.=$arr['user'].':'.$arr['pass'].'@';
    $r.=$arr['host'];
    if($arr['path']) {
      switch($arr['path']) {
        case '/index.html';
	  $r.='/';
	  break;
        case '/index.php';
	  $r.='/';
	  break;
        default:
          $r.=$arr['path'];
	  break;
        }
      }
      else
      {
      $r.='/';
      }
    if($arr['query']) $r.='?'.$arr['query'];
    if($arr['fragment']) $r.='#'.$arr['fragment'];
    return($r);
    }

  function ClearArray($arr) {
    $newarr=array();
    foreach(array_keys($arr) as $key) {
      if(strlen($arr[$key])) $newarr[$key]=$arr[$key];
      }
    return($newarr);
    }

  function jsonentities($string) {
    $out=str_replace("'","\\'",$string);
    $out=str_replace("\"","\\\"",$string);
    $out=str_replace("\r",'\r',$out);
    $out=str_replace("\n",'\n',$out);
    return($out);
    }

  function xmlentities($string, $quote_style=ENT_QUOTES,$charset='UTF-8') {
    static $trans;
    if (!isset($trans)) {
      $trans = get_html_translation_table(HTML_SPECIALCHARS, $quote_style);
      foreach ($trans as $key => $value)
        $trans[$key] = '&#'.ord($key).';';
       // dont translate the '&' in case it is part of &xxx;
        $trans[chr(38)] = '&';
        }
     // after the initial translation, _do_ map standalone '&' into '&#38;'
     return preg_replace("/&(?![A-Za-z]{0,4}\w{2,3};|#[0-9]{2,3};)/","&#38;" , strtr(html_entity_decode($string), $trans));
    }

  function TitleToNameDir($title = false) {
    $str = array();
    $str['a'] = "/Á|À|Ã|Â|á|à|ã|â/";
    $str['e'] = "/É|È|Ê|é|è|ê/";
    $str['i'] = "/Í|Ì|Î|í|ì|î|í|ì|î/";
    $str['o'] = "/Ó|Ò|Õ|Ô|ó|ò|õ|ô/";
    $str['u'] = "/Ú|Ù|Û|ú|ù|û/";
    $str['c'] = "/Ç|ç/";

    if($title) {
      $title = strtolower($this->Strip($title));
      $title = str_replace(" ", "", $title);
      $title = str_replace('/', "", $title);
      $title = str_replace('(', "", $title);
      $title = str_replace(')', "", $title);
      $title = preg_replace("/\/\.|!|$|^|~|'|`|\+|\*|\?|\"|#|%|&|\|=|{|}|\[|\]|«|»|,|;|:|º|ª/", "", $title);
      foreach($str as $key => $value) {
        $title = preg_replace($value, $key, $title);
        }
      }
    return $title;
    }

  function Plural($n,$s,$p) {
    if($n>1) return($p);
    return($s);
    }

  function htmlEntitiesDecode($txt) {
    return(preg_replace('~&#([0-9]+);~e', 'chr("\\1")', html_entity_decode($txt,null,'UTF-8')));
    }

  function Strip($in = false,$utf8=TRUE) {

    $subs=array("á",'a', "à",'a', "ã",'a', "â",'a', "Á",'A', "À",'A', "Â",'A', "Ã",'A', "é",'e', "è",'e', "ê",'e', "É",'e', "È",'e', "Ê",'e', "í",'i', "ì",'i', "Í",'i', "Ì",'i', "Î",'i', "ó",'o', "ò",'o', "ô",'o', "õ",'o', "Ó",'o', "Ò",'o', "Ô",'o', "Õ",'o', "ú",'u', "ù",'u', "û",'u', "Ú",'u', "Ù",'u', "Û",'u', "ñ",'n', "Ñ",'n', "ç",'c', "Ç",'c');

    if(!$in) return(FALSE);
    if($utf8) $r=utf8_decode($in); else $r=$in;
    for($i=0;$i<count($subs);$i+=2) {
      $r=str_replace($subs[$i],$subs[$i+1],$r);
      }
    preg_match_all("/[a-z]?[A-Z]?[0-9]?[- +\._]?/",$r,$out);
    $r=implode('',$out[0]);
    if($utf8) $r=utf8_encode($r);
    return($r);
    }

  function MergeTags($arr1,$arr2) {
    $out=array();
    foreach ($arr1 as $word1) {
      $h=0;
      foreach($arr2 as $word2) {
        if($this->Strip($word2)==$this->Strip($word1)) { $h=1; break; }
        }
      if($h) array_push($out,$word2);
        else array_push($out,$word1);
      }
    return($out);
    }

  function FileGetContents($url,$proxy=true,$user=null,$pass=null,$cookies=null) {
    #echo "Getting $url\n";
    $ch = curl_init();
    curl_setopt ($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Macintosh; U; Intel Mac OS X; en-US; rv:1.8.1.14) Gecko/20080404 Firefox/2.0.0.14');
    curl_setopt ($ch, CURLOPT_URL, $url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt ($ch, CURLOPT_FOLLOWLOCATION,1);
    curl_setopt ($ch, CURLOPT_HEADER,0);
    #curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "Expect:"); 
    curl_setopt($ch,  CURLOPT_COOKIEJAR, "/tmp/".sha1($url).".cookie");
    if($proxy==true) {
      curl_setopt ($ch, CURLOPT_PROXY, 'proxy.srv.bk.sapo.pt:3128');
      }
      else
      {
      curl_setopt ($ch, CURLOPT_PROXY, NULL);
      }
    curl_setopt ($ch, CURLOPT_CONNECTTIMEOUT, $this->http_timeout);
    curl_setopt ($ch, CURLOPT_TIMEOUT, $this->http_timeout);
    if($cookies) {
      curl_setopt($ch, CURLOPT_COOKIE, $cookies);
    }
    curl_setopt($ch,CURLOPT_VERBOSE,false);

    if($user!=null) {
      curl_setopt($ch,CURLOPT_USERPWD,$user.":".$pass);
      }
    $r=curl_exec($ch);
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);
    if($fp=@fopen("/tmp/".sha1($url).".cookie","r")) {
      while($line=fgets($fp,4096)) {
        if(strlen($line)>2&&$line[0]!='#') {
          list(,,,,,$key,$value)=split(" ",preg_replace('/\s+/', ' ',$line));
          $this->last_cookies[$key]=$value;
          }
        }
      fclose($fp);
      unlink("/tmp/".sha1($url).".cookie");
    }
    if($http_code==200||$http_code==302) {
      return($r);
      }
    return(false);
    }

  function post($url,$args,$user=null,$pass=null,$cookies=null,$header=null) {
    echo "Posting to $url\n";
    $ch = curl_init();
    curl_setopt ($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Macintosh; U; Intel Mac OS X; en-US; rv:1.8.1.14) Gecko/20080404 Firefox/2.0.0.14');
    curl_setopt ($ch, CURLOPT_URL, $url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt ($ch, CURLOPT_FOLLOWLOCATION,1);
    curl_setopt ($ch, CURLOPT_HEADER,0);
    curl_setopt($ch, CURLOPT_FAILONERROR, false);

    curl_setopt ($ch, CURLOPT_CONNECTTIMEOUT, $this->http_timeout);
    curl_setopt ($ch, CURLOPT_TIMEOUT, $this->http_timeout);
    curl_setopt($ch, CURLOPT_POST, 1);
    #curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "Expect:"); 
    curl_setopt($ch,  CURLOPT_COOKIEJAR, "/tmp/".sha1($url).".cookie");
    curl_setopt($ch, CURLOPT_POSTFIELDS, $args); 
    if($header) {
      curl_setopt($ch, CURLOPT_HEADER, 1);
      curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
    }
    if($cookies) {
      curl_setopt($ch, CURLOPT_COOKIE, $cookies);
    }
    if($user!=null) {
      curl_setopt($ch,CURLOPT_USERPWD,$user.":".$pass);
      }
    curl_setopt($ch,CURLOPT_VERBOSE,false);
    $r=curl_exec($ch);
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);
    if($fp=fopen("/tmp/".sha1($url).".cookie","r")) {
      while($line=fgets($fp,4096)) {
        if(strlen($line)>2&&$line[0]!='#') {
          list(,,,,,$key,$value)=split(" ",preg_replace('/\s+/', ' ',$line));
          $this->last_cookies[$key]=$value;
          }
        }
      fclose($fp);
      unlink("/tmp/".sha1($url).".cookie");
    }
    if($http_code==200||$http_code==302) {
      return($r);
      }
    return(false);
    }

  function makeThumbnail($url,$width=100,$height=100,$force=0) {
//    $key='O que tu queres sou EU!';
//    return('http://thumbs.sapo.pt/?pic='.urlencode($url).'&H='.$height.'&W='.$width.'&hash='.md5($url.$key).'&force='.$force.'&e=http://images.sapo.pt/images/0.gif');
    return('/imgproxy/?img='.urlencode($url).'&H='.$height.'&W='.$width);
    }


  function ucsmart($msg) {
     $msg=utf8_encode(str_replace("&quot;","\"",strtolower(html_entity_decode(utf8_decode($msg),ENT_QUOTES))));
     $words=array();
     $n=0;
     foreach(explode(" ",trim($msg," \"\'\t\n\r\x0B\0")) as $w) {
       if($n&&preg_match('/^(um|ao|para|num|numa|uma|de|com|do|da|e|a|o)$/i',$w)) array_push($words,$w);
         else array_push($words,ucfirst($w));
       $n++;
       }
     return(implode(" ",$words));
     }


  function isUTF8($str) {
    return preg_match('%^(?:
         [\x09\x0A\x0D\x20-\x7E]           # ASCII
       | [\xC2-\xDF][\x80-\xBF]            # non-overlong 2-byte
       | \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
       | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2} # straight 3-byte
       | \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
       | \xF0[\x90-\xBF][\x80-\xBF]{2}     # planes 1-3
       | [\xF1-\xF3][\x80-\xBF]{3}         # planes 4-15
       | \xF4[\x80-\x8F][\x80-\xBF]{2}     # plane 16
       )*$%xs', $str);
    }

  function scrapUrl($url) {
    $block='';
    if($r=$this->FileGetContents('http://services.bk.sapo.pt/Scrapper/GetStory?url='.urlencode($url))) {
      $obj=simplexml_load_string($r);
      if($obj->Contents) {
        foreach($obj->Contents->Content as $b) {
          $b=preg_replace('/\s+/', ' ',trim($b));
          $block.=$b."\n\n";
          }
        }
      }
    return(mb_convert_encoding($block,'UTF-8','UTF-8'));
    }

  function trimUrl($url) {
    $url=str_replace('http://www.economico.pt','http://economico.sapo.pt',$url);
    $url=preg_replace('/^(\++)?(.*)\++$/','\2',trim($url));
    return($url);
    }

  function writeLangCookie() {

        if(!empty($_GET['lang'])) {
            switch($_GET['lang']) {
                case 'pt_PT':
                case 'tet_TL':

                   preg_match ('/(\.sapo\.[a-z]{2})$/i', $_SERVER['HTTP_HOST'], $matches); 
                   if(count($matches)>1) {
                        $domain = $matches[1];
                   } else {
                        $domain = $_SERVER['HTTP_HOST'];
                   }
                   setcookie ("lang", $_GET['lang'], time()+31*24*3600, '/', $domain);
                   $to = '/';
                   if (!empty($_SERVER['HTTP_REFERER']) ) {
                       $referer = $_SERVER['HTTP_REFERER'];
                       $referer = str_replace( 'http://'.$_SERVER['HTTP_HOST'], '', $referer); 
                       if ( !preg_match ( '/^[0-9a-z\/]+$/', $referer) ) {
                            $to = '/';
                       } else {
                            $to = $referer;
                       }
                   }
                   header('Location: '.$to, true, 302);
                   exit();
                   break;
            }
        }

  }


  }
