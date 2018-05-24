import os 
import sys
import re
import numpy as np
from matplotlib import pyplot
from pylab import genfromtxt
from os import listdir
from os.path import isfile, join

def getProcs (mat):
    procs = []
    for i in range(len(mat[:,0])):
        procs.append(str(mat[i,0]))
    return procs

def getSpeedup (mat):
    rows = len(mat[0:,0])
    cols = len(mat[0,1:])
    speedup = np.zeros((rows,cols))
    for i in range(len(mat[0:,0])):
        for j in range(len(mat[i,1:])):
            serial = mat[0,j+1]
            parallel = mat[i,j+1]
            speedup[i,j] = serial / parallel
    return speedup
    
def getEff (mat, proc):
    rows = len(mat[0:,0])
    cols = len(mat[0,1:])
    eff = np.zeros((rows,cols))
    for i in range(rows):
        for j in range(cols):
            serial = mat[0,j+1]
            parallel = mat[i,j+1]
            eff[i,j] = float((serial / parallel)) / float(proc[i])
    return eff

def getSizes ():
    i = 0
    tam = (100000)
    #files = os.listdir("./Inputs")
    #files.sort()
    #tam = (1048576, 2097152)
    #sizes = np.zeros(len(tam))
    #for t in tam:
    #    number = int(t)
    #    sizes[i] = number
    #    i = i + 1
    
    return tam

def plotExecutionTimes (procs,sizes,mat):
    pyplot.grid()
    pyplot.title("Execution times")
    pyplot.xlabel("Processes")
    pyplot.ylabel("Time elapsed (s)")
    for i in range(len(sizes)):
        pyplot.plot(mat[:,0], mat[:,i+1], label="n = "+str(sizes[i]), marker='o')
    pyplot.legend(loc=0)
    pyplot.savefig("./Plot/times.pdf")
    pyplot.clf()

def plotSpeedups (procs,sizes,speedups):
    pyplot.grid()
    pyplot.title("Speedups")
    pyplot.xlabel("Processes")
    pyplot.ylabel("Speedup")
    for i in range(len(sizes)):
        pyplot.plot(procs[0:], speedups[:,i], label="n = "+str(sizes[i]), marker='o')
    pyplot.legend(loc=0)
    pyplot.savefig("./Plot/speedup.pdf")
    pyplot.clf()

if (len(sys.argv) < 2 ):
    print("================================================================")
    print("Usage:> python " + sys.argv[0] + " <time_file>")
    print("================================================================")
else:
    filename = sys.argv[1]
    mat = genfromtxt(filename)
    procs = getProcs(mat)
    speedups = getSpeedup(mat)
    print(speedups)
    sizes = getSizes()
    plotExecutionTimes(procs,sizes,mat)
    plotSpeedups(procs,sizes,speedups)
    print(getEff(mat, procs))
