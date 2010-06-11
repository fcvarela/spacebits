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
  case "blog":
    $hp->blog();
    break;
  case "bo":
    $hp->backoffice();
    break;
  case "multimedia":
    $hp->multimedia();
    break;
  case "live":
    $hp->live();
    break;
  default:
    $hp->display();
    break;
  }
?>
