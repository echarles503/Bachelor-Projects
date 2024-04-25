debug = print

def median(aList):
    copyList = aList[:] #make a copy using slice operator
    copyList.sort() #sort the copy
    
    if len(copyList) % 2 == 0: #even length
        rightMid = len(copyList) // 2
        leftMid = rightMid - 1
        median = copyList[leftMid] + copyList[rightMid] / 2
    else:   #odd length
        mid = len(copyList) // 2
        median = copyList[mid]
    return median

def isEven(n):
    if (n % 2) == 0:
        return True
    else:
        return False

shortlist = [1, 2, 3, 1, 2, 2, 2, 4, 4, 1, 4, 4]

median(shortlist)
