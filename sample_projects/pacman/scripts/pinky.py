# pinky.py

import game

import sample_projects.pacman.scripts.global_obj as global_obj
import sample_projects.pacman.scripts.scene_tree as scene_tree
from sample_projects.pacman.scripts.vector2 import Vector2, Direction
from sample_projects.pacman.scripts.ghost import Ghost, State


class Pinky(Ghost):
    def __create__(self):
        super().__create__()
        self.reset()

    def reset(self):
        super().reset()
        self.state = State.IN_HOUSE
        self.home_tile = Vector2(0, 0)
        self.direction = Direction.down
        self.direction_to_turn = self.direction
        self.exit_pellete_counter = 0
        game.play_animation(entity_id=self.entity_id, animation_name="down")

    def determine_target_tile(self):
        if self.state == State.CHASE:
            pacman_entity = scene_tree.get_entity("pacman")
            (
                pacman_grid_position_x,
                pacman_grid_position_y,
            ) = global_obj.level_grid.convert_position_to_grid(
                pacman_entity.position.x, pacman_entity.position.y
            )
            if pacman_entity.direction == Direction.left:
                pacman_grid_position_x -= 4
            elif pacman_entity.direction == Direction.right:
                pacman_grid_position_x += 4
            if pacman_entity.direction == Direction.up:
                pacman_grid_position_y -= 4
            elif pacman_entity.direction == Direction.down:
                pacman_grid_position_y += 4
            return (pacman_grid_position_x, pacman_grid_position_y)
        else:
            return super().determine_target_tile()
