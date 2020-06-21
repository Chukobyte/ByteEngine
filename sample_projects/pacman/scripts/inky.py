# inky.py

import game

import sample_projects.pacman.scripts.global_obj as global_obj
import sample_projects.pacman.scripts.scene_tree as scene_tree
from sample_projects.pacman.scripts.vector2 import Vector2, Direction
from sample_projects.pacman.scripts.ghost import Ghost, State


class Inky(Ghost):
    def __create__(self):
        super().__create__()
        self.reset()

    def reset(self):
        super().reset()
        self.state = State.IN_HOUSE
        self.home_tile = Vector2(31, 31)
        self.direction = Direction.up
        self.direction_to_turn = self.direction
        if global_obj.player_stats.level <= 1:
            self.exit_pellete_counter = 30
        else:
            self.exit_pellete_counter = 0
        game.play_animation(entity_id=self.entity_id, animation_name="up")

    def determine_target_tile(self):
        if self.state == State.CHASE:
            (
                grid_position_x,
                grid_position_y,
            ) = global_obj.level_grid.convert_position_to_grid(
                self.position.x, self.position.y
            )
            # Get pacman grid position
            pacman_entity = scene_tree.get_entity("pacman")
            (
                pacman_grid_position_x,
                pacman_grid_position_y,
            ) = global_obj.level_grid.convert_position_to_grid(
                pacman_entity.position.x, pacman_entity.position.y
            )
            if pacman_entity.direction == Direction.left:
                pacman_grid_position_x -= 2
            elif pacman_entity.direction == Direction.right:
                pacman_grid_position_x += 2
            if pacman_entity.direction == Direction.up:
                pacman_grid_position_y -= 2
            elif pacman_entity.direction == Direction.down:
                pacman_grid_position_y += 2
            # Get blinky grid position
            blinky_entity = scene_tree.get_entity("blinky")
            (
                blinky_grid_position_x,
                blinky_grid_position_y,
            ) = global_obj.level_grid.convert_position_to_grid(
                blinky_entity.position.x, blinky_entity.position.y
            )
            return (
                grid_position_x + (pacman_grid_position_x - blinky_grid_position_x) * 2,
                grid_position_y + (pacman_grid_position_y - blinky_grid_position_y) * 2,
            )
        else:
            return super().determine_target_tile()
