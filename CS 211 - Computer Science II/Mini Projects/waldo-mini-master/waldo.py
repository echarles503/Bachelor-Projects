""" Universal and existential quantifiers"""
import numpy as np
Waldo = 'W'
Other = '.'


def all_row_exists_waldo(collection) -> bool:
    for row in range(len(collection)):
        row_value = []
        for col in range(len(collection[row])):
            if collection[row][col] is not Other:
                row_value.append(collection[row][col])
        if len(row_value) == 0:
            return False
    return True


def all_col_exists_waldo(collection) -> bool:
    for row in range(len(collection)):
        col_value = []
        for col in range(len(collection[row])):
            if collection[col][row] is not Other:
                col_value.append(collection[col][row])
        if not collection[row]:
            return True
        if len(col_value) == 0:
            return False
    return True


def all_row_all_waldo(collection) -> bool:
    for row in range(len(collection)):
        for col in range(len(collection[row])):
            if collection[row][col] is Other:
                return False
        if not collection[row]:
            return True
    return True


def all_col_all_waldo(collection) -> bool:
    for row in range(len(collection)):
        for col in range(len(collection[row])):
            if collection[row][col] is Other:
                return False
        if not collection[row]:
            return True
    return True


def exists_row_exists_waldo(collection) -> bool:
    for row in range(len(collection)):
        col_value = []
        for col in range(len(collection[row])):
            if collection[row][col] is not Other:
                col_value.append(collection[row][col])
        if not collection[row]:
            return False
        if len(col_value) != 0:
            return True
    return False


def exists_col_exists_waldo(collection) -> bool:
    for row in range(len(collection)):
        col_value = []
        for col in range(len(collection[row])):
            if collection[col][row] is not Other:
                col_value.append(collection[col][row])
        if not collection[row]:
            return False
        if len(col_value) != 0:
            return True
    return False


def exists_row_all_waldo(collection) -> bool:
    for row in range(len(collection)):
        ctr = 0
        for col in range(len(collection[row])):
            if collection[row][col] is Other:
                ctr = 0
                break
            else:
                ctr += 1
        if not collection[row]:
            return True
        if ctr > 0:
            return True
    return False


def exists_col_all_waldo(collection) -> bool:
    values = []
    for x in range(len(collection)):
        for y in range(len(collection[x])):
            if collection[x][y] is Waldo:
                values.append(y)
            if values.count(y) >= len(collection[x]):
                return True
        if not collection[x]:
            return False
    return False
