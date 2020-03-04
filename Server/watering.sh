#!/bin/bash 

arduino=arduino0
test $# -eq 1 && arduino=$1

GO="/home/greenhouse/Git/SerreConnectee/Server/sendCommand.sh $arduino "

$GO set water 1
sleep 30
$GO set water 0
sleep 2

$GO get water 
if [[ `$GO get water | cut -b1-5 ` != "ACK 0" ]]
then
  echo "Pbl with $arduino , water still opened" 1>&2
fi

