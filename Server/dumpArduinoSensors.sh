#!/bin/bash

PROGNAME="arduino-sensors"

ASKARDUINO=/home/beau/Git/SerreConnectee/Server/talkArduino.py

HOSTNAME="${COLLECTD_HOSTNAME:-`hostname -f`}"
INTERVAL="${COLLECTD_INTERVAL:-60}"

while sleep "$INTERVAL"
do
  TEMP=`$ASKARDUINO temp | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    TEMP="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/Temperature interval=$INTERVAL N:$TEMP"

  HUMI=`$ASKARDUINO humidity | awk '{print $2}'`
  if [ $? -ne 0 ]
  then
    HUMI="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/Humidity interval=$INTERVAL N:$HUMI"

  LIGHT=`$ASKARDUINO light | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    LIGHT="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/Light interval=$INTERVAL N:$LIGHT"
  
done
