# blinky.py

import game

import sample_projects.pacman.scripts.global_obj as global_obj
import sample_projects.pacman.scripts.scene_tree as scene_tree
from sample_projects.pacman.scripts.vector2 import Vector2, Direction
from sample_projects.pacman.scripts.ghost import Ghost, State


class Blinky(Ghost):
    def __create__(self):
        super().__create__()
        self.reset()

    def reset(self):
        super().reset()
        self.state = State.SCATTER
        self.home_tile = Vector2(31, 0)
        self.direction = Direction.left
        self.direction_to_turn = self.direction
        game.play_animation(entity_id=self.entity_id, animation_name="left")

    def determine_target_tile(self):
        # TODO: Implement override for scatter
        if self.state == State.SCATTER:
            return (self.home_tile.x, self.home_tile.y)
        elif self.state == State.CHASE:
            pacman_entity = scene_tree.get_entity("pacman")
            return global_obj.level_grid.convert_position_to_grid(
                pacman_entity.position.x, pacman_entity.position.y
            )
        else:
            return super().determine_target_tile()
