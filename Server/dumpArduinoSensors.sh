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

  O2RATE=`$ASKARDUINO o2rate | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    O2RATE="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/o2_rate interval=$INTERVAL N:$O2RATE"

  CO2PPM="U"
  echo "PUTVAL $HOSTNAME/$PROGNAME/co2_ppm interval=$INTERVAL N:$CO2PPM"

  TEST=`$ASKARDUINO volt | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then 
    TEST="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/test_3_3V interval=$INTERVAL N:$TEST"

done
