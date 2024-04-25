"""Lecture Example: Observer Pattern """


class Listenable:
    """Abstract class. Do not implement"""

    def __init__(self):
        self.listeners = []

    def add_listener(self, listener: 'Listener'):
        self.listeners.append(listener)

    def notify_all(self, event: str):
        for listener in self.listeners:
            listener.notify(event)


class Listener:
    """Has a notify method"""
    def notify(self, event: str):
        raise NotImplementedError('Class Listener is abstract, do not implement.')


class Hammer(Listener):

    def notify(self, event: str):
        print(f"The {event} just got WHACKED!!!")


class Wrench(Listener):

    def notify(self, event: str):
        print(f"The {event} just got CRANKED!!!")


class Worker(Listenable):

    def __init__(self):
        super().__init__()

    def grab(self, thing: str):
        self.notify_all(thing)


bob = Worker()
mjolnir = Hammer()

bob.grab('nail')
bob.add_listener(mjolnir)
bob.grab('bolt')

wrench = Wrench()
bob.add_listener(wrench)
bob.grab('screw')