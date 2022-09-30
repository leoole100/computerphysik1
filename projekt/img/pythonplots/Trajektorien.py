#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 12 16:41:12 2022

@author: moritz
"""
import csv
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
from mpl_toolkits import mplot3d

filenames = ['UlyssesCartesianCoordinates.csv','trajectory.csv','earth.csv','jupiter.csv']
colors = ['red','blue','grey','black']
labels = ['historical data','numerical solution','earth trajectory', 'jupiter trajectory']

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.set_xlim(-5,2)
ax.set_ylim(-2,4)
ax.set_zlim(-1,4)
ax.set_xlabel('x in AU')
ax.set_ylabel('y in AU')
ax.set_zlabel('z in AU')

for filename,color, label in zip(filenames,colors,labels):
    x,y,z=[],[],[]
    
    with open(filename) as csv_file:
            reader = csv.reader(csv_file, delimiter=',')
            header_row = next (reader)   
            for index, column_header in enumerate(header_row):
                for row in reader:
                    xe=float(row[0])
                    ye=float(row[1])
                    ze=float(row[2])
                    x.append(xe)
                    y.append(ye)
                    z.append(ze)
    ax.plot3D(x,y,z,color,linewidth=0.5, label=label)
    



ax.plot3D([0],[0],[0],'orange',marker='.', label = 'sun',markersize=10,linewidth=0)


plt.legend()
plt.savefig("trajectoryswingbyby.png", dpi=300)



