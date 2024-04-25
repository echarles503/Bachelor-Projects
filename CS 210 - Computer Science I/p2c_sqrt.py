'''
Project: Approximate Square Root

Author: Elias C

Credit: n/a
'''
from math import sqrt

def mysqrt(n,k):
    '''
    Generates an approximate square root for n,
    a positive integer, via an iterative process
    that runs k times.

    The approximate square root is returned.
    '''
    x = 1
    for i in range(k):
        x = .5 * (x + float(n)/x)
    print(x)
    return x

def sqrt_compare(num, iterations):
    '''
    Compares the Babylonian method of sqrt generation
    to the more accurate defined sqrt function. Calculates
    difference between the two.
    '''
    print("For", num, "using", iterations, "iterations:")
    print("mysqrt value is:")
    temp = mysqrt(num, iterations) 
    print("math lib sqrt value is:", sqrt(num))
    difference = round((temp - sqrt(num)), 2)
    print("The difference is:", difference)
    
    return

def sqrt_compare_main():
    '''Square root comparison program driver.'''
    #sqrt_compare(25, 5)
    #sqrt_compare(25, 10)
    #sqrt_compare(625, 5)
    #sqrt_compare(625, 10)
    sqrt_compare(10000, 8)
    #sqrt_compare(10000, 10)
    return
sqrt_compare_main()
