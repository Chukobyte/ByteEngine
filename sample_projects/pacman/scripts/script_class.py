# script_class.py

import sample_projects.pacman.scripts.scene_tree as scene_tree
from sample_projects.pacman.scripts.vector2 import Vector2


class ScriptClass:
    def __init__(self, entity_id, position_x, position_y):
        self.entity_id = entity_id
        self._position = Vector2(position_x, position_y, entity_id=entity_id)
        self.velocity = Vector2(0.0, 0.0)
        scene_tree.add_entity(self.entity_id, self)

    @property
    def position(self):
        return self._position

    @position.setter
    def position(self, value):
        self._position.x += value.x
        self._position.y += value.y

    @property
    def animation_signal_id(self):
        return f"{self.entity_id}_animation"

    def __delete__(self):
        scene_tree.remove_entity(self.entity_id)

    def __str__(self):
        return f"(entity_id = {self.entity_id}, position = {self.position})"

    def __repr__(self):
        return f"(entity_id = {self.entity_id}, position = {self.position})"
