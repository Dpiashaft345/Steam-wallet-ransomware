
<?php
class send{
 public $link='';
 function __construct($id,$info){
  $this->connect();
  $this->storeInDB($id,$info);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'test') or die('Cannot select the DB');
 }
 
 function storeInDB($id,$info){
  $query = "insert into ids set user_id='".$id."',info='".$info."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['uid'] != '' and $_GET['info'] != ''){
 $send=new send($_GET['uid'],$_GET['info']);
}


?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
   
</head>
<body>
    

<h2>dfsgdsffgh</h2>



  
</body>
</html>



