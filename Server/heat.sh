#!/bin/bash 

arduino=arduino0
test $# -eq 1 && arduino=$1

ldir=`dirname $0`
getval="$ldir/getValue.sh $arduino "
setval="$ldir/setValue.sh $arduino "

TGND=`$getval thermo || echo err`
HEAT=`$getval heat || echo err`
TMAX=28.9
TMIN=28.5

if [ "$TGND" == "err" ] || [ "$HEAT" == "err" ]
then 
  echo "Incorrect values." >&2 
  exit 10
fi

if [ `awk 'BEGIN {print ('$HEAT'==0)}'` -eq 1 ] && [ `awk 'BEGIN {print ('$TGND'<'$TMIN')}'` -eq 1 ]
then
  echo "Heating on $arduino, currently $TGND "
  $setval heat 1
elif [ `awk 'BEGIN {print ('$HEAT'==1)}'` -eq 1 ] && [ `awk 'BEGIN {print ('$TGND'>'$TMAX')}'` -eq 1 ]
then
  echo "Heating off $arduino, currently $TGND "
  $setval heat 0
fi

