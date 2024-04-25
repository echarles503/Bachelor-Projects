'''
Title: 'testfuncs'
Author: Elias Charles
Credits: n/a

This program tests outside modules by comparing the received integer result to
the expected result.
'''
import doctest
import lab3_sscount

def test_sscount(f, args, expected_result):
    '''
    (f: function, args: str, expected_result: int) --> None

    Calls the function sscount0 & sscount1 imported to the script from lab3_sscount.py, and checks if the arguments
    given cause the expected_result. Tests if the script is working correctly.

    >>>test_sscount(sscount0, 'sses assesses', 2)
    testing function sscount0
    checking arguments...
    received the expected value, 2 is correct!
    '''

    print('testing', f)
    needle = args[0:args.find(' ')]
    haystack = args[args.find(' ')+1:]
    
    print('checking', needle, haystack, '...')

    if f(needle, haystack) == expected_result:
        print('its value', expected_result,'is correct!')
        print('--------------------')
    else:
        print('the value of', f(needle,haystack),'is incorrect.')
        print('the correct value should be', expected_result)
        print('--------------------')
       
    return

def main():
    '''
    Repeatedly calls test_sscountt to test sscount0 and sscount1.

    Returns None
    '''
    test_sscount(lab3_sscount.sscount0, 'ass assesses', 1)
    test_sscount(lab3_sscount.sscount1, 'ass assesses', 1)
    test_sscount(lab3_sscount.sscount1, 'sses assesses', 2)
    test_sscount(lab3_sscount.sscount0,'sses assesses', 2)
    test_sscount(lab3_sscount.sscount1,'an trans-Panamanian banana', 6)
    test_sscount(lab3_sscount.sscount0,'needle haystack', 0)
    test_sscount(lab3_sscount.sscount1,'!!! !!!!!', 3)
    test_sscount(lab3_sscount.sscount0,'o pneumonoultramicroscopicsilicovolcanoconiosis', 9)
    test_sscount(lab3_sscount.sscount1,'', 0)
    test_sscount(lab3_sscount.sscount0,'a ', 0)
    test_sscount(lab3_sscount.sscount1,' abc', 0)
    #^^^this case doesnt work bc the space is on the edge. The string splitter will not separate the two.
    test_sscount(lab3_sscount.sscount0,'a a', 1)
    return
main()
