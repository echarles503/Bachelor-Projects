"""Elias Charles... My extract method is ugly asf lmaooo"""

class BinaryNumber:
    def __init__(self, bits: [int]):
        self.bits = bits

    def __or__(self, other):
        if len(self.bits) != len(other.bits):
            return AssertionError("Bit length needs to be the same on both lines")
        li = []
        for i in range(len(self.bits)):
            if self.bits[i] == 1 or other.bits[i] == 1:
                li.append(1)
            else:
                li.append(0)
        return BinaryNumber(li)

    def __and__(self, other):
        if len(self.bits) != len(other.bits):
            return AssertionError("Bit length needs to be the same on both lines")
        li = []
        for i in range(len(self.bits)):
            if self.bits[i] == 1 and other.bits[i] == 1:
                li.append(1)
            else:
                li.append(0)
        return BinaryNumber(li)

    def __str__(self):
        return f"{self.bits}"

    def left_shift(self):
        for i in range(len(self.bits) - 1):
            self.bits[i] = self.bits[i+1]
        self.bits[-1] = 0

    def right_shift(self):
        temp = self.bits.copy()
        for i in range(len(self.bits) - 1):
            self.bits[i+1] = temp[i]
        self.bits[0] = 0

    def extract(self, start: int, end: int):
        assert 0 <= start < end < len(self.bits)
        num_bits = end - start + 1
        copy = self.bits.copy()
        mask = []
        for i in range(len(copy) - num_bits):
            mask.append(0)
        for j in range(num_bits):
            mask.append(1)
        for i in range(start):
            BinaryNumber(copy).right_shift()
        extract = BinaryNumber(copy) & BinaryNumber(mask)
        return extract


bn = BinaryNumber([1,0,1,0,1])
bn2 = BinaryNumber([1,1,1,0,0])

print("1st binary number =",bn)
print("2nd binary number =", bn2)

print("AND", bn & bn2)

print("OR", bn | bn2)

bn.right_shift()
print("1st number right-shifted =", bn)

bn.left_shift()
print("1st number left-shifted =", bn)

bn = BinaryNumber([1, 0, 0, 1, 0, 1, 1, 1])
extracted = bn.extract(2, 4)
print(extracted)