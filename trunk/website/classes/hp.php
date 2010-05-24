<?php
/**
 * @author Celso Martinho
 * @version 1.0
 * @package Homepage
 */
include('classes/smarty/Smarty.class.php');
include('config/site.php');

setlocale(LC_ALL, 'en_US');

class Spacebits_Homepage {

  function __construct() {
    $this->uploads="uploads";
    $this->secret=SECRET;
    $this->db=DB_SITE;
    $this->smarty=new Smarty;
    $this->smarty->cache_lifetime = 60; // in seconds
    $this->smarty->compile_dir = '../cache/templates_c';
    $this->smarty->cache_dir = '../cache/generic';
    $this->smarty->template_dir = array('../www/templates');
    $this->smarty->caching = false;
//      $this->smarty->cache_lifetime = 30;
    $this->smarty->force_compile = true; // good for testing
    $this->uid=$this->auth();
    $this->smarty->assign('uid',$this->uid);
    list($uri)=explode("?",$_SERVER["REQUEST_URI"]);
    list($uri)=explode("&",$uri);
    if($uri[strlen($uri)-1]=='/') $uri=substr($uri,0,-1);
    $this->smarty->assign('uri',$uri);
    if(strstr($_SERVER['HTTP_USER_AGENT'],'iPad')) $this->smarty->assign('ipad',true);
    }

  function display() {
    GLOBAL $sconfig;
    header("Content-Type: text/html; charset=utf-8"); 
    switch($_GET['action']) {
    default:
      $this->smarty->assign('bigpic','bigpic'.rand(1,2).".jpg");
      $this->smarty->display('index.tpl');
      break;
      }
    }

  function multimedia() {
    GLOBAL $sconfig;
    header("Content-Type: text/html; charset=utf-8"); 
    switch($_GET['action']) {
    default:
      $this->smarty->display('multimedia.tpl');
      break;
      }
    }

  function live() {
    GLOBAL $sconfig;
    header("Content-Type: text/html; charset=utf-8"); 
    switch($_GET['action']) {
    default:
      $this->smarty->display('live.tpl');
      break;
      }
    }

  function auth() {
    list($id,$sha1,$ts,$hash)=explode("#",$_COOKIE['login'],4);
    if(md5($id.$sha1.$ts.getenv('REMOTE_ADDR').$this->secret)==$hash) {
      if($ts+60*60*24*3>time()) {
        return($id);
        }
      }
    return("");
    }

  function login() {
    GLOBAL $sconfig;
    header("Content-Type: text/html; charset=utf-8"); 
    header("Expires: " . date("D, j M Y H:i:s", time() - (5 * 60)) . " UTC");
    header("Cache-Control: no-cache");
    header("Pragma: no-cache");
    if($_POST['login']&&$_POST['password']) {
      if($db = new PDO($this->db)) {
        $sql="SELECT user,password FROM users WHERE user=".$db->quote($_POST['login'])." AND password=".$db->quote(sha1($_POST['password']));
        $q = $db->prepare($sql);
        $q->execute();
        if($q->fetch(PDO::FETCH_ASSOC)) {
           $ts=time();
           $cookie=$_POST['login']."#".sha1($_POST['password'])."#".$ts."#".md5($_POST['login'].sha1($_POST['password']).$ts.getenv('REMOTE_ADDR').$this->secret);
           $ct=time()+60*60*24*60;
           setcookie("login",$cookie,$ct,'/',getenv('HTTP_HOST'));
           header("Location: /");
           exit;
           }
        }
      }
    $this->smarty->display('login.tpl');
    }

  function logout() {
    GLOBAL $sconfig;
    setcookie("login","",time() - 3600,'/',getenv('HTTP_HOST'));
    header("Location: /");
    exit;
    }

  function page($id,$action) {
    GLOBAL $sconfig;
    header("Content-Type: text/html; charset=utf-8"); 
    header("Expires: " . date("D, j M Y H:i:s", time() - (5 * 60)) . " UTC");
    header("Cache-Control: no-cache");
    header("Pragma: no-cache");
    $this->smarty->assign('page',$id);
    $this->smarty->assign('action',$action);
    $this->smarty->assign('rand',rand(0,10000));
    $this->smarty->assign('editable',true);
    switch($action) {
      case "browser":
        if($this->uid=="") return;
        $files=array();
        if (is_dir($this->uploads)) {
          if ($dh = opendir($this->uploads)) {
            while (($file = readdir($dh)) !== false) {
              if(preg_match("/\.jpg$/i",$file)) {
                array_push($files,$file);
                }
              }
            closedir($dh);
            }
          }
        $this->smarty->assign('count',sizeof($files));
        $this->smarty->assign('offset',$id);
        $this->smarty->assign('files',$files);
        $this->smarty->assign('func',$_GET['CKEditorFuncNum']);
        $this->smarty->display('browser.tpl');
        break;
      case "save":
        if($this->uid=="") return;
        if($db = new PDO($this->db)) {
          $sql="REPLACE INTO articles VALUES(".$db->quote($id).",".$db->quote($_POST['articlecontent']).",".$db->quote($_POST['articletitle']).",".time().",'".$this->uid."',".$db->quote($_POST['articletags']).");";
          $q = $db->exec($sql);
          header("Location: /page/".$id); 
          }
        break;
      case "delete":
        if($this->uid=="") return;
        if($db = new PDO($this->db)) {
          $sql="DELETE FROM articles WHERE id=".$db->quote($id);
          $q = $db->exec($sql);
          header("Location: /"); 
          }
        break;
      default:
        if($db = new PDO($this->db)) {
          $sql="SELECT body,title,tags FROM articles WHERE id=".$db->quote($id);
          $q = $db->prepare($sql);
          $q->execute();
          if($r=$q->fetch(PDO::FETCH_ASSOC)) {
            $this->smarty->assign('title',stripslashes($r['title']));
            $this->smarty->assign('tags',stripslashes($r['tags']));
            $this->smarty->assign('body',stripslashes($r['body']));
            }
            else
            {
            $this->smarty->assign('title','Not found');
            $this->smarty->assign('body','This page doesn\'t exist');
            }
          $this->smarty->display('page.tpl');
          }
        break;
      }
    }

}
