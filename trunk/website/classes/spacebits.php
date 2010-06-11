<?php
require_once('classes/smarty/Smarty.class.php');
require_once('config/site.php');

setlocale(LC_ALL, 'en_US');
date_default_timezone_set('Europe/Lisbon');

class Spacebits {
   var $db=DB_SITE;

  function getblogposts() {
   $posts=array();
   if($db = new PDO($this->db)) {
     $sql="select a.id,a.title,a.body,a.user,a.change from articles a,articles_tags at where at.id=a.id and at.tag='blog' order by a.change DESC limit 10";
     $q = $db->prepare($sql);
     $q->execute();
     while($r=$q->fetch(PDO::FETCH_ASSOC)) {
       preg_match("/<p>(.*)<\/p>/is",$r['body'],$matches);
       $teaser=strip_tags($matches[1]);
       $matches=explode(". ",$teaser);
       array_push($posts,array('title'=>$r['title'],'id'=>$r['id'],'body'=>$r['body'],'teaser'=>$matches[0].". ".$matches[1].".",'user'=>$r['user'],'change'=>$r['change'],'date'=>date('l jS \of F Y',$r['change']),'tags'=>$this->getpagetags($r['id'])));
       }
     }
   return($posts);
   }

 function getpagetags($id) {
   $tags=array();
   if($db = new PDO($this->db)) {
     $sql="SELECT tag FROM articles_tags WHERE id=".$db->quote($id);
     $q = $db->prepare($sql);
     $q->execute();
     while($r=$q->fetch(PDO::FETCH_ASSOC)) {
       array_push($tags,$r['tag']);
       }
     }
   return($tags);
   }

}
