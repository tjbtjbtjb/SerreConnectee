#!/usr/bin/env python

import serial
import sys
import glob

tty=glob.glob('/dev/ttyACM*')

if len(tty) != 1:
  sys.stderr.write('No (unique) /dev/ttyACM found. Exiting.');
  exit(1)

#print(tty[0])
ser = serial.Serial(tty[0],9600)
#print(ser.readline())
#ser.readline()

ser.write('get ' + sys.argv[1] + ' \n')
ans=ser.readline()
pre=ans.split()[0]
if pre != 'ACK':
  sys.stderr.write(pre + '\n')
  exit(2)
else:
  sys.stdout.write(ans)

