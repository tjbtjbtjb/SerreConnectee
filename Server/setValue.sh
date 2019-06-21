#!/bin/bash 

test $# -ne 3 && echo "Not enough arg for setValue script. Need arduino, sensor name and value" >&2 && exit 1
sendCommand="/home/greenhouse/Git/SerreConnectee/Server/sendCommand.sh"
arduino=$1
sensor=$2
val=$3

ans=`$sendCommand $arduino set $sensor $val`
test "$ans" != "ACK SET OK" && echo "Incorrect response of $arduino for setting sensor $sensor to $val, got *$ans* answer which is a pbl." >&2 && exit 2
exit 0
