'''
More Automated Testing and Debugging
CIS 210 Project 6-b Fall 2020

Credits: Elias Charles

Functions need to be tested and debugged;
write a function to automate testing of print_cal function (project 5-b)
'''
import doctest
from p5b_printcal import print_cal

def bigSalesBug(sales_list):
    '''(sales_list: list) -> number

    Returns sum of all sales for amounts at or over $40,000.
    sales_list has the record of all the sales.

    >>> bigSalesBug([40_000, 45.67, 19000.0, 25000, 100_000])
    140000.0
    '''
    total = 0.00                #mispelled object
    for sales in sales_list:    #missing semicolon
        if sales >= 40_000:     # added >= operator rather than >
            total = total + sales
    return total    #moved total out of the if loop

############

def findRangeBug(salesli):
    '''(salesli: list) -> tuple

    Returns largest and smallest number in non-empty salesli.
    (Note that Python has built in funcs max and min
    to do this, but not using them here, so we can
    work with the list directly.)

    >>> findRangeBug([40000, 45.67, 19000.0, 25000, 100000])
    (45.67, 100000)
    '''
    
    salesli.sort(key = float)   #.sort() doesn't need to be an object
    
    low = (salesli[0])
    high = (salesli[-1])
    return low, high

def salesReportBug(salesli):
    '''(salesli: list) --> None

    Prints report of sales totals for each day of week (salesli)
    and range of per-day sales. salesli is non-empty - 0 sales
    for any day are reported as 0.

    >>> salesReportBug([40000, 45.67, 19000.0, 25000, 100000])
    Weekly Range: $45.67 - $100,000.00
    <BLANKLINE>
    Mon          Tue          Wed          Thu          Fri         
    $40,000.00   $45.67       $19,000.00   $25,000.00   $100,000.00  
    '''
    #calculate and report low and high sales
    low = min(salesli)  #rather than calling findRange I used min and max functions lol
    high = max(salesli)
    print(f'Weekly Range: ${low:,.2f} - ${high:,.2f}\n')
    
    #print daily report header
    fw = 12
    print(f"{'Mon':<{fw}} {'Tue':<{fw}} {'Wed':<{fw}} {'Thu':<{fw}} {'Fri':<{fw}}")

    #report on sales per day from list data
    for sales in salesli:
        print(f'${float(sales):<{fw},.2f}', end='')
        
    return 

def test_calendar():
    tests = [(8, 2020), # start Sat / end midwk / 31
        (10, 2020), # start midweek / end Sat / 31
         (9, 1920),
             (3, 1950),
             ]
    
    
    for i in range(len(tests)):
        test = tests[i]
        a, b = test
        #print(a)
        #print(b)
        print_cal(a,b)
        #print_cal(test)
    return
test_calendar()
doctest.testmod()
