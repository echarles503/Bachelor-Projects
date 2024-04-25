from unittest import TestCase
from Lab-02 import Fraction

class TestFraction(TestCase):
    def test_simplify_succeeds(self):
        f1 = Fraction (4,6)
        self.assertEqual(f1.simplify(), 2/3)