#!/usr/bin/env python3

import serial
import struct
import sys
import open3d as o3d
from scipy.spatial.transform import Rotation as R

ser = serial.Serial(sys.argv[1])

while True:
    d = ser.read()
    if d == b'\n':
        print("End of packet found, retrieving full packets")
        break
    print("Passing: ", d)

v = o3d.visualization.Visualizer()
v.create_window()
mesh = o3d.geometry.TriangleMesh.create_coordinate_frame()
v.add_geometry(mesh)
while True:
    data = ser.read(19)
    d = struct.unpack("<ccffffx", data);
    if d[0] != b'y' or d[1] != b'o':
        print("Malformed packet!")
        continue
    print(d)
    r = R.from_quat([d[3], d[4], d[5], d[2]])
    mesh.rotate(r.as_matrix(), center=(0,0,0))
    v.update_geometry(mesh)
    v.poll_events()
    v.update_renderer()
