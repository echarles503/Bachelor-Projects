class Superhero:

    def power(self) -> str:
        return f"Generic Superpower"

    def battle(self, challenge: str):
        return f"I will use my {self.power()} to defeat {challenge}"


class WonderWoman(Superhero):

    def power(self):
        return f"Lasso"


superhero = Superhero()
conflict = superhero.battle("Villain")
print(conflict)

hero = WonderWoman()
conflict = hero.battle("Cheetah")
print(conflict)
