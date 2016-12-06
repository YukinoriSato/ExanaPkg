<?php

// [ Obtain the URL options for the php files ]
$filename=$_GET['filename'];
$summary_threshold='-thr';
$summary_threshold_ratio=$_GET['summary_threshold_ratio'];  
$nodeID='-nodeID';
$nodeIDvalue=$_GET['nodeID']; 
$checkedflags='checkedflags';
$flags=$_GET['checkedflags'];
$exefile='-exefile';
$exefilepath=$_GET['exefile']; 

if($exefilepath != "" && $summary_threshold_ratio != "" ){
	exec("./generateDot $filename $flags $summary_threshold $summary_threshold_ratio $nodeID $nodeIDvalue $exefile $exefilepath", $output, $result); echo "p1";
}
elseif($exefilepath != "" && $summary_threshold_ratio == ""){
	exec("./generateDot $filename $flags $nodeID $nodeIDvalue $exefile $exefilepath", $output, $result); echo "p2";
}
elseif($exefilepath == "" && $summary_threshold_ratio != ""){
	exec("./generateDot $filename $flags $summary_threshold $summary_threshold_ratio $nodeID $nodeIDvalue", $output, $result); echo "p3";
}
else{ 
	exec("./generateDot $filename $flags $nodeID $nodeIDvalue", $output, $result); echo "p4";
}

echo "./generateDot $filename $summary_threshold_ratio $flags $nodeIDvalue";
echo "\n";//echo "summary_threshold: $summary_threshold_ratio";
echo implode("<br>", $output);
echo "$result";

if($result >= 2) // now it is ok with result == 1 But why ?????
{ 
   echo "Error Found Here!!!"; 
?>   
   <a href="main.php" alt="Click to accept the change">Click Me to go back!</a>
<?php
  die();
}  

?>

<html>
	<head>
	
	</head>
   <body>
        <?php 
           header( 'Location: main.php' ) ; 
        ?> 
	<!-- This link will add ?run=true to your URL, myfilename.php?run=true -->
	<a href="main.php" alt="Click to accept the change">Click Me!</a>
   </body>
</html>

