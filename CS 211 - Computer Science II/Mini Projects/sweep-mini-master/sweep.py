"""
Title: Sweep
Author: Elias C
Credits: N/A

This is an exercise in writing loops that calculate a result in a single sweep of a list.
aka: O(n): loops in linear time
"""


def all_same(l: list) -> bool:
    """Determines whether all the elements in a list are the same.
    >>> all_same([1,1,1,1])
    True
    >>> all_same([])
    True
    >>> all_same([1,3,1,1])
    False
    """
    if len(l) == 0:
        return True
    flag = l[0]
    for item in l:
        if item != flag:
            return False
    return True


def dedup(l: list):
    """
    Returns a 'de-duplicated' version of the input list.
    Elements appear in the same order as the input list,
    but subsequence of equal elements are replaced by one representative.
    >>> dedup([])
    []
    >>> dedup([1,1,2,1,1])
    [1, 2, 1]
    >>> dedup([1,2,3,1,2,3])
    [1, 2, 3, 1, 2, 3]
    >>> dedup([1,1,2,2,3,3])
    [1, 2, 3]
    """
    result = []
    if len(l) == 0:
        return result
    for item in l:
        if item not in result[-1:]:
            result.append(item)
    return result


def max_run(l: list) -> int:
    """Returns the value for longest run in a set of identical values."""

    if len(l) == 0:
        return 0
    ctr = 0
    run_ctr = 1
    flag = l[0]
    for item in l:
        if item == flag:
            ctr += 1
        else:
            flag = item
            ctr = 1
        if ctr > run_ctr:
            run_ctr = ctr
    return run_ctr
