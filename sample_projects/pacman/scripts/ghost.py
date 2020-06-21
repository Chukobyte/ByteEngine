# ghost.py

import random
from enum import Enum

import game
import sample_projects.pacman.scripts.scene_tree as scene_tree

from sample_projects.pacman.scripts.script_class import ScriptClass
from sample_projects.pacman.scripts.vector2 import Vector2, Direction
from sample_projects.pacman.scripts.level_grid import LevelGrid
import sample_projects.pacman.scripts.global_obj as global_obj
from sample_projects.pacman.scripts.game_state import GameState


class State(Enum):
    IN_HOUSE = 0  # Initial state when game start in house
    LEAVE_HOUSE = 1
    CHASE = 2
    SCATTER = 3
    FRIGHTENED = 4
    DEFEATED_TRAVELING_TO_HOUSE = 5
    DEFEATED_ENTERING_HOUSE = 6


class Ghost(ScriptClass):
    MIDDLE_SPEED = 70
    NORMAL_SPEED = 100

    def __create__(self):
        self.speed = 100
        self.direction = Direction.left
        self.direction_to_turn = self.direction
        self.accumulated_delta = 0.0
        self.offset = Vector2(12, 12)
        self._state = State.IN_HOUSE
        self.previous_state = None
        self.target_tile = Vector2(0, 0)
        self.home_tile = Vector2(0, 0)
        self.ghost_house_tile = Vector2(13, 11)
        self.frightened_flash = False
        # Counters
        self.exit_pellete_counter = 0
        self.chase_state_counter = 0
        self.scatter_state_counter = 0
        # Timer
        self.chase_timer_id = f"{self.entity_id}_chase_timer"
        self.chase_timer_wait_time = 20.0
        self.scatter_timer_id = f"{self.entity_id}_scatter_timer"
        self.scatter_timer_wait_time = 9.0
        self.frightened_timer_id = f"{self.entity_id}_frightened_timer"
        self.frightened_timer_wait_time = 5.0
        self.frightened_flash_timer_id = f"{self.entity_id}_frightened_flash_timer"
        self.frightened_flash_timer_wait_time = 2.0

        game.create_timer(
            timer_id=self.chase_timer_id,
            time=self.chase_timer_wait_time,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.chase_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_chase_timer_timeout",
        )

        game.create_timer(
            timer_id=self.scatter_timer_id,
            time=self.scatter_timer_wait_time,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.scatter_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_scatter_timer_timeout",
        )

        game.create_timer(
            timer_id=self.frightened_timer_id,
            time=self.frightened_timer_wait_time,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.frightened_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_frightened_timer_timeout",
        )

        game.create_timer(
            timer_id=self.frightened_flash_timer_id,
            time=self.frightened_flash_timer_wait_time,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.frightened_flash_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_frightened_flash_timer_timeout",
        )

        pacman_entity = scene_tree.get_entity("pacman")
        game.subscribe_to_signal(
            source_id=pacman_entity.entity_id,
            signal_id="power_pellete_eaten",
            subscriber_entity_id=self.entity_id,
            function_name="_on_pacman_power_pellete_eaten",
        )

    @property
    def state(self):
        return self._state

    @state.setter
    def state(self, value):
        self.previous_state = self._state
        self._state = value
        if self._state == State.CHASE:
            if self.previous_state == State.FRIGHTENED:
                game.resume_timer(timer_id=self.chase_timer_id)
            else:
                self.chase_state_counter += 1
                if self.chase_state_counter < 4:
                    game.start_timer(timer_id=self.chase_timer_id)
                else:
                    game.start_timer(timer_id=self.chase_timer_id, time=1000)
                if self.previous_state != State.LEAVE_HOUSE and self.previous_state != State.IN_HOUSE and not global_obj.level_grid.has_turn_space(self.position.x, self.position.y):
                    self.flip_direction()
        elif self._state == State.SCATTER:
            if self.previous_state == State.FRIGHTENED:
                game.resume_timer(timer_id=self.scatter_timer_id)
            else:
                self.scatter_state_counter += 1
                scatter_start_time = self.scatter_timer_wait_time
                if self.scatter_state_counter >= 3:
                    scatter_start_time -= 2.0
                game.start_timer(
                    timer_id=self.scatter_timer_id, time=scatter_start_time
                )
                if self.previous_state != State.LEAVE_HOUSE and self.previous_state != State.IN_HOUSE and not global_obj.level_grid.has_turn_space(self.position.x, self.position.y):
                    self.flip_direction()
        elif self._state == State.FRIGHTENED:
            if self.previous_state == State.CHASE:
                game.pause_timer(self.chase_timer_id)
            elif self.previous_state == State.SCATTER:
                game.pause_timer(self.scatter_timer_id)
            self.frightened_flash = False
            game.start_timer(timer_id=self.frightened_timer_id)
            if not global_obj.level_grid.has_turn_space(self.position.x, self.position.y):
                self.flip_direction()
        self.target_tile = self.determine_target_tile()

    def reset(self):
        game.stop_timer(timer_id=self.chase_timer_id)
        game.stop_timer(timer_id=self.scatter_timer_id)
        game.stop_timer(timer_id=self.frightened_timer_id)
        game.stop_timer(timer_id=self.frightened_flash_timer_id)

    def flip_direction(self):
        if self.direction == Direction.left:
            self.direction = Direction.right
        elif self.direction == Direction.right:
            self.direction = Direction.left
        if self.direction == Direction.up:
            self.direction = Direction.down
        elif self.direction == Direction.down:
            self.direction = Direction.up

    def __process__(self, delta_time):
        if global_obj.game_started and not global_obj.game_paused:

            # Determine speed
            grid_space_x, grid_space_y = self.get_level_grid_position(
                self.position.x, self.position.y
            )
            if global_obj.level_grid.has_ghost_slow_down_space(
                grid_space_x, grid_space_y
            ):
                self.speed = self.MIDDLE_SPEED
            else:
                self.speed = self.NORMAL_SPEED

            self.accumulated_delta += delta_time * self.speed
            if self.state == State.DEFEATED_TRAVELING_TO_HOUSE or self.state == State.DEFEATED_ENTERING_HOUSE:
                self.accumulated_delta *= 1.75
            move_speed = 1.0
            while self.accumulated_delta >= move_speed:
                self.accumulated_delta -= move_speed
                if self.state == State.IN_HOUSE:
                    self.in_house_move(move_speed)
                elif self.state == State.LEAVE_HOUSE:
                    self.leave_house_move(move_speed)
                elif self.state == State.FRIGHTENED:
                    self.frightened_move(move_speed)
                elif self.state == State.DEFEATED_ENTERING_HOUSE:
                    self.defeated_entering_house_move(move_speed)
                else:
                    self.move(move_speed)

            self.update_animations()

            if abs(self.velocity.x) == 0 and abs(self.velocity.y) == 0:
                self.accumulated_delta = 0.0
        elif (
            global_obj.game_paused
            and global_obj.game_state == GameState.PACMAN_LOSE_LIFE
        ):
            pass
        else:
            game.stop_animation(entity_id=self.entity_id)

    def frightened_move(self, speed):
        if self.previous_state == State.IN_HOUSE:
            self.in_house_move(speed)
        elif self.previous_state == State.LEAVE_HOUSE:
            self.leave_house_move(speed)
        elif self.previous_state == State.SCATTER or self.previous_state == State.CHASE:
            self.move(speed * 0.6)

    def defeated_entering_house_move(self, speed):
        self.velocity.x = 0
        self.velocity.y = 0

        # Check if can enter regular states
        end_position_x = global_obj.level_grid.board_position.x + (13 * 16) + 4
        end_position_y = global_obj.level_grid.board_position.y + (14 * 16) - 2
        if (int(self.position.x), int(self.position.y)) == (
            end_position_x,
            end_position_y,
        ):
            self.state = State.LEAVE_HOUSE
        else:
            self.direction = Direction.down
            self.velocity.y += speed
            self.position.y += self.velocity.y

    def move(self, speed):
        self.velocity.x = 0
        self.velocity.y = 0
        direction_offset_x = 0
        direction_offset_y = 0
        if self.direction == Direction.left:
            self.velocity.x -= speed
            direction_offset_x = -1
        elif self.direction == Direction.right:
            self.velocity.x += speed
            direction_offset_x = 1
        elif self.direction == Direction.up:
            self.velocity.y -= speed
            direction_offset_y = -1
        elif self.direction == Direction.down:
            self.velocity.y += speed
            direction_offset_y = 1
        # Movement
        grid_space_x, grid_space_y = self.get_level_grid_position(
            self.position.x, self.position.y
        )
        grid_space = global_obj.level_grid.get(grid_space_x, grid_space_y)
        if grid_space == LevelGrid.VALID_SPACE:
            if global_obj.level_grid.has_loop_around_space(grid_space_x, grid_space_y):
                # Left\
                if grid_space_x < 0:
                    self.position.x = (
                        global_obj.level_grid.board_position.x
                        + (LevelGrid.RIGHT_LOOP_SPACE[0] * 16)
                        - 16
                    )
                # Right
                else:
                    self.position.x = (
                        global_obj.level_grid.board_position.x
                        + (LevelGrid.LEFT_LOOP_SPACE[0] * 16)
                        + 16
                    )
            elif (
                self.state == State.DEFEATED_TRAVELING_TO_HOUSE
                and global_obj.level_grid.has_ghost_house_turn_space(
                    int(self.position.x), int(self.position.y)
                )
            ):
                # TODO: add proper ghost house re-entry logic
                self.state = State.DEFEATED_ENTERING_HOUSE
            else:
                # Update position
                self.position.x += self.velocity.x
                self.position.y += self.velocity.y
                # print(f"ghost valid grid_space = {(grid_space_x, grid_space_y)}, real_position = {(self.position.tuple())}")
                if global_obj.level_grid.has_turn_space(
                    int(self.position.x), int(self.position.y)
                ):
                    left_space = global_obj.level_grid.get(
                        grid_space_x - 1, grid_space_y
                    )
                    right_space = global_obj.level_grid.get(
                        grid_space_x + 1, grid_space_y
                    )
                    up_space = global_obj.level_grid.get(grid_space_x, grid_space_y - 1)
                    down_space = global_obj.level_grid.get(
                        grid_space_x, grid_space_y + 1
                    )
                    # Space selection logic (will need to be ghost independent)
                    valid_spaces = []
                    if (
                        left_space == LevelGrid.VALID_SPACE
                        and self.direction != Direction.right
                    ):
                        valid_spaces.append(
                            (Direction.left, grid_space_x - 1, grid_space_y)
                        )
                    if (
                        right_space == LevelGrid.VALID_SPACE
                        and self.direction != Direction.left
                    ):
                        valid_spaces.append(
                            (Direction.right, grid_space_x + 1, grid_space_y)
                        )
                    if (
                        up_space == LevelGrid.VALID_SPACE
                        and self.direction != Direction.down
                    ):
                        valid_spaces.append(
                            (Direction.up, grid_space_x, grid_space_y - 1)
                        )
                    if (
                        down_space == LevelGrid.VALID_SPACE
                        and self.direction != Direction.up
                    ):
                        valid_spaces.append(
                            (Direction.down, grid_space_x, grid_space_y + 1)
                        )

                    self.target_tile = self.determine_target_tile()

                    # Determine shortest path
                    if valid_spaces and self.target_tile:
                        closest_space = None
                        closest_distance = 1000
                        for space in valid_spaces:
                            _dir, _x, _y = space
                            target_tile_distance = Vector2.distance(
                                self.target_tile, (_x, _y)
                            )
                            if target_tile_distance < closest_distance:
                                closest_distance = target_tile_distance
                                closest_space = space
                        if closest_space:
                            self.direction = closest_space[0]
        else:
            # print(f"invalid grid_space = {(grid_space_x, grid_space_y)}, real_position = {self.position.tuple()}")
            self.velocity.x = 0
            self.velocity.y = 0

    def in_house_move(self, speed):
        if (
            global_obj.player_stats.pelletes < self.exit_pellete_counter
            or self.state == State.FRIGHTENED
        ):
            self.velocity.x = 0
            self.velocity.y = 0
            if self.direction == Direction.up:
                self.velocity.y -= speed * 0.6
            elif self.direction == Direction.down:
                self.velocity.y += speed * 0.6
            # Update position
            self.position.x += self.velocity.x
            self.position.y += self.velocity.y

            center_y = global_obj.level_grid.board_position.y + (14 * 16) - 8
            if abs(self.position.y - center_y) > 8:
                if self.direction == Direction.up:
                    self.direction = Direction.down
                else:
                    self.direction = Direction.up
        else:
            self.state = State.LEAVE_HOUSE

    def leave_house_move(self, speed):
        self.velocity.x = 0
        self.velocity.y = 0
        center_x = global_obj.level_grid.board_position.x + (13 * 16) + 4
        if int(self.position.x) != center_x:
            if int(self.position.x) > center_x:
                self.velocity.x -= speed
                self.direction = Direction.left
            elif int(self.position.x) < center_x:
                self.velocity.x += speed
                self.direction = Direction.right
        else:
            self.velocity.y -= speed
            self.direction = Direction.up

        # Update position
        self.position.x += self.velocity.x
        self.position.y += self.velocity.y

        # Check if can enter regular states
        end_position_x = global_obj.level_grid.board_position.x + (13 * 16) + 4
        end_position_y = global_obj.level_grid.board_position.y + (11 * 16) - 10
        if (int(self.position.x), int(self.position.y)) == (
            end_position_x,
            end_position_y,
        ):
            self.direction = Direction.left
            if self.state == State.FRIGHTENED:
                self.previous_state = State.SCATTER
            elif self.previous_state == State.DEFEATED_ENTERING_HOUSE:
                # TODO: switch to proper previous state
                if self.chase_state_counter <= 3:
                    self.state = random.choice([State.SCATTER, State.CHASE])
                else:
                    self.state = State.CHASE
            else:
                self.state = State.SCATTER

    # Ghost Independent - must return target tile tuple (make break up into smaller functions)
    def determine_target_tile(self):
        if self.state == State.SCATTER:
            return (self.home_tile.x, self.home_tile.y)
        elif self.state == State.FRIGHTENED:
            return (self.home_tile.x, self.home_tile.y)
        elif self.state == State.DEFEATED_TRAVELING_TO_HOUSE:
            return (self.ghost_house_tile.x, self.ghost_house_tile.y)
        else:
            return (0, 0)

    def update_animations(self):
        # Update animations
        if self.state == State.FRIGHTENED:
            if self.frightened_flash:
                game.play_animation(
                    entity_id=self.entity_id, animation_name="frightened_flash"
                )
            else:
                game.play_animation(
                    entity_id=self.entity_id, animation_name="frightened"
                )
        else:
            animation_prefix = ""
            if self.state == State.DEFEATED_TRAVELING_TO_HOUSE or self.state == State.DEFEATED_ENTERING_HOUSE:
                animation_prefix = "blank_"
            if self.velocity.y > 0:
                game.play_animation(
                    entity_id=self.entity_id, animation_name=f"{animation_prefix}down"
                )
            elif self.velocity.y < 0:
                game.play_animation(
                    entity_id=self.entity_id, animation_name=f"{animation_prefix}up"
                )
            elif self.velocity.x > 0:
                game.play_animation(
                    entity_id=self.entity_id, animation_name=f"{animation_prefix}right"
                )
            elif self.velocity.x < 0:
                game.play_animation(
                    entity_id=self.entity_id, animation_name=f"{animation_prefix}left"
                )
            else:
                game.stop_animation(entity_id=self.entity_id)

    def eaten(self, score):
        previous_state_ref = self.previous_state
        self.state = State.DEFEATED_TRAVELING_TO_HOUSE
        self.previous_state = previous_state_ref
        game.stop_timer(timer_id=self.frightened_timer_id)
        game.stop_timer(timer_id=self.frightened_flash_timer_id)
        game.play_animation(
            entity_id=self.entity_id, animation_name=f"score_gain_{score}"
        )

    def get_level_grid_position(self, position_x, position_y):
        grid_space_x = int(
            (
                position_x
                + self.velocity.x
                + self.offset.x
                - global_obj.level_grid.board_position.x
            )
            / 16
        )
        grid_space_y = int(
            (
                position_y
                + self.velocity.y
                + self.offset.y
                - global_obj.level_grid.board_position.y
            )
            / 16
        )
        return grid_space_x, grid_space_y

    def _on_chase_timer_timeout(self):
        self.state = State.SCATTER

    def _on_scatter_timer_timeout(self):
        self.state = State.CHASE

    def _on_frightened_timer_timeout(self):
        self.frightened_flash = True
        game.start_timer(timer_id=self.frightened_flash_timer_id)

    def _on_frightened_flash_timer_timeout(self):
        if self.state == State.FRIGHTENED:
            self.state = self.previous_state

    def _on_pacman_power_pellete_eaten(self):
        if self.state == State.FRIGHTENED:
            self.frightened_flash = False
            game.stop_timer(timer_id=self.frightened_timer_id)
            game.stop_timer(timer_id=self.frightened_flash_timer_id)
            game.start_timer(timer_id=self.frightened_timer_id)
        elif self.state != State.DEFEATED_TRAVELING_TO_HOUSE:
            self.state = State.FRIGHTENED
