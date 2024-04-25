'''
CIS210 Project 6-a Fall 2020

Author: [Solution]

Credits: N/A

Implement functions from text ch. 4 -
mean, median, mode, frequency table
'''

import doctest
import statistics

def mean(alist):
    '''(alist: list of numbers) -> number

    Return mean of alist (len(alist) > 0).

    >>> mean([3, 1, 2])    #normal
    2.0
    >>> mean([99])         #edge - one item list  
    99.0
    >>> mean([0, 0])       #list of all one item (0)
    0.0
    '''
    assert len(alist) > 0, 'in function mean, alist must be non-empty'
    
    mean = sum(alist) / len(alist)
    return mean

def isEven(n):
    '''(n: int) -> bool

    Return True if n is even,
    else return False

    >>> isEven(4)          #normal - True
    True
    >>> isEven(1)          #edge - False
    False
    >>> isEven(0)          #edge - True
    True
    '''
    return (n % 2) == 0

def median(alist):
    '''(alist: list of numbers) -> number

    Return median of alist (of len > 0).

    >>> median([5, 7, 1, 3])       #normal - even len list
    4.0
    >>> median([1, 2, 2, 3, 99])   #normal - odd len list
    2
    >>> median([99])               #edge - list len 1
    99
    >>> median([0, 0, 0, 0])       #list of all the same values 
    0.0
    '''
    assert len(alist) > 0, 'in function mean, alist must be non-empty'
    
    copyl = alist[:]
    copyl.sort()
    copylen = len(copyl)
    
    if isEven(copylen):
        rmid = copylen // 2
        lmid = rmid - 1
        medi = (copyl[lmid] + copyl[rmid]) / 2
    else:
        mid = copylen // 2
        medi = copyl[mid]

    return medi

def mode(alist):
    '''(alist: list of numbers) -> list

    Return mode(s) of alist. like statistics module multimode.

    Calls: genFreqTable

    >>> mode([1, 2, 2, 3, 99])      #normal 
    [2]
    >>> mode([99])                  #border - list len 1
    [99]
    >>> mode([0, 0, 1, 1])          #all values are a mode 
    [0, 1]
    >>> mode([5, 7, 1, 3])          #all values are a mode  
    [5, 7, 1, 3]
    '''
    countd = genFrequencyTable(alist)

    countli = countd.values()
    maxct = max(countli)

    #there may be more than one mode
    modeli = []
    for k in countd:
        if countd[k] == maxct:
            modeli.append(k)
            
    # better - use a list comprehension
    #modeli = [k for k in countd if countd[k] == maxct]
      
    return modeli

def genFrequencyTable(alist):
    '''(alist: list) -> dict

    Generate a frequency dictionary with
    number of occurrences of each item
    in alist.

    Called by:  frequencyTable, freqChart, mode

    >>> genFrequencyTable([1, 2, 3, 3, 1, 4, 5])     #normal
    {1: 2, 2: 1, 3: 2, 4: 1, 5: 1}
    >>> genFrequencyTable([])                        #border - empty list
    {}
    '''
    freqD = {}
    '''
    for item in alist:
        if item in freqD:
            freqD[item] += 1
        else:
            freqD[item] = 1
    '''
    # better (no conditional) - use dict get method
    for item in alist:
        freqD[item] = freqD.get(item, 0) + 1

    '''
    # or dict setdefault method
    for item in alist:
        freqD.setdefault(item, 0)
        freqD[item] += 1
    '''
    return freqD

def drawTable(freqD):
    '''(freqD: dict) -> None

    Display each key-value pair
    from freqD in a frequency table.
   
    Called by: frequencyTable

    > drawTable({1:2, 2:1, 3:2, 4:1, 5:1})
    ITEM  FREQUENCY
    1     2
    2     1
    3     2
    4     1
    5     1
    '''
    iteml = list(freqD)
    iteml.sort()
    #iteml.sort(reverse=True)

    #could use sorted function instead
    #iteml = sorted(freqD)
    
    #or, to sort by value 
    #iteml = sorted(freqD, key=freqD.__getitem__, reverse=True)

    print(f"{'ITEM':<6} {'FREQUENCY':<9}")

    for item in iteml:
        print(f'{item:<6} {freqD[item]:<9}')

    return 

def frequencyTable(alist):
    '''(alist: list of numbers) -> None

    Print frequency table of count
    of each number in alist.

    Calls:  genFrequencyTable, drawTable

    > frequencyTable([1, 3, 3, 2])          #normal
    ITEM   FREQUENCY
    1      1        
    2      1        
    3      2
    > frequencyTable([])                    #edge - empty list
    ITEM   FREQUENCY
    > frequencyTable([99])                  #edge - list len 1
    ITEM   FREQUENCY
    1      99
    '''
    freqD = genFrequencyTable(alist)
    drawTable(freqD)
 
    return 

shortlist = [1, 2, 3, 1, 2, 2, 2, 4, 4, 1, 4, 4]

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
           3.2, 5.2, 2.8, 3.2, 2.6, 5.3, 5.5,
           2.7, 5.2, 6.4, 4.2, 3.1, 2.8, 4.5,
           2.9, 3.1, 4.3, 4.9, 5.2, 2.6, 6.7,
           2.7, 4.9, 3.0, 4.9, 4.7, 2.6, 4.6,
           2.5, 3.2, 2.7, 6.2, 4.0, 4.6, 4.9,
           2.5, 5.1, 3.3, 2.5, 4.7, 2.5, 4.1,
           3.1, 4.6, 2.8, 3.1, 6.3]

def main():
    '''controller for earthquake data funcs'''
    frequencyTable(equakes)
    print('Mean value is:', mean(equakes))
    print('Python mean is:', statistics.mean(equakes))
    print('Median value is:', median(equakes))
    print('Python median is:', statistics.median(equakes))
    print('Mode is:', mode(equakes))
    print('Python multimode is:', statistics.multimode(equakes))

    return

def main():
    '''check functions using shortlist data'''
    frequencyTable(shortlist)
    print('Mean value is:', mean(shortlist))
    print('Python mean is:', statistics.mean(shortlist))
    print('Median value is:', median(shortlist))
    print('Python median is:', statistics.median(shortlist))
    print('Mode is:', mode(shortlist))
    print('Python multimode is:', statistics.multimode(shortlist))

    return 

if __name__ == '__main__':
    main()

doctest.testmod()

'''
>>> main()
ITEM   FREQUENCY
2.5    18       
2.6    15       
2.7    14       
2.8    12       
2.9    11       
3.0    4        
3.1    10       
3.2    6        
3.3    7        
3.9    1        
4.0    3        
4.1    3        
4.2    5        
4.3    6        
4.4    6        
4.5    6        
4.6    8        
4.7    6        
4.8    9        
4.9    9        
5.0    4        
5.1    6        
5.2    3        
5.3    4        
5.4    1        
5.5    1        
5.7    2        
5.8    1        
6.0    1        
6.2    1        
6.3    1        
6.4    1        
6.6    1        
6.7    1        
Mean value is: 3.780748663101608
Python mean is: 3.7807486631016043
Median value is: 3.3
Python median is: 3.3
Mode is: [2.5]
'''

'''
note:  Python 3.x and higher includes a statistics module
in the Python standard library.  mode behaves differently
than for our mode, when there is more than one mode:

>>> from statistics import mode, multimode
>>> mode([1, 1, 2, 2])
1
>>> multimode([1, 1, 2, 2])
[1, 2]
'''

          
    
    

    

    
    
