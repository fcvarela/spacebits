<?php

$post=file_get_contents('php://input');

file_put_contents("/tmp/spacepost.xml",print_r($post,true),FILE_APPEND);

echo "<result>ok</result>\n";

?>
