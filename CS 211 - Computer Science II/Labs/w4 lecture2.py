"""Trees"""


class Boxes:

    def total_size(self):
        raise NotImplementedError("Abstract class not implemented")


class BasicBox(Boxes):

    def __init__(self, color: str, size: int):
        self._color = color
        self._size = size

    def total_size(self) -> int:
        """ Returns total size of a basic box """
        return self._size


class Bag(BasicBox):

    def __init__(self, left: BasicBox, right: Boxes):
        self._left = left
        self._right = right

    def total_size(self) -> int:
        """ Returns total size of a basic box """
        left_total = self._left.total_size()
        right_total = self._right.total_size()
        return left_total + right_total

def main():
    sock_box = BasicBox("Blue", 5)
    shoe_box = BasicBox("Red", 8)
    clothes_box = BasicBox("Green", 12)
    hat_box = BasicBox("Yellow", 15)

    bag_one = Bag(sock_box,shoe_box)
    bag_two = Bag(clothes_box, hat_box)

    bigger_bag = Bag(bag_one, bag_two)

    print(f"This is the size of sock box:{sock_box.total_size()}")

if __name__ == __main__:
    main()