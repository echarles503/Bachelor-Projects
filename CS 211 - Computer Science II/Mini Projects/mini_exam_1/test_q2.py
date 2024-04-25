"""Test cases for outdoor_passing"""

import unittest
from mini_exam import *

class Test_Passing(unittest.TestCase):
    """Testing the outdoor_passing function,
    which uses the Room class.
    """

    def test_empty_class_list(self):
        """If we don't have to go anywhere,
        we can just stay in bed and not go
        outdoors at all.
        """
        self.assertEqual(outdoor_passing([]), 0)

    def test_just_one(self):
        """If you've only got one class,
        then likewise you never go outdoors
        to get from one to another
        """
        easy_day = [ Room("Straub", 150)]
        self.assertEqual(outdoor_passing(easy_day), 0)

    def test_typical_day(self):
        """The example from the HOWTO"""
        tuesday = [Room("Straub", 150),
                   Room("Willamette", 150),
                   Room("Willamette", 238),
                   Room("Chapman", 218),
                   Room("Chapman", 13)
                   ]
        self.assertEqual(outdoor_passing(tuesday), 2)

    def test_busy_day(self):
        """Back and forth and back and forth"""
        monday = [ Room("Straub", 150),
                   Room("Willamette", 150),
                   Room("Chapman", 218),
                   Room("Willamette", 238),
                   Room("Chapman", 13)
                   ]
        self.assertEqual(outdoor_passing(monday), 4)

if __name__ == "__main__":
    unittest.main()