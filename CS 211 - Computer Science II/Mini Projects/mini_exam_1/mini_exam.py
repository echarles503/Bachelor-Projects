"""Mini-exam 1, week 2, CIS 211 Winter 2021
University of Oregon

Your name: Elias Charles
"""
from typing import List

# See the HOWTO for what to place below.

## Q1 asks you to build a class called Room ...
## Put it here ..


class Room:

    def __init__(self, building: str, room_num: int):
        """Initializes building and room number"""
        self.building = building
        self.room_num = room_num

    def __str__(self):
        """Prints objects as string"""
        return f"{self.building} {self.room_num}"

    def same_building(self, other: "Room") -> bool:
        """These two rooms are in the same building"""
        return self.building == other.building


## Q2 asks you to build a function
## whose header should look like
## def outdoor_passing(classes: List[Room]) -> int:
# Put it here ...


def outdoor_passing(classes: List[Room]) -> int:
    """Counts the number of times a student must go outdoors"""
    ctr = 0
    if len(classes) == 0:
        return 0
    for i in range(len(classes)-1):
        if Room.same_building(classes[i], classes[i+1]):
            pass
        else:
            ctr += 1
    return ctr
