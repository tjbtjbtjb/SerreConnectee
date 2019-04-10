#!/bin/bash 

GO="/home/greenhouse/Git/SerreConnectee/Server/sendCommand.sh arduino0"

$GO set water 1
sleep 5
$GO set water 0
sleep 2

$GO get water 
if [[ `$GO get water | cut -b1-5 ` != "ACK 0" ]]
then
  echo Pbl, water still opened 1>&2
fi

