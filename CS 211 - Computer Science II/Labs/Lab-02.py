"""Fraction.py
Author - Elias Charles
Title: Lab 02
"""

class Fraction:

    def __init__(self, num: int, den: int):
        assert num > 0, "Numerator is not greater than 0"
        assert den > 0, "Denominator is not greater than 0"
        self.num = num
        self.den = den
        self.simplify()

    def __str__(self):
        return f"{self.num}/{self.den}"

    def __repr__(self):
        return f"Fraction({self.num},{self.den})"

    def __mul__(self, other):
        """Multiplies two Fraction objects"""
        return Fraction(self.num * other.num, self.den * other.den)

    def __add__(self, other):
        """Adds two Fraction objects"""
        return Fraction((self.num * other.den) + (other.num * self.den), self.den * other.den)


    def simplify(self) -> "Fraction":
        """Simplifies a fraction to its simplest form"""
        def gcd(a, b) -> int:
            while a != b:
                if a > b:
                    a -= b
                else:
                    b -= a
            return a
        _gcd = gcd(self.num, self.den)
        self.num = int(self.num / _gcd)
        self.den = int(self.den / _gcd)

if __name__ == '__main__':
    f1 = Fraction(4, 6)
    print(f1)
    f2 = Fraction(15,20)
    print(f2)
    print(f1 * f2)