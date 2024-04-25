
def all_same(l: list) -> bool:
    """checks if all elements in a list are the same"""
    if len(l) == 0:
        return False
    flag = l[0]
    for item in l:
        if item != flag:
            return False
    return True


def dedup(l:list) -> list:
    """returns a list that removes consecutive duplicate elements"""
    result = []
    if len(l) == 0:
        return result
    for item in list:
        if item not in result[-1:]:
            result.append(item)
    return result



def max_run(l: list) -> int:
    """returns the number of the longest consecutive duplicate elements in a list"""

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
        if run_ctr > ctr:
            run_ctr = ctr
    return run_ctr


if __name__ == '__main__':
    assert all_same([1,1,1,1])
    assert all_same([1, 1, 1, 1])
    print((dedup([1, 1, 1, 1])))
    print((dedup([1, 1, 2, 2])))
    print(max_run([1, 2, 2, 3, 4]))
    print('passed')