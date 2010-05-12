<?php
include('classes/hp.php');

$hp=new Spacebits_Homepage;
switch($_GET['action']) {
  case "login":
    $hp->login();
    break;
  case "logout":
    $hp->logout();
    break;
  default:
    $hp->display();
    break;
  }
?>
