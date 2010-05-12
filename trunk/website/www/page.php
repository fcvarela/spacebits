<?php
include('classes/hp.php');

$hp=new Spacebits_Homepage;
$hp->page($_GET['id'],$_GET['action']);

?>
