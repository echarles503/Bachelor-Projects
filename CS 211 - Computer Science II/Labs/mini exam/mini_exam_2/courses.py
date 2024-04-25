"""Mini-exam: course conflict example
(very simplified to fit in mini-exam format)
"""
from typing import List

class Course:
    """Any course, scheduled or web-based (asynchronous)"""

    def conflicts_with(self, other: "Course") -> bool:
        """Does this course have a time commitment that conflicts
        with other course?
        """
        raise NotImplementedError("conflicts_with not implemented")

    def busy_at(self, hour: int) -> bool:
        """True if this course has a time commitment at
        time 'hour'
        """
        raise NotImplementedError("busy_at not implemented")


class LectureCourse(Course):
    """A lecture course has a scheduled lecture time, which must
    not conflict with the lecture time of any other course a student
    is taking.
    """
    def __init__(self, title: str, hour: int):
        self._hour = hour  # Simplified model of class times
        self.title = title

    def conflicts_with(self, other: "Course") -> bool:
        pass # FIXME

    def busy_at(self, hour: int) -> bool:
        pass # FIXME


class WebCourse(Course):
    """A web-based course has no scheduled lecture; it never
    conflicts with another course.
    """

    def __init__(self, title: str):
        self.title = title

    def conflicts_with(self, other: "Course") -> bool:
        pass # FIXME

    def busy_at(self, hour: int) -> bool:
        pass # FIXME


class Schedule:
    """Wraps a list of courses"""

    def __init__(self, courses: List[Course]):
        self._courses = courses

    def has_conflicts(self) -> bool:
        pass # FIXME

