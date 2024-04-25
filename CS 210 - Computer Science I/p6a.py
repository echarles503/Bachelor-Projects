import statistics


def mean(aList):
    mean = sum(aList) / len(aList)
    return mean

def frequencyTable(aList):
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
