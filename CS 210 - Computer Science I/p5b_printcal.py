import datetime
import doctest
debug = print
'''
Title: Print Calendar
Author: Elias Charles
Credits: n/a

This program prints out a calendar for a given month and year
'''

def print_cal(month, year):
    '''
    (month: number, year: number) -> object

    prints out a calendar for a given month and year
    '''

    MONTHS_DAYS = '31 28 31 30 31 30 31 31 30 31 30 31 '
    MONTHS_NAMES = 'Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec '

    adate = datetime.date(year, month, 1)
    startday = adate.isoweekday() % 7

    space = ' '
    indent = space * (startday * 3)

    m_multiplier = (month-1) * 4
    print_month = MONTHS_NAMES[m_multiplier:m_multiplier + 3]

    d_multiplier = (month - 1) * 3
    print_day = int(MONTHS_DAYS[d_multiplier:d_multiplier + 2])

    print(print_month, year)
    print(f'{"Su Mo Tu We Th Fr Sa"}')
    print(f'{indent}', end='')
    
    date = 0
    for i in range(7 - startday):
        date += 1
        print(f'{date:>2} ', end='')

    for i in range(3):
        print()
        for p in range(7):
            date+=1
            print(f'{date:>2} ', end='')
    print()

    for i in range(date, print_day):
        date+= 1
        print(f'{date:>2} ', end='')
    print()
    return

def main():
    year = input('enter a year:\n')
    month = input('enter a month:\n')
    year = int(year)
    month = int(month)
    print_cal(month, year)
    return

if __name__ == '__main__':
    main()

