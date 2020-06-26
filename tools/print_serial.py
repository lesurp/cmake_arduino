#!/usr/bin/env python3

import serial
import struct
import sys

ser = serial.Serial(sys.argv[1])

while True:
    d = ser.read()
    if d == b'\n':
        break
    print("Passing: ", d)

while True:
    data = ser.read(19)
    d = struct.unpack("<ccffffx", data);
    if d[0] != b'y' or d[1] != b'o':
        print("Malformed packet!")
        continue
    print(d)
