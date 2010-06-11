<?php
require_once('classes/smarty/Smarty.class.php');
require_once('classes/spacebits.php');
require_once('config/site.php');

setlocale(LC_ALL, 'en_US');
date_default_timezone_set('Europe/Lisbon');

class Spacebits_Homepage extends Spacebits {

  function __construct() {
    $this->uploads="uploads";
    $this->secret=SECRET;
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
    $launch=strtotime("2010-05-30 11:00:00");
    $this->smarty->assign('now',time());
    $this->smarty->assign('launch',$launch);
    $this->smarty->assign('left',$launch-time());
    if(strstr($_SERVER['HTTP_USER_AGENT'],'iPad')) $this->smarty->assign('ipad',true);
    if(strstr($_SERVER["SERVER_NAME"],'localhost')) $this->smarty->assign('dev',1);
      else $this->smarty->assign('dev',0);
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
           if($_POST['referer']) header("Location: ".$_POST['referer']);
             else header("Location: /");
           exit;
           }
        }
      }
    $this->smarty->assign('referer',getenv('HTTP_REFERER'));
    $this->smarty->display('login.tpl');
    }

  function logout() {
    GLOBAL $sconfig;
    setcookie("login","",time() - 3600,'/',getenv('HTTP_HOST'));
    header("Location: /");
    exit;
    }

  function blog() {
    $this->smarty->assign('posts',$this->getblogposts());
    $this->smarty->display('blog.tpl');
    }

  function page($id,$action) {
    GLOBAL $sconfig;
    header("Content-Type: text/html; charset=utf-8"); 
    header("Expires: " . date("D, j M Y H:i:s", time() - (5 * 60)) . " UTC");
    header("Cache-Control: no-cache");
    header("Pragma: no-cache");
    $this->smarty->assign('page',$id);
    if(strlen($action)==0) $action='page';
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
              if(preg_match("/\.(jpg|gif|png)$/i",$file)) {
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
        $this->smarty->assign('suffix','CKEditor=articlecontent&CKEditorFuncNum='.$_GET['CKEditorFuncNum'].'&langCode=en');
        $this->smarty->display('browser.tpl');
        break;
      case "save":
        if($this->uid==""|strlen($_POST['articlecontent'])==0) return;
        if($db = new PDO($this->db)) {
          $sql="SELECT body,change,title,user FROM articles WHERE id=".$db->quote($id);
          $q = $db->prepare($sql);
          $q->execute();
          if($r=$q->fetch(PDO::FETCH_ASSOC)) { // save old article in history table
            $sql="REPLACE INTO articles_history VALUES(".$db->quote($id).",".$db->quote($r['body']).",".$db->quote($r['title']).",".$r['change'].",'".$r['user']."');";
            $db->exec($sql);
            }
          // write the article into db
          $sql="REPLACE INTO articles VALUES(".$db->quote($id).",".$db->quote($_POST['articlecontent']).",".$db->quote($_POST['articletitle']).",".time().",'".$this->uid."');";
          $q = $db->exec($sql);
          // tags
          $db->exec("DELETE FROM articles_tags WHERE id=".$db->quote($id));
          $tags=str_replace(" ",",",$_POST['articletags']);
          foreach(explode(",",$tags) as $tag) {
            if(strlen($tag)) {
              $db->exec("INSERT INTO articles_tags VALUES(".$db->quote($id).",".$db->quote($tag).");");
              }
            }
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
          $sql="SELECT body,title FROM articles WHERE id=".$db->quote($id);
          $q = $db->prepare($sql);
          $q->execute();
          if($r=$q->fetch(PDO::FETCH_ASSOC)) {
            $this->smarty->assign('title',stripslashes($r['title']));
            $this->smarty->assign('tags',implode(",",$this->getpagetags($id)));
            $this->smarty->assign('body',stripslashes($r['body']));
            }
            else
            {
            $this->smarty->assign('title','Not found');
            $this->smarty->assign('body','This page doesn\'t exist');
            }
          if(strstr($_SERVER["REQUEST_URI"],'fbc_channel')) {
            $this->smarty->assign('fbc_channel',1);
            }
          $this->smarty->display('page.tpl');
          }
        break;
      }
    }

}
