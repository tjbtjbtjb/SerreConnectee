#!/bin/bash 

arduino=arduino0
test $# -eq 1 && arduino=$1

ldir=`dirname $0`
getval="$ldir/getValue.sh $arduino "
setval="$ldir/setValue.sh $arduino "

CO2=`$getval co2ppm || echo err`
FAN=`$getval fan || echo err`

CO2MAX=500
CO2MIN=400

if [ "$CO2" == "err" ] || [ "$FAN" == "err" ]
then
  echo "Incorrect values." >&2
  exit 10
fi

if [ `awk 'BEGIN {print ('$FAN'!=1.)}'` -eq 1 ] && [ `awk 'BEGIN {print ('$CO2'>'$CO2MAX')}'` -eq 1 ]
then
  echo "Too much co2 : $CO2 , fanning $arduino "
  $setval fan 1
elif [ `awk 'BEGIN {print ('$FAN'!=0.)}'` -eq 1 ] && [ `awk 'BEGIN {print ('$CO2'<'$CO2MIN')}'` -eq 1 ]
then
  echo "Stopping fan $arduino "
  $setval fan 0
fi 
