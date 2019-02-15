#!/usr/bin/env python

import serial
import sys
import glob

tty=glob.glob('/dev/ttyACM*')

if len(tty) != 1:
  sys.stderr.write('No (unique) /dev/ttyACM found. Exiting.');
  exit(1)

ser = serial.Serial(tty[0],9600,timeout=1)

command=""
i=0
for s in sys.argv:
	if i:
		command += s + " " 
	i=i+1
command += "\n"

ser.write(command)

for l in ser:
	ll=l.splitlines()[0]
	print ll
