#!/bin/bash

PROGNAME="sensors"

ASKARDUINO=/home/beau/Git/SerreConnectee/Server/talkArduino.py

HOSTNAME="arduino0"
#.${COLLECTD_HOSTNAME:-`hostname -f`}"
INTERVAL="${COLLECTD_INTERVAL:-60}"

while sleep "$INTERVAL"
do
  TEMP=`$ASKARDUINO temp | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    TEMP="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/air_temperature interval=$INTERVAL N:$TEMP"

  HUMI=`$ASKARDUINO humidity | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    HUMI="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/air_humidity interval=$INTERVAL N:$HUMI"

  LIGHT=`$ASKARDUINO light | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    LIGHT="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/ambiant_light interval=$INTERVAL N:$LIGHT"
  
done
