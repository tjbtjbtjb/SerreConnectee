#!/bin/bash 

GO="/home/beau/Git/SerreConnectee/Server/sendCommand.sh arduino0"

$GO set water 1
sleep 4
$GO set water 0
sleep 1

$GO get water 
if [[ `$GO get water | cut -b1-5 ` != "ACK 0" ]]
then
  echo Pbl, water still opened 1>&2
fi

