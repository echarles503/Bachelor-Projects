"""
The game state and logic (model component) of 512, 
a game based on 2048 with a few changes. 
This is the 'model' part of the model-view-controller
construction plan.  It must NOT depend on any
particular view component, but it produces event 
notifications to trigger view updates.

Author: Elias Charles
"""

from game_element import GameElement, GameEvent, EventKind
from typing import List, Tuple, Optional
import random

# Configuration constants
GRID_SIZE = 4


class Vec:

    """A Vec is an (x,y) or (row, column) pair that
    represents distance along two orthogonal axes.
    Interpreted as a position, a Vec represents
    distance from (0,0).  Interpreted as movement,
    it represents distance from another position.
    Thus we can add two Vecs to get a Vec.
    """
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other: "Vec") -> "Vec":
        return Vec(self.x + other.x, self.y + other.y)

    def __eq__(self, other) -> bool:
        if (self.x, self.y) == (other.x, other.y):
            return True
        else:
            return False


class Tile(GameElement):
    """A slidy numbered thing."""

    def __init__(self, pos: Vec, value: int):
        super().__init__()
        self.row = pos.x
        self.col = pos.y
        self.value = value

    def __str__(self):
        return str(self.value)

    def __repr__(self):
        """Not like constructor --- more useful for debugging"""
        return f"Tile[{self.row},{self.col}]:{self.value}"

    def __eq__(self, other: "Tile"):
        return self.value == other.value

    def move_to(self, new_pos: Vec):
        self.row = new_pos.x
        self.col = new_pos.y
        self.notify_all(GameEvent(EventKind.tile_updated, self))

    def merge(self, other: "Tile"):
        # This tile incorporates the value of the other tile
        self.value = self.value + other.value
        self.notify_all(GameEvent(EventKind.tile_updated, self))
        # The other tile has been absorbed.  Resistance was futile.
        other.notify_all(GameEvent(EventKind.tile_removed, other))


class Board(GameElement):
    """The game grid.  Inherits 'add_listener' and 'notify_all'
    methods from game_element.GameElement so that the game
    can be displayed graphically.
    """

    def __init__(self, rows=4, cols=4):
        super().__init__()
        self.rows = rows
        self.cols = cols
        self.tiles = []
        for row in range(rows):
            row_tiles = []
            for col in range(cols):
                row_tiles.append(None)
            self.tiles.append(row_tiles)

    def __setitem__(self, pos: Vec, tile: Tile):
        self.tiles[pos.x][pos.y] = tile

    def __getitem__(self, pos: Vec) -> Tile:

        return self.tiles[pos.x][pos.y]

    def _empty_positions(self) -> List[Vec]:
        """Return a list of positions of None values,
        i.e., unoccupied spaces.
        """
        empties = []
        for row in range(len(self.tiles)):
            for col in range(len(self.tiles[row])):
                if self.__getitem__(Vec(row, col)) is None:
                    empties.append(Vec(row, col))
        return empties

    def has_empty(self) -> bool:
        """Is there at least one grid element without a tile?"""
        return len(self._empty_positions()) != 0


    def place_tile(self, value=None):
        """Place a tile on a randomly chosen empty square."""
        empties = self._empty_positions()
        assert len(empties) > 0
        choice = random.choice(empties)
        row, col = choice.x, choice.y
        if value is None:
            # 0.1 probability of 4
            if random.random() < 0.1:
                value = 4
            else:
                value = 2
        new_tile = Tile(Vec(row, col), value)
        self.tiles[row][col] = new_tile
        self.notify_all(GameEvent(EventKind.tile_created, new_tile))
        return

    def in_bounds(self, pos: Vec) -> bool:
        """Is position (pos.x, pos.y) a legal position on the board?"""
        return (0 <= pos.x < self.rows) and (0 <= pos.y < self.cols)

    def to_list(self) -> List[List[int]]:
        """Test scaffolding: represent each Tile by its
        integer value and empty positions as 0
        """
        result = []
        for row in self.tiles:
            row_values = []
            for col in row:
                if col is None:
                    row_values.append(0)
                else:
                    row_values.append(col.value)
            result.append(row_values)
        return result

    def from_list(self, values: List[List[int]]):
        """Test scaffolding: set board tiles to the
        given values, where 0 represents an empty space.
        """
        for row in range(self.rows):
            for col in range(self.cols):
                if values[row][col] == 0:
                    self.tiles[row][col] = None
                else:
                    self.tiles[row][col] = Tile(Vec(row, col), values[row][col])
        return

    def slide(self, pos: Vec, dir: Vec):
        """Slide tile at row,col (if any)
        in direction (dx,dy) until it bumps into
        another tile or the edge of the board.
        """
        if self[pos] is None:
            return
        while True:
            new_pos = pos + dir
            if not self.in_bounds(new_pos):
                break
            if self[new_pos] is None:
                self._move_tile(pos, new_pos)
            elif self[pos] == self[new_pos]:
                self[pos].merge(self[new_pos])
                self._move_tile(pos, new_pos)
                break  # Stop moving when we merge with another tile
            else:
                # Stuck against another tile
                break
            pos = new_pos

    def _move_tile(self, old_pos: Vec, new_pos: Vec):
        """Updates the board with new position and value of tile"""
        val = self.__getitem__(old_pos)
        Tile.move_to(val, new_pos)
        self.__setitem__(old_pos, None)
        self.__setitem__(new_pos, val)

    def up(self):
        """+(-1,0)"""
        dir = (Vec(-1, 0))
        for row in range(self.rows):
            for col in range(self.cols):
                self.slide((Vec(row, col)), dir)

    def down(self):
        """+(1,0)"""
        dir = (Vec(1, 0))
        for row in range(self.rows):
            for col in reversed(range(self.cols)):
                self.slide((Vec(row, col)), dir)

       

    def right(self):
        """+(0,1)"""
        dir = (Vec(0, 1))
        for row in range(self.rows):
            for col in reversed(range(self.cols)):
                self.slide((Vec(row, col)), dir)

    def score(self) -> int:
        """Calculate a score from the board.
        (Differs from classic 1024, which calculates score
        based on sequence of moves rather than state of
        board.
        """
        score = 0
        for row in range(self.rows):
            for col in range(self.cols):
                score += self[Vec(row, col)].value
        return score

