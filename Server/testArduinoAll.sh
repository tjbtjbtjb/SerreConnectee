#!/bin/bash

ARDUINO_SCRIPT=./talkArduino_v3.py

x=1
while [ $x -gt 0 ] 
do
  echo "===>>> ITER $x <<<==="
  $ARDUINO_SCRIPT all
  if [ $? -eq 0 ]
  then
    x=`expr $x + 1`
    sleep 5
  else
    x=0
  fi
done

  
