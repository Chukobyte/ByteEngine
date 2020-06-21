# vector2.py

import math
from enum import Enum

import game


class Direction(Enum):
    left = 0
    right = 1
    up = 2
    down = 3


class Vector2:
    entity_id = None

    def __init__(self, x, y, **kwargs):
        self.entity_id = kwargs.get("entity_id", None)
        self._x = x
        self._y = y
        self.direction = Direction.left

    @property
    def x(self):
        return self._x

    @x.setter
    def x(self, value):
        if not math.isclose(self._x, value, abs_tol=0.0000001):
            self._x = value
            if self.entity_id is not None:
                game.update_entity_position(
                    entity_id=self.entity_id, position=(int(self.x), int(self.y))
                )
                # print(f"{self.entity_id} position = {self.tuple(divisor=16)}, real_position = {self.tuple()}")

    @property
    def y(self):
        return self._y

    @y.setter
    def y(self, value):
        if not math.isclose(self._y, value, abs_tol=0.0000001):
            self._y = value
            if self.entity_id is not None:
                game.update_entity_position(
                    entity_id=self.entity_id, position=(int(self.x), int(self.y))
                )
                # print(f"{self.entity_id} position = {self.tuple(divisor=16)}, real_position = {self.tuple()}")

    def tuple(self, divisor=1):
        return f"({int(self.x / divisor)}, {int(self.y / divisor)})"

    @staticmethod
    def distance_vector(source_vector, target_vector):
        return math.sqrt(
            ((target_vector.x - source_vector.x) * (target_vector.x - source_vector.x))
            + (
                (target_vector.y - source_vector.y)
                * (target_vector.y - source_vector.y)
            )
        )

    @staticmethod
    def distance(source_position, target_position):
        return math.sqrt(
            (
                (target_position[0] - source_position[0])
                * (target_position[0] - source_position[0])
            )
            + (
                (target_position[1] - source_position[1])
                * (target_position[1] - source_position[1])
            )
        )

    def __str__(self):
        return f"({self.x}, {self.y})"

    def __repr__(self):
        return f"({self.x}, {self.y})"
