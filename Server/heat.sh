#!/bin/bash 

arduino=arduino0
test $# -eq 1 && arduino=$1

GO="/home/greenhouse/Git/SerreConnectee/Server/sendCommand.sh $arduino "

TGND=`$GO get thermo | awk '{print $2}'`

TMAX=29.5
TMIN=26.5

if [ `echo $TGND '<' $TMIN | bc` -eq 1 ]
then
  echo "Heating on and fan off $arduino, currently $TGND "
  $GO set heat 1
  $GO set fan 0
elif [ `echo $TGND '>' $TMAX | bc` -eq 1 ]
then
  echo "Heating off and fan on $arduino, currently $TGND "
  $GO set heat 0
  $GO set fan 1
fi

