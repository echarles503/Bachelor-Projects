class Node:
    def __init__(self, value: int):
        self.value = value

    def sum_node_data(self):
        raise NotImplementedError("Each concrete Node class must implement sum_node_data")

    def __str__(self):
        raise NotImplementedError("Each concrete Node class must define __str__")

class Leaf(Node):

    def __init__(self, node_data: int):
        self.node_data = node_data

    def sum_node_data(self):
        return self.node_data

    def __str__(self):
        return f"{self.node_data}"

class Internal(Node):

    def __init__(self, node_data: int, left: Node, right: Node):
        self.node_data = node_data
        self.left = left
        self.right = right

    def sum_node_data(self):
        left_total = self.left.sum_node_data()
        right_total = self.right.sum_node_data()

        return self.node_data + left_total + right_total

    def __str__(self):
        left_string = str(self.left)
        right_string = str(self.right)
        data_string = str(self.node_data)
        return f"<{data_string}, {left_string}, {right_string}>"

def main():
    l1 = Leaf(3)
    l2 = Leaf(6)
    l3 = Leaf(9)
    i = Internal(7, l2, l3)
    root = Internal(5, l1, i)
    print(root.sum_node_data())
    print(root)

if __name__ == '__main__':
    main()
