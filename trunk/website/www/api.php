<?php

$post=file_get_contents('php://input');

file_put_contents("/tmp/spacepost.xml",print_r($post,true));

echo "<result>ok</result>\n";

?>
