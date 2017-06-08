<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <title>CGIお試し用</title>
    </head>
    <body>
<?php
parse_str($_SERVER['POST_STR'], $_POST);
$omikuzi = false;
if (isset($_POST['name'])) {
    $name = $_POST['name'];
    $omikuzi = true;
}
if (isset($_POST['age'])) {
    $age = $_POST['age'];
}
$hash = md5($name . $age);
$hash = pow(hexdec($hash)/10,35)%10;

if ($omikuzi) {
    echo $name."さん:".$age."歳!!\n";
    echo "<h1>あなたの運勢は…</h1><br>\n<p><b>";

    switch ($hash) {
      case 0:
      echo "大吉!!おめでとう!!";
      break;

      case 1:
      echo "中吉!!";
      break;

      case 2:
      echo "小吉!";
      break;

      case 3:
      echo "中吉!!";
      break;

      case 4:
      echo "木吉!";
      break;

      case 5:
      echo "うし!w";
      break;

      case 6:
      echo "小吉!!";
      break;

      case 7:
      echo "大凶";
      break;

      case 8:
      echo "末吉";
      break;

      case 9:
      echo "凶";
      break;

      default:
      echo "人生やり直せ";
      break;
    }
    echo "<b></p>";
}
?>
       <h1>おみくじ送信コーナーわーーーーーー</h1>
        <form action="omikuzi.php" method="post">
            <fieldset>
                <legend>入力フォーム</legend>
                <p>あなたのお名前 : <input type="text" name="name" value="" required></p>
                <p>年齢 : <input type="number" name="age" value="" nim=0 required></p>
                <p><input type="submit" name="" value="おみくじを引く"></p>
            </fieldset>
        </form>
    </body>
</html>
