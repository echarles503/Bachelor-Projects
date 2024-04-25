""" Lab 7, Elias Charles"""
from typing import List


def total_sum(li) -> int:
    return sum(li)


def apply(f, li) -> List:
    return [f(i) for i in li]


def square(li) -> List:
    return [x**2 for x in li]


def magnitude(li) -> float:
    return total_sum(square(li))**.5


dispatch_table = {1: total_sum, 2: square, 3: magnitude}


class FunctionDispatcher:
    def __init__(self, table: dict):
        self.table = table

    def process_command(self, key: int, li):
        result = self.table[key](li)
        return result


if __name__ == "__main__":
    fd = FunctionDispatcher(dispatch_table)
    print(fd.process_command(1, [3, 4]))
    print(fd.process_command(2, [3, 4]))
    print(fd.process_command(3, [3, 4]))
