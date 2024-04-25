"""Unit tests for testme.py"""

import unittest
from buggy import *

class TestMaxRun(unittest.TestCase):

    def test_max_run_example(self):
        self.assertEqual(max_run([1, 2, 2, 2, 3]), [2, 2, 2])
        self.assertEqual(max_run([3, 4, 4]), [4, 4])
        self.assertEqual(max_run([5, 5, 5, 5]), [5, 5, 5, 5])


if __name__ == "__main__":
    unittest.main()

