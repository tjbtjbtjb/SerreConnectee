#!/usr/bin/env python2.7

import serial
import sys
import glob
import time 
import termios

tty=glob.glob('/dev/ttyACM*')

if len(tty) != 1:
  sys.stderr.write('No (unique) /dev/ttyACM found. Exiting.\n');
  exit(1)

arduino = serial.Serial()
arduino.port     = tty[0]
arduino.baudrate = 9600
arduino.bytesize = serial.EIGHTBITS
arduino.parity   = serial.PARITY_NONE
arduino.stopbits = serial.STOPBITS_ONE
arduino.timeout  = 1
arduino.xonxoff  = 0
arduino.rtscts   = 0
arduino.dtr      = 0
arduino.open()
time.sleep(1)

command=""
i=0
for s in sys.argv:
	if i:
		command += s + " " 
	i=i+1
command += "\n"

arduino.write(command)

for l in arduino:
	ll=l.splitlines()[0]
	print(ll)
