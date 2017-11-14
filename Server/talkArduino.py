#!/usr/bin/env python

import serial
import sys

ser = serial.Serial('/dev/ttyACM1',9600)
ser.write('get ' + sys.argv[1] + ' \n')
ans=ser.readline()
pre=ans.split()[0]
if pre != 'ACK':
  sys.stderr.write(pre + '\n')
  exit(1)
else:
  sys.stdout.write(ans)

