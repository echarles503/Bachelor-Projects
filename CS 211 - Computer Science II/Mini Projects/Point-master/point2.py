"""move point x,y to dx,dy"""


class Point:
    def __init__(self, x:int, y:int):
        self.x = x
        self.y = y

    def move_to(self, dx: int, dy: int):
        self.x += dx
        self.y += dy

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __str__(self):
        return f"({self.x}, {self.y})"


point = Point(1, 2)
point.move_to(3, 4)
print(point)
