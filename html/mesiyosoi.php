<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>作ってみたメシヨソイゲーム</title>
    </head>
    <body>
<?php

   parse_str($_SERVER['POST_STR'], $_POST);
   $mesiyosoi = false;
   if (isset($_POST['name'])) {
   $name = $_POST['name'];
   $mesiyosoi = true;
   }

   if ($mesiyosoi) 
   {
   $hash = mt_rand()%5;
   echo "<h1>診断結果</h1><br>\n<p><b>";

   switch ($hash) {
   case 0:
   echo "おめでとう!".$name."はメシをよそえました！おめでとう！よっそいよっそい！よっそいよっそい！";
   break;

   case 1:
   echo $name."はメシをこぼしました";
   break;

   case 2:
   echo $name."はメシをよそえませんでした";
   break;

   case 3:
   echo $name."はそもそもメシぎらいでした";
   break;

   case 4:
   echo $name."はパンを食べました";
   break;
   
   default:
   echo $name."はメシをよそえませんでした";
   break;
   }
   echo "<b></p>";
   }
   ?>
<h1>診断メーカー 作ってみたメシヨソイゲーム</h1>
<h2>君は飯をよそえるか</h2>
<form action="mesiyosoi.php" method="post">
  <fieldset>
    <legend>診断したい名前を入れてください</legend>
    <p>あなたのお名前 : <input type="text" name="name" value="" required></p>
    <p><input type="submit" name="" value="診断する"></p>
  </fieldset>
</form>
    </body>
</html>
