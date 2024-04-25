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
            
            if dup_ct > high_ct:
                high_ct = dup_ct
            dup_ct = 1
            
    if dup_ct > high_ct:
        high_ct = dup_ct
        
    return high_ct
