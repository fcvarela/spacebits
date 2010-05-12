<?php
function smarty_modifier_substr($string, $start=0,$length = 80)
{
    if ($length == 0)
        return '';
    return substr($string, $start, $length);
}

/* vim: set expandtab: */

?>
