#!/usr/bin/env python3
"""Interactive serial console for Arduino. Usage: python3 serialConsole.py /dev/ttyACM0"""

import serial
import sys
import threading
import time

tty = sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyACM0'

arduino = serial.Serial(
    port=tty,
    baudrate=9600,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=0.1,
    xonxoff=False,
    rtscts=False,
    dsrdtr=False,   # prevents DTR toggle that resets the Arduino on connect
)

print(f"Connected to {tty} at 9600 baud. Type commands, Ctrl-C to quit.\n")

def reader():
    while True:
        try:
            line = arduino.readline()
            if line:
                print("<", line.decode(errors='replace').rstrip())
        except Exception:
            break

t = threading.Thread(target=reader, daemon=True)
t.start()

try:
    while True:
        cmd = input("> ")
        arduino.reset_input_buffer()
        arduino.write((cmd + "\n").encode())
        arduino.flush()
except KeyboardInterrupt:
    pass
finally:
    arduino.close()
    print("\nClosed.")
