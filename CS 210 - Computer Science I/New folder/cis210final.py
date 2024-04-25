'''
Title: Final Exam

Author: Madeline Porcaro

Credits: None
'''
import doctest

def rainData(f):
    '''(f: str -> dict)

    Reads file (f) and creates and returns a dictionary(rain_dict)

    Calls: None

    Called By: reportOneDay, main

    >>> rainData('rainfall_short.txt')
    {1: 0.2, 2: 4.0}

    '''
    with open(f, 'r') as checkf:
        # skip header
        checkf.readline()

        # read data from text file and create list
        checkf = checkf.readline()
        checkf_li = checkf.strip().split()

        # creates the dictionary
        rain_dict = {}
        for i in range(1,len(checkf_li)+1):
            rain_dict[i] = float(checkf_li[i-1])

        return rain_dict


def reportOneDay(date, rdict):
    '''(date: int, rdict dict -> None)

    Prints rainfall for from rdict and the date
    If ridct has a value over 4.0, print "HEAVY RAINFALL ALERT"

    Calls: rainData

    Called By: main

    '''
    
    # selects dictionary
    value = rdict[date]
    if value >= 4:
        print("HEAVY RAINFALL ALERT")
    print(f'The rainfall on day {date} was {value}.')
    
    return None

def main():
    '''
    main function
    '''

    filename = 'rainfall.txt'
    rain_data = rainData(filename)
    for i in rain_data:
        reportOneDay(i, rain_data)

    return None
    
if '__name__' == '__main__':
    main()
# print(doctest.testmod())
    
