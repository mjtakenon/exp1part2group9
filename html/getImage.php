<?php
    for ($i = 0; $i < 1; $i++) {
        $str = '<img src="'.sprintf('%03d', rand(0,99)).'.jpg" width="100" style="float:left;">';
        echo $str;
    }
?>