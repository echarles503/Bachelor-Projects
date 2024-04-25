"""Foods are recursive!
The base cases are basic ingredients like salt and flour.
The recursive cases are foods composed by combining other
foods, which may be basic or composite.
"""

from enum import Enum, auto
from typing import List, Set


class Irritant(Enum):
    """Includes allergens and other ingredients that
    people may want to avoid, e.g., a vegan may avoid
    milk for reasons other than allergy.
    """
    MILK = auto()
    SOY = auto()
    MEAT = auto()
    GLUTEN = auto()
    SUGAR_ADD = auto()
    CORN_SYRUP = auto()

    def __repr__(self):
        """Make lists of irritants more readable"""
        return f"{self.name}"


class Food:
    """Abstract base class."""

    def bad_ingredients(self, avoiding: List[Irritant]) -> List['Basic']:
        """Returns list of basic ingredients containing one of
        the irritants in 'avoiding'
        """
        raise NotImplementedError(
            f"bad_ingredients not implemented in {type(self)}")


class Basic(Food):
    def __init__(self, name: str, irritants: List[Irritant] = []):
        self._name = name
        self._irritants = set(irritants)

    def __str__(self) -> str:
        return self._name

    def __repr__(self):
        return f"Basic({self._name}, irritants={list(self._irritants)})"

    def bad_ingredients(self, avoiding: List[Irritant]) -> List['Basic']:
        if not self._irritants.isdisjoint(set(avoiding)):
            return [self]
        return []


class Composite(Food):
    def __init__(self, name: str, ingredients: List[Food]):
        self._name = name
        self._ingredients = ingredients

    def __str__(self) -> str:
        parts = ", ".join(str(i) for i in self._ingredients)
        return f"{self._name}: [{parts}]"

    def __repr__(self) -> str:
        return f"Composite('{self._name}, {repr(self._ingredients)})"

    def bad_ingredients(self, avoiding: List[Irritant]) -> List[Basic]:
        irritants = []
        for ingredient in self._ingredients:
            if ingredient.bad_ingredients(avoiding):
                for bad_ingredient in ingredient.bad_ingredients(avoiding):
                    irritants.append(bad_ingredient)
        return irritants


def main():
    """Smoke test: Pizza"""
    dough = Composite("dough",
                      [Basic("flour", irritants=[Irritant.GLUTEN]),
                       Basic("yeast"),
                       Basic("water")])
    salami = Basic("salami", irritants=[Irritant.MEAT])
    cheese = Basic("cheese", irritants=[Irritant.MILK])
    topping = Composite("topping", [cheese, salami])
    sauce = Composite("sauce", [Basic("tomato"), Basic("spices")])
    pizza = Composite("pizza", [dough, sauce, topping])
    print(f"Pizza: {pizza}")
    vegans_avoid = pizza.bad_ingredients([Irritant.MILK, Irritant.MEAT])
    names_bad = ", ".join(str(i) for i in vegans_avoid)
    print(f"Vegan?  Pizza contains {names_bad}")
    glutens = pizza.bad_ingredients([Irritant.GLUTEN])
    names_bad = ", ".join(str(i) for i in glutens)
    print(f"Gluten intolerant?  Pizza contains {names_bad}")

"""Expected output: 
Pizza: pizza: [dough: [flour, yeast, water], sauce: [tomato, spices], topping: [cheese, salami]]
Vegan?  Pizza contains cheese, salami
Gluten intolerant?  Pizza contains flour

(more tests in test_food.py)
"""

if __name__ == "__main__":
    main()
