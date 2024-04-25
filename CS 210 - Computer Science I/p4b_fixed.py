'''
Testing and debugging
CIS 210 Project 4-b Fall 2020

Title: 'Fixed'
Author: Elias Charles
Credits: Aidan Hart

Functions need to be tested.
'''
import doctest

def ratsBug(weight, rate):
    '''(weight: number, rate: number) -> None

    Print number of weeks it will
    take for a rat to weigh 1.5 times
    as much as its original weight
    (weight > 0) if it gains at rate (rate > 0).

    >>> ratsBug(10, .1)     # normal
    The rat weighs 16.1 after 5 weeks.
    >>> ratsBug(1, .5)      # edge - minimum weeks
    The rat weighs 1.5 after 1 weeks.
    >>> ratsBug(20, .5)      #edge - minimum weeks
    The rat weighs 30.0 after 1 weeks.
    >>> ratsBug(20, .1)      #normal
    The rat weighs 32.2 after 5 weeks.
    >>> ratsBug(40, .2)      #normal
    The rat weighs 69.1 after 3 weeks.
    '''
    weeks = 0
    total_weight = 1.5 * weight #added total_weight outside of for loop, to stop infinite loop
    while weight < total_weight: 
        weight += weight * rate
        weeks += 1
    
    weight = round(weight, 1)
    print('The rat weighs', weight, 'after', weeks, 'weeks.')
    return 

def countSeqBug(astr):
    '''(astr: str) -> int

    Returns the length of the longest recurring
    sequence in astr

    >>> countSeqBug('abccde')  # normal  	
    2
    >>> countSeqBug('')        # edge - empty string
    0
    >>> countSeqBug('aabbbbc') # edge - multiple recurring sequences 
    4
    >>> countSeqBug('aabccc')  # edge - multiple recurring sequences 
    3
    >>> countSeqBug('a aa') #edge - testing empty space 
    2
    '''
    if len(astr) != 0:
        prev_item = astr[0]
        dup_ct = 1
        high_ct = 1
    else:
        high_ct = 0
        dup_ct = 0
        
    for i in range(1, len(astr)):
        if astr[i] == prev_item:
            dup_ct += 1
        else:
            prev_item = astr[i]
            dup_ct = 1
            
        if dup_ct > high_ct:
            high_ct = dup_ct

        
    return high_ct

def my_averageBug(dataset):
    '''(dataset: str) -> float

    Returns average of values in input string values,
    but zeros do not count at all.  Return 0 if there
    is no real data.
    
    >>> my_averageBug('23')    #normal, no zeros
    2.5
    >>> my_averageBug('203')   #normal, a zero
    2.5
    >>> my_averageBug('0000')  #all zeros
    0
    >>> my_averageBug('1')     #single item string
    1.0
    >>> my_averageBug('05050505')  
    5.0
    >>> my_averageBug('')       #edge - empty string
    0
    >>> my_averageBug('10001') #normal, 3 zeros
    1.0
    >>> my_averageBug('4200')  #normal, 2 end zeros
    3.0
    '''
    count = 0
    total = 0
    for value in dataset:
        if value != '0':
            total += int(value) # use int to change string to integer
            count += 1  #nested count inside the if statement
            
    if count == 0:          #if count == 0 after the for loop, there is no data, return 0
        return 0
    else:                   #else, continue to calculate and return avg
        avg = total / count
    return avg

print(doctest.testmod())
