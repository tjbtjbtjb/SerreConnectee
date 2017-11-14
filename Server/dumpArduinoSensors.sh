#!/bin/bash

PROGNAME="sensors"

ASKARDUINO=/home/beau/Git/SerreConnectee/Server/talkArduino.py

HOSTNAME="arduino0"
#.${COLLECTD_HOSTNAME:-`hostname -f`}"
INTERVAL="${COLLECTD_INTERVAL:-60}"

test $# -eq 1 && INTERVAL=$1

#
# === LOOP ==========================================================================-=
#
while sleep "$INTERVAL"
do
  # --- TEMPERATURE -------------------------------------------------------------------
  TEMP=`$ASKARDUINO temp | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    TEMP="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/air_temperature interval=$INTERVAL N:$TEMP"

  # --- HUMIDITY ----------------------------------------------------------------------
  HUMI=`$ASKARDUINO humidity | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    HUMI="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/air_humidity interval=$INTERVAL N:$HUMI"

  # --- AMBIANT LIGHT -----------------------------------------------------------------
  LIGHT=`$ASKARDUINO light | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    LIGHT="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/ambiant_light interval=$INTERVAL N:$LIGHT"

  # --- O2 RATE -----------------------------------------------------------------------
  O2RATE=`$ASKARDUINO o2rate | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    O2RATE="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/o2_rate interval=$INTERVAL N:$O2RATE"

  # --- CO2 in PPM --------------------------------------------------------------------
  CO2PPM=`$ASKARDUINO co2ppm | awk '{print $2;}' | sed 's/$//'`
  if [ $? -ne 0 ]
  then
    CO2PPM="U"
  elif [[ `echo "0. > $CO2PPM " | bc -l` -eq 1 ]]
  then 
    CO2PPM="U" 
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/co2_ppm interval=$INTERVAL N:$CO2PPM"

  # --- TEMPERATURE GIVEN BY THE CO2 SENSOR --------------------------------------------
  TEMP2=`$ASKARDUINO temp2 | awk '{print $2;}' | sed 's/$//'` # Need to suppress \n at the end of the bc test below 
  if [ $? -ne 0 ]
  then
    TEMP2="U"
  elif [[ `echo "-275. > $TEMP2 " | bc -l` -eq 1 ]] 
  then
    TEMP2="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/co2_temperature interval=$INTERVAL N:$TEMP2"

  # --- Test of the 3.3V numerization ---------------------------------------------------
  TEST=`$ASKARDUINO volt | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then 
    TEST="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/test_3_3V interval=$INTERVAL N:$TEST"

done # OF LOOP
