#!/bin/bash

LOCALPATH=/home/greenhouse/Git/SerreConnectee/Server
TALKARDUINO=$LOCALPATH/talkArduino_v4.py
LISTMEGA=$LOCALPATH/listMega.sh

ARDUINO_TARGET=`echo $@ | awk '{print $1}'`
ARGS=`echo $@ | sed 's/'$ARDUINO_TARGET' //'`

TTY=`$LISTMEGA | grep $ARDUINO_TARGET | awk -F: '{print $1}'`

! test -n "$TTY" && echo "Arduino board $ARDUINO_TARGET not found on USB bus." >&2 && exit 11

$TALKARDUINO $TTY $ARGS
