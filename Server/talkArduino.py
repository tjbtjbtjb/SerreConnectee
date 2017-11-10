#!/usr/bin/env python

import serial
import sys

ser = serial.Serial('/dev/ttyACM0',9600)
ser.write('get ' + sys.argv[1] + ' \n')
print ser.readline()

