import statistics
import doctest
'''
Title: Data Analysis
Author: Elias Charles
Credit: n/a

Demonstrates that the functions: mean, median, mode, and isEven work correctly
'''
def mean(aList):
    '''(aList: list) --> float

    >>>mean([1,2,3,4])
    2.5
    >>>mean([0,0,0,1,5])
    1.2
    '''
    mean = sum(aList) / len(aList)
    
    return mean

def median(aList):
    '''(aList: list) -> float

    >>>median([1,2,3,4])
    2.5
    >>>median([0,0,0,1,1,5])
    0.5
    '''
    copyList = aList[:] #make a copy using slice operator
    copyList.sort()    #sort the copy
    if isEven(len(copyList)) == True: #even length
        rightMid = len(copyList) // 2
        leftMid = rightMid - 1
        median = (copyList[leftMid] + copyList[rightMid]) / 2
    else:   #odd length
        mid = len(copyList) // 2
        median = copyList[mid]

    
    return median

def mode(aList):
    '''(aList: list) -> list

    >>>mode([1,2,3,4])
    [1,2,3,4]
    >>>mode([1,0,1,0,1])
    [1]
    '''
    countDict = {}

    for item in aList:
        if item in countDict:
            countDict[item] = countDict[item] + 1
        else:
            countDict[item] = 1
    countList = countDict.values()
    maxCount = max(countList)

    modeList = []
    for item in countDict:
        if countDict[item] == maxCount:
            modeList.append(item)
                
    return modeList

def frequencyTable(aList):
    ''' (aList: list) -> None

    This function prints a frequency table
    '''
    countDict = {}

    for item in aList:
        if item in countDict:
            countDict[item] = countDict[item] + 1
        else:
            countDict[item] = 1

    itemList = list(countDict.keys())
    itemList.sort()

    print("ITEM","FREQUENCY")
    
    for item in itemList:
        print(item, "    ", countDict[item])

    return


def isEven(n):
    '''(n: int) -> bool

    >>>isEven(4)
    True
    >>>isEven(5)
    False
    '''
    if (n % 2) == 0:
        return True
    else:
        return False
    
def genFrequencyTable(aList):
    ''' (aList: list) -> list
    
    '''
    countDict = {}

    for item in aList:
        if item in countDict:
            countDict[item] = countDict[item] + 1
        else:
            countDict[item] = 1

    countList = countDict.values()
    maxCount = max(countList)

    modeList = []
    for item in countDict:
        if countDict[item] == maxCount:
            modeList.append(item)
    print(countDict)
    return countDict

def main():
    '''
    This the main program driver. It calls and prints the median/mean/mode functions
    and compares them to the statistics module value.
    '''
    equakes = [5.3, 3.0, 2.6, 4.4, 2.9, 4.8, 4.3,
    2.6, 2.9, 4.9, 2.5, 4.8, 4.2, 2.6,
    4.8, 2.7, 5.0, 2.7, 2.8, 4.3, 3.1,
    4.1, 2.8, 5.8, 2.5, 3.9, 4.8, 2.9,
    2.5, 4.9, 5.0, 2.5, 3.2, 2.6, 2.7,
    4.8, 4.1, 5.1, 4.7, 2.6, 2.9, 2.7,
    3.3, 3.0, 4.4, 2.7, 5.7, 2.5, 5.1,
    2.5, 4.4, 4.6, 5.7, 4.5, 4.7, 5.1,
    2.9, 3.3, 2.7, 2.8, 2.9, 2.6, 5.3,
    6.0, 3.0, 5.3, 2.7, 4.3, 5.4, 4.4,
    2.6, 2.8, 4.4, 4.3, 4.7, 3.3, 4.0,
    2.5, 4.9, 4.9, 2.5, 4.8, 3.1, 4.9,
    4.4, 6.6, 3.3, 2.5, 5.0, 4.8, 2.5,
    4.2, 4.5, 2.6, 4.0, 3.3, 3.1, 2.6,
    2.7, 2.9, 2.7, 2.9, 3.3, 2.8, 3.1,
    2.5, 4.3, 3.2, 4.6, 2.8, 4.8, 5.1,
    2.7, 2.6, 3.1, 2.9, 4.2, 4.8, 2.5,
    4.5, 4.5, 2.8, 4.7, 4.6, 4.6, 5.1,
    4.2, 2.8, 2.5, 4.5, 4.6, 2.6, 5.0,
    2.8, 2.9, 2.7, 3.1, 2.6, 2.5, 3.2,
    .2, 5.2, 2.8, 3.2, 2.6, 5.3, 5.5,
    2.7, 5.2, 6.4, 4.2, 3.1, 2.8, 4.5,
    2.9, 3.1, 4.3, 4.9, 5.2, 2.6, 6.7,
    2.7, 4.9, 3.0, 4.9, 4.7, 2.6, 4.6,
    2.5, 3.2, 2.7, 6.2, 4.0, 4.6, 4.9,
    2.5, 5.1, 3.3, 2.5, 4.7, 2.5, 4.1,
    3.1, 4.6, 2.8, 3.1, 6.3]
    funcMean = mean(equakes)
    funcMedian = median(equakes)
    funcMode = mode(equakes)
    #frequencyTable(equakes)
    
    pythonMean = statistics.mean(equakes)
    pythonMode = statistics.mode(equakes)
    pythonMedian = statistics.median(equakes)

    print("Mean value is:", funcMean)
    print("Python mean is:", pythonMean)
    print("Median value is:", funcMedian)
    print("Python median is:", pythonMedian)
    print("Mode is:", funcMode)
    print("Python multimode is:", pythonMode)
          
    #print(pythonMean,pythonMode,pythonMedian)
    return






