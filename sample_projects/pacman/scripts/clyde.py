# clyde.py

import game

import sample_projects.pacman.scripts.global_obj as global_obj
import sample_projects.pacman.scripts.scene_tree as scene_tree
from sample_projects.pacman.scripts.vector2 import Vector2, Direction
from sample_projects.pacman.scripts.ghost import Ghost, State


class Clyde(Ghost):
    def __create__(self):
        super().__create__()
        self.reset()

    def reset(self):
        super().reset()
        self.state = State.IN_HOUSE
        self.home_tile = Vector2(0, 31)
        self.direction = Direction.up
        self.direction_to_turn = self.direction
        if global_obj.player_stats.level <= 1:
            self.exit_pellete_counter = 60
        elif global_obj.player_stats.level == 2:
            self.exit_pellete_counter = 50
        else:
            self.exit_pellete_counter = 0
        game.play_animation(entity_id=self.entity_id, animation_name="up")

    def determine_target_tile(self):
        if self.state == State.CHASE:
            grid_position = global_obj.level_grid.convert_position_to_grid(
                self.position.x, self.position.y
            )
            pacman_entity = scene_tree.get_entity("pacman")
            pacman_grid_position = global_obj.level_grid.convert_position_to_grid(
                pacman_entity.position.x, pacman_entity.position.y
            )
            if Vector2.distance(grid_position, pacman_grid_position) > 8:
                return pacman_grid_position
            else:
                return (self.home_tile.x, self.home_tile.y)
        else:
            return super().determine_target_tile()
