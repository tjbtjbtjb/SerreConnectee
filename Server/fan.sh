#!/bin/bash 

arduino=arduino0
test $# -eq 1 && arduino=$1

GO="/home/greenhouse/Git/SerreConnectee/Server/sendCommand.sh $arduino "

INTHR=`$GO get intairhr | awk '{print $2}'`
FAN=`$GO get fan | awk '{print $2}'`

HRMAX=97.0
HRMIN=81.0

#echo $INTHR
#echo $FAN

if [ `echo $FAN '!=' 1. | bc` -eq 1 ]
then
 if [ `echo $INTHR '>' $HRMAX | bc` -eq 1 ]
 then
  echo "Too much humidity : $INTHR , fanning $arduino, currently $TGND "
  $GO set fan 1
 fi
else
 if [ `echo $INTHR '<' $HRMIN | bc` -eq 1 ]
 then
  echo "Stopping fan $arduino "
  $GO set fan 0
 fi
fi 
