# -*- coding: utf-8 -*-
"""
Created on Sat May  5 23:02:02 2018

@author: Jia Lihong
"""

import json
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def funcNLogN(x, a):
    return a * np.multiply(np.log2(x), x, dtype=np.float64)
def funcNSquared(x, a):
    return a * np.multiply(x, x, dtype=np.float64)

def drawAndFit(xdata, ydata, name, mark="-"):
    plt.plot(xdata, ydata, 'b-' + mark, label=name)
    popt, pcov = curve_fit(funcNLogN, xdata, ydata)
    plt.plot(xdata, funcNLogN(xdata, *popt), 'g-' + mark, label = 'O(NLogN): a=%5.10f' % tuple(popt))
    popt, pcov = curve_fit(funcNSquared, xdata, ydata)
    plt.plot(xdata, funcNSquared(xdata, *popt), 'r-' + mark, label = 'O(N*N): a=%5.10f' % tuple(popt))

def drawSerial(benchmarks, name, mark):
    drawAndFit(np.array(benchmarks[name][0]), np.array(benchmarks[name][1]), name, mark)

if __name__ == "__main__":
    jsonData = {}

    with open("bm_int.json", "r") as f:
        jsonData = json.loads(f.read())
        
        benchmarkData = jsonData["benchmarks"]
        benchmarks = {}
        for item in benchmarkData:
            bmName = item["name"].split("/")
            name = bmName[0]
            xdata = int(bmName[1])
            ydata = item["cpu_time"]
            if name not in benchmarks:
                benchmarks[name] = [[], []]
                benchmarks[name][0].append(xdata)
                benchmarks[name][1].append(ydata)
            else:
                benchmarks[name][0].append(xdata)
                benchmarks[name][1].append(ydata)

    drawSerial(benchmarks, "STL_PQ_Push<Node>", "+")
    drawSerial(benchmarks, "MPQ_Push<Node>", "*")

    #drawSerial(benchmarks, "STL_PQ_Pop<Node>", "+")
    #drawSerial(benchmarks, "MPQ_Pop<Node>", "*")

    plt.xlabel('n')
    plt.ylabel('time(us)')
    plt.legend()
    plt.show()

