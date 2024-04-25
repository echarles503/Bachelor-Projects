'''
Elias Charles
Lab 2c
Credit: CIS210

'''

def mysqrt(n,k):
    '''
    Generates an approximate square root for n,
    a positive integer, via an iterative process
    that runs k times.

    The approximate square root is returned.
    '''
    assert n > 0
    
    x = 1
    for i in range(k):
        
        x = .5 * (x + float(n)/x) 
    print(x)
    return x
    
mysqrt(4,10)
