"""Lab 05 - Aliasing"""

from typing import List, Set, Dict, Optional


class Student:

    def __init__(self, name: str,
                 interests: List[str]):
        self.name = name
        self.interests = interests
        self.freetimes = set([8, 9, 10, 11, 12, 13, 14, 15])
        self.meetings: List[int] = []

    def schedule_meeting(self, time: int):
        """Checks if a student is free at a given time."""
        if time in self.freetimes:
            self.meetings.append(time)
            self.freetimes.remove(time)


class Club:

    def __init__(self, name: str):
        self.name = name
        self.members: List[Student] = []
        self.meeting_time: Optional[int] = None

    def join(self, student: Student):
        """Function to add a student to the member list of a club."""
        self.members.append(student)

    def find_common_time(self) -> int:
        """Find the common time slot for all members to attend."""
        if len(self.members) == 0:
            return 0

        member1_times = self.members[0].freetimes
        for member in self.members[1:]:
            member1_times = member1_times.intersection(member.freetimes)

        if member1_times == set([]):
            return 0

        return member1_times.pop()

    def schedule(self, time: int):
        """Schedules the club meeting."""
        self.meeting_time = time
        for member in self.members:
            member.schedule_meeting(time)

    def __str__(self) -> str:
        member_names = [member.name for member in self.members]
        return f"{self.name} ({', '.join(member_names)})"


class ASUO:

    def __init__(self):
        self.students: List[Student] = []
        self.clubs: List[Club] = []

    def enroll(self, s: Student):
        """Adds a student to the list of students."""
        self.students.append(s)

    def form_clubs(self):
        """Forms a club."""
        clubs_to_form: Dict[str, Club] = {}
        for student in self.students:
            for interest in student.interests:
                if interest in clubs_to_form:
                    club = clubs_to_form[interest]
                else:
                    club = Club(interest)
                    clubs_to_form[interest] = club
                    self.clubs.append(club)

                club.join(student)


    def schedule_clubs(self):
        """Schedules clubs."""
        for club in self.clubs:
            time = club.find_common_time()
            if time != 0:
                club.schedule(time)

    def print_club_schedule(self):
        """Prints club schedule."""
        for club in self.clubs:
            if club.meeting_time is not None:
                print(f"{club} meets at {club.meeting_time}")


if __name__ == "__main__":
    asuo = ASUO()
    asuo.enroll(Student("Marty", ["badminton", "robotics"]))
    asuo.enroll(Student("Kim", ["backgammon"]))
    asuo.enroll(Student("Tara", ["robotics", "horticulture", "chess"]))
    asuo.enroll(Student("George", ["chess", "badminton"]))

    asuo.form_clubs()
    asuo.schedule_clubs()
    asuo.print_club_schedule()






