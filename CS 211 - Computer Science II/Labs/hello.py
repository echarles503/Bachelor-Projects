
"""
Test
"""
class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def move(self, dx: int, dy: int):
        y = self.y + dy
        x = self.x + dx
        return (x, y)


p1 = Point(3, 4)
moved = p1.move(1,2)
print(moved)
