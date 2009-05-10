<?php
////////////////Multiprocessor Brute Force Simulator////////////////
//$data: key = processor # value = array of processor data.
//$p = pointer of each array in array $data.  The array size.
////////////////////////////////////////////////////////////////////
$data = array(array(0),array(0),array(0),array(0),array(0),array(0));
$p = array(0,0,0,0,0,0);
$password = array(21,24,24,28,14,27);
echo base_convert(implode($password),10,36)."\n";
echo base_convert("loo",36,10)."\n";

while ($password != $data[0] and $password != $data[1] and $password != $data[2] and $password != $data[3] and $password != $data[4] and $password != $data[5])
{

//print_r($data);

//$i is processor number 0 - 5
for ($i = 0; $i < 6; $i ++)
{
$data[$i][$p[$i]] = $data[$i][$p[$i]] + 1;

if ($data[$i][$p[$i]] > 36)
{
for ($c = $p[$i]; $c >= 0; $c --){
if ($data[$i][$c] > 36 and $c == 0){
$data[$i][$p[$i]+1] = 0;
//use 0 for 1 processor mode, use $i for 6
$data[$i][$c] = $i;
$p[$i]++;
}
elseif ($data[$i][$c] > 36 and $c > 1){
//use + 1 for all processor modes
$data[$i][$c-1] = $data[$i][$c-1] + 1;
$data[$i][$c] = 0;
}
elseif($data[$i][$c] > 36 and $c == 1){
//$c would be == 1
//increment 6 for 6 processor mode, 1 for single processor.	
$data[$i][$c-1] = $data[$i][$c-1] + 6;
$data[$i][$c] = 0;

}
}
}

}
}

print_r($data);
print_r($p);
?>
