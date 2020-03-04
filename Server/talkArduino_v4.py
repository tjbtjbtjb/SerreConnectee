#!/usr/bin/env python2.7

import serial
import sys
import glob
import time 
import termios
import os
import fcntl
import errno

tty=sys.argv[1]
print(tty)
def mylock(path):
        try:
                fd = os.open(path, os.O_WRONLY | os.O_CREAT, 0o0600)
        except:
                raise Exception('Cannot open lockfile %r' % path)

        try:
                for x in range(100):
                        try:
                                fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
                                break
                        except IOError as err:
                                if err.errno != errno.EAGAIN:
                                        raise
                        #print("wait a bit...")
                        time.sleep(0.1)
                else:
                        raise Exception('Timeout trying to get lock on %r' % path)
                yield True
        finally:
                os.close(fd)

lockfilename='/tmp/arduino_' + str(os.getuid()) + '_' + os.path.basename(tty) + '.lock'
# previously /var/lock ...

for l in mylock(lockfilename):
 arduino = serial.Serial()
 arduino.port     = tty
 arduino.baudrate = 9600
 arduino.bytesize = serial.EIGHTBITS
 arduino.parity   = serial.PARITY_NONE
 arduino.stopbits = serial.STOPBITS_ONE
 arduino.timeout  = 4.5
 arduino.xonxoff  = 0
 arduino.rtscts   = 0
 arduino.dtr      = 0
 arduino.open()
 
 time.sleep(0.1)
 
 command=""
 i=0
 for s in sys.argv:
 	if i>1 :
 		command += s + " " 
 	i=i+1
 command += "\n"
 
 #print(command)
 arduino.reset_input_buffer()
 arduino.write(command)
 arduino.flush()
 
 for l in arduino:
 	ll=l.splitlines()[0]
 	print(ll)
 	ret=ll.split()
 	if len(ret) >= 1 :
 		if ret[0] == 'ACK' :
 			break	
 
 arduino.reset_input_buffer()
 arduino.reset_output_buffer()
 arduino.close()

