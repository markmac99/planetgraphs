<meta http-equiv="refresh" content="5;JupiterGraphs.html" />

<?php
$a1 = $_GET['Lat'];
$a2 = $_GET['Long'];
$a3 = $_GET['date5'];

$mystr = "./runscript.sh ". $a1. " ". $a2." ".$a3;
shell_exec($mystr);

echo("Executing script to generate your data<br>");
echo($mystr);
echo("<br>redirecting you shortly<br>");
?>
