import doctest

def sscount(needle, haystack):
    '''
    >>> sscount('ss', 'assesses')
    2
    >>> sscount('!!','!!.!!.!!')
    3
    >>> sscount('b', 'accbbbd')
    3
    '''
    ctr = 0
    n = len(needle)
    for i in range(len(haystack)):
        if haystack[i:i+n] == needle:
            ctr += 1

    return ctr

print(doctest.testmod())
