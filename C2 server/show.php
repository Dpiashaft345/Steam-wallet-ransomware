

<?php
        
                   
$con = mysqli_connect("localhost","root","","test");
                

$id=mysqli_real_escape_string($con,$_GET['id']);
$query = "SELECT * FROM ids WHERE user_id = '$id' LIMIT 1";

$query_run = mysqli_query($con,$query);



$row = mysqli_fetch_row($query_run);

echo $row[2];


?>
                     
