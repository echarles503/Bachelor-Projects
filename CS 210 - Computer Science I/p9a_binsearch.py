'''
Title: Binary Search
Author: Elias Charles

Was working then started to get a weird syntax error....
'''

def isMemberI(aseq, target):
    midpoint = 0

    try:
        assert len(aseq) > 0
        while midpoint != target:
            if len(aseq) > 0:
                mid_indx = len(aseq) // 2
                midpoint = aseq[mid_indx]

                if target != midpoint:
                    if target > midpoint:
                        aseq = aseq[(mid_indx + 1):]

                    else:
                        aseq = aseq[:mid_indx]
                else:
                    return True
            else:
                return False
    except AssertionError:
        return False
    
def isMemberR(seq, n):
    '''(seq: sequence, n: item) -> bool

    Use binary search to check for membership
    of int n in sorted sequence, seq. Return
    True if n is a member, else return False.

    >>> isMemberR((1, 2, 3, 3, 4), 4)
    True
    >>> isMemberR ((1, 2, 3, 3, 4), 2)
    True
    >>> isMemberR('aeiou', 'i')
    True
    >>> isMemberR('aeiou', 'y')
    False
    '''
    if len(seq) == 0:
        return False
    else:
        mid = len(seq) // 2

        if seq[mid] == n:
            return True
        elif seq[mid] > n:
            return(isMemberR(seq[:mid], n))
        else:
            return(isMemberR(seq[mid+1:], n))

def bintest(f):
    function_name = f.__name__
    test_cases = [((1, 2, 3, 3, 4), 3, True), #basic = True
                ((1, 2, 3, 3, 4), 99, False), #basic = False
                ('aeiou', 'i', True),
                ('aeiou', 'y', False),
                ((1, 3, 5, 7), 4, False),
                ((23, 24, 25, 26, 27), 5, False),
                ((0, 1, 4, 5, 6, 8), 4, True),
                ((0, 1, 2, 3, 4, 5, 6), 3, True),
                ((1, 3), 1, True),
                ((2, 10), 10, True),
                ((99, 100), 101, False),
                ((42,), 42, True),
                ((43,), 44, False),
                ((), 99, False)]
    for tests in test_cases:
        boolean_operator = tests[2]
        if f(tests[0], tests[1]) == boolean_operator:
            print("Checking {0}{1}".format(function_name, (tests[0], tests[1])),
            print("its value {0} is correct!".format(f(tests[0], tests[1])))
        else:
            print("Checking {0}{1}".format(function_name, (tests[0], tests[1]))),
            print("Error: '{0}', expected'{1}'.".format(boolean_operator, f(tests[0], tests[1])))
    return

def main():
    '''
    Calls iterative and recursive binary search functions
    '''
    bintest(isMemberI)
    print()
    bintest(isMemberR)
    return None
