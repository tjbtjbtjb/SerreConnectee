#!/bin/bash 

test $# -ne 2 && echo "Not enough arg for getValue script. Need arduino and sensor name" >&2 && exit 1
sendCommand="/home/pi/Git/SerreConnectee/Server/sendCommand.sh"
arduino=$1
sensor=$2

ans=`$sendCommand $arduino get $sensor || echo err`

ack=`echo $ans | awk '{print $1}'`
test "$ack" != "ACK" && echo "Incorrect response of $arduino for sensor $sensor, got *$ans* which cannot be interpreted." >&2 && exit 2

val=`echo $ans | awk '{print $2}'`
test `awk -v a="$val" 'BEGIN {print (a==a+0);}'` -ne 1 && echo "Value from $arduino for sensor $sensor is *$val* which is not a number." >&2 && exit 3

echo $val
