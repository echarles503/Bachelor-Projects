'''
Title:
Author:
Credits:
'''
from math import sqrt

def readFile(fname):
    with open(fname, 'r') as dataFile:
        dataDict: {}

        key = 0
        for aLine in dataFile:
            key += 1
            score = int(aLine)

            dataDict[key] = [score]

    return dataDict

def euclidD(point1, point2):    #points are equal-length lists or tuples
    total = 0
    for index in range(len(point1)):
        diff = (point1[index] - point2[index]) ** 2
        total = total + diff

    euclidDistance = sqrt(total)
    return euclidDistance
