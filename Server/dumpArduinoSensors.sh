#!/bin/bash

PROGNAME="sensors"

#ASKARDUINO="/home/beau/Git/SerreConnectee/Server/talkArduino.py get " 
ASKARDUINO=/home/beau/Git/SerreConnectee/Server/getSensor.py

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
  HUMI=`$ASKARDUINO airhr | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    HUMI="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/air_humidity interval=$INTERVAL N:$HUMI"

  # --- GROUND HUMIDITY ----------------------------------------------------------------------
  GHUMI=`$ASKARDUINO gndhr | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    GHUMI="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/ground_humidity interval=$INTERVAL N:$GHUMI"

  # --- AMBIANT LIGHT -----------------------------------------------------------------
  LUX=`$ASKARDUINO lux | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    LUX="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/ambiant_light interval=$INTERVAL N:$LUX"

  # --- STOPSTEP ----------------------------------------------------------------------
  stopstep=`$ASKARDUINO stopstep | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    stopstep="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/stop_step_motor interval=$INTERVAL N:$stopstep"

  # --- FLUX ----------------------------------------------------------------------
  FLUX=`$ASKARDUINO flux | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    FLUX="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/energy_flux interval=$INTERVAL N:$FLUX"

  # --- GND TEMP ----------------------------------------------------------------------
  GNDTEMP=`$ASKARDUINO thermo | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    GNDTEMP="U"
  fi
  echo "PUTVAL $HOSTNAME/$PROGNAME/ground_temperature interval=$INTERVAL N:$GNDTEMP"

  # --- O2 RATE -----------------------------------------------------------------------
  #O2RATE=`$ASKARDUINO o2rate | awk '{print $2;}'`
  #if [ $? -ne 0 ]
  #then
  #  O2RATE="U"
  #fi
  #echo "PUTVAL $HOSTNAME/$PROGNAME/o2_rate interval=$INTERVAL N:$O2RATE"

  # --- CO2 in PPM --------------------------------------------------------------------
  #CO2PPM=`$ASKARDUINO co2ppm | awk '{print $2;}' | sed 's/$//'`
  #if [ $? -ne 0 ]
  #then
  #  CO2PPM="U"
  #elif [[ `echo "0. > $CO2PPM " | bc -l` -eq 1 ]]
  #then 
  #  CO2PPM="U" 
  #fi
  #echo "PUTVAL $HOSTNAME/$PROGNAME/co2_ppm interval=$INTERVAL N:$CO2PPM"

  # --- TEMPERATURE GIVEN BY THE CO2 SENSOR --------------------------------------------
  #TEMP2=`$ASKARDUINO temp2 | awk '{print $2;}' | sed 's/$//'` # Need to suppress \n at the end of the bc test below 
  #if [ $? -ne 0 ]
  #then
  #  TEMP2="U"
  #elif [[ `echo "-275. > $TEMP2 " | bc -l` -eq 1 ]] 
  #then
  #  TEMP2="U"
  #fi
  #echo "PUTVAL $HOSTNAME/$PROGNAME/co2_temperature interval=$INTERVAL N:$TEMP2"

  # --- Test of the 3.3V numerization ---------------------------------------------------
  #TEST=`$ASKARDUINO volt | awk '{print $2;}'`
  #if [ $? -ne 0 ]
  #then 
  #  TEST="U"
  #fi
  #echo "PUTVAL $HOSTNAME/$PROGNAME/test_3_3V interval=$INTERVAL N:$TEST"

# === WHAT ABOUT SWITCHES ? ============================================================

  # --- Test if light is on -------------------------------------------------------------
  sauto=`$ASKARDUINO auto | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then 
    sauto="U"
  fi
  echo "PUTVAL $HOSTNAME/switches/switch_auto interval=$INTERVAL N:$sauto"

  # --- Test if light is on -------------------------------------------------------------
  salarm=`$ASKARDUINO alarm | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    salarm="U"
  fi
  echo "PUTVAL $HOSTNAME/switches/switch_alarm interval=$INTERVAL N:$salarm"

  # --- Test if light is on -------------------------------------------------------------
  sfan=`$ASKARDUINO fan | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    sfan="U"
  fi
  echo "PUTVAL $HOSTNAME/switches/switch_fan interval=$INTERVAL N:$sfan"

  # --- Test if light is on -------------------------------------------------------------
  swater=`$ASKARDUINO water | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    swater="U"
  fi
  echo "PUTVAL $HOSTNAME/switches/switch_water interval=$INTERVAL N:$swater"

  # --- Test if light is on -------------------------------------------------------------
  smist=`$ASKARDUINO mist | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    smist="U"
  fi
  echo "PUTVAL $HOSTNAME/switches/switch_mist interval=$INTERVAL N:$smist"

  # --- Test if light is on -------------------------------------------------------------
  LIGHT=`$ASKARDUINO light | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    LIGHT="U"
  fi
  echo "PUTVAL $HOSTNAME/switches/switch_light interval=$INTERVAL N:$LIGHT"

  # --- Test if light is on -------------------------------------------------------------
  sheat=`$ASKARDUINO heat | awk '{print $2;}'`
  if [ $? -ne 0 ]
  then
    sheat="U"
  fi
  echo "PUTVAL $HOSTNAME/switches/switch_heat interval=$INTERVAL N:$sheat"

done # OF LOOP
