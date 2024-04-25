'''
Title: Rain Data
Author: 
Credits: n/a
'''
import doctest

def rainData(f):
    '''
    (f: str) -> dict

    Creates and returns a rain dictionary, where dictionary keys
    are the days of the month(1-15) and the associateds value is
    the rainfall for that day
    
    >>> rainData('rainfall.txt')
    {1: 0.2, 2: 4.0, 3: 0.16, 4: 0.2, 5: 0.16, 6: 0.15, 7: 0.2, 8: 0.12, 9: 0.15, 10: 0.16, 11: 0.16, 12: 0.16, 13: 0.15, 14: 0.16, 15: 0.2}
    '''
    
    with open(f, 'r') as myf:
        myf.readline()  #skip header

        #reads data and creates a list
        rainData = myf.readline()
        rainli = rainData.strip().split()

        #creating the dictionary
        rainDict = {}
        for i in range(1, len(rainli)+1):
            rainDict[i] = float(rainli[i-1])
            
    return rainDict

def reportOneDay(date, rdict):
    '''
    (date: int, rdict: Dict) -> None

    This function reports the rainfall for date:
    >>> reportOneDay(1, rainData('rainfall.txt')
    
    '''
    
    rainfall = rdict[date]  #rainfall for given date
    if rainfall >= 4:   #conditional for heavy rain alert
        print('HEAVY RAINFALL ALERT')
    print(f'The rainfall on day {date} was {rainfall}')

    return None

def main():
    f = 'rainfall.txt'
    rainDict = rainData(f)
    for i in rainDict:
        reportOneDay(i, rainDict)
        
    return None
#main()
if '__name__' == '__main__':
    main()

#print(doctest.testmod())
