#!/bin/bash

LISTMEGA=/home/beau/Git/SerreConnectee/Server/listMega.sh
TALKARDUINO=/home/beau/Git/SerreConnectee/Server/talkArduino_v4.py

#.${COLLECTD_HOSTNAME:-`hostname -f`}"
INTERVAL="${COLLECTD_INTERVAL:-60}"

test $# -eq 1 && INTERVAL=$1

declare -A Descr
Descr[LIVETIME]=status/live_time
Descr[LUX]=sensors/ambiant_lux
Descr[OUTTEMP]=sensors/out_air_temp
Descr[OUTAIRHR]=sensors/out_air_hr
Descr[INTTEMP]=sensors/int_air_temp
Descr[INTAIRHR]=sensors/int_air_hr
Descr[CO2PPM]=sensors/co2_ppm
Descr[GNDHR]=sensors/ground_hr
Descr[STOPSTEP]=sensors/stop_step
Descr[FLUX]=sensors/energy_flux
Descr[THERMO]=sensors/ground_temp
Descr[AUTO]=switches/switch_auto
Descr[ALARM]=switches/switch_alarm
Descr[FAN]=switches/switch_fan
Descr[WATER]=switches/switch_water
Descr[MIST]=switches/switch_mist
Descr[LIGHT]=switches/switch_light
Descr[HEAT]=switches/switch_heat
Descr[FLOW]=switches/water_flow
#
# === LOOP ==========================================================================-=
#
while sleep "$INTERVAL"
do
  for i in `$LISTMEGA`
  do
    d=`echo $i | awk -F: '{print $1}'`
    HOSTNAME=`echo $i | awk -F: '{print $2}'`
    #echo $d $HOSTNAME
    $TALKARDUINO $d all | grep Sensor | while read i 
    do
      ID=`echo $i | sed 's,.*(\(.*\)).*,\1,'`
      VAL=`echo $i | awk '{print $NF}'`
      test -n "${Descr[$ID]}" && echo PUTVAL $HOSTNAME/${Descr[$ID]} interval=$INTERVAL N:$VAL
    done
  done
done
