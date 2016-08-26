#!/usr/bin/python3
"""
read a two coloums data file(data.txt), with following format:
    P1 Q1
    P2 Q2
    ...
    P5 Q5
    --- 
    X1 Y1
    X2 Y2
    ...
    Xk Yk
where (Pi, Qi) will be scattered in the plot,  (Xi, Yi) will be linked together.
"""
import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('data.txt');
peaks = data[0:5, :];
movement = data[5:, :];

plt.scatter(peaks[:, 0], peaks[:, 1], c = 'red', marker = 'v');
plt.plot(movement[:, 0], movement[:, 1]);
plt.xlim(0, 100);
plt.ylim(0, 100);
plt.show();





