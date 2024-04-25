"""
Author - Elias Charles
Title - Point
Credits - n/a

This program moves points to a different location (???)
"""

class Point:

    def __init__(self, x:int, y: int):
        self.x = x
        self.y = y

    def move(self, dx[]\
       ,dy):
        self.x += dx
        self.y += dy

    def __eq__(self, other):
        if self.x == other.x and self.y == other.y:
            return True
        else:
            return False

    def __str__(self):
        return f"({self.x}, {self.y})"