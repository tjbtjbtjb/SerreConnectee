#!/bin/bash 

arduino=arduino0
test $# -eq 1 && arduino=$1

GO="/home/greenhouse/Git/SerreConnectee/Server/sendCommand.sh $arduino "

CO2=`$GO get co2ppm | awk '{print $2}'`
FAN=`$GO get fan | awk '{print $2}'`

CO2MAX=500
CO2MIN=400

#echo $INTHR
#echo $FAN

if [ `echo $FAN '!=' 1. | bc` -eq 1 ]
then
 if [ `echo $CO2 '>' $CO2MAX | bc` -eq 1 ]
 then
  echo "Too much co2 : $CO2 , fanning $arduino "
  $GO set fan 1
 fi
else
 if [ `echo $CO2 '<' $CO2MIN | bc` -eq 1 ]
 then
  echo "Stopping fan $arduino "
  $GO set fan 0
 fi
fi 
