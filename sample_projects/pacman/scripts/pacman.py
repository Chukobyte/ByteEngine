# pacman.py

import game
import sample_projects.pacman.scripts.scene_tree as scene_tree

from sample_projects.pacman.scripts.script_class import ScriptClass
from sample_projects.pacman.scripts.vector2 import Vector2, Direction
from sample_projects.pacman.scripts.level_grid import LevelGrid
import sample_projects.pacman.scripts.global_obj as global_obj
from sample_projects.pacman.scripts.ghost import State
from sample_projects.pacman.scripts.game_state import GameState


class Pacman(ScriptClass):
    def __create__(self):
        self.speed = 100
        self.direction = Direction.left
        self.direction_to_turn = self.direction
        self.chomp_timer_id = "chomp-sound-timer"
        self.eaten_freeze_timer_id = "eaten-freeze-timer"
        self.turn_speed_up_timer_id = "turn_speed_up_timer"
        self.accumulated_delta = 0.0
        self.offset = Vector2(12, 12)
        self.on_stop_animation_frame = True
        self.power_pellete_eaten_signal_id = "power_pellete_eaten"
        self.level_cleared_signal_id = "level_cleared"
        self.lose_life_signal_id = "lose_life"
        self.ghost_eaten_score = 0
        self._pellete_counter = len(global_obj.level_grid.get_pellete_spaces()) + len(
            global_obj.level_grid.get_power_pellete_spaces()
        )

        game.create_timer(timer_id=self.chomp_timer_id, time=0.55)

        game.create_timer(timer_id=self.eaten_freeze_timer_id, time=1.0)
        game.subscribe_to_signal(
            source_id=self.eaten_freeze_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_eaten_freeze_timer_timeout",
        )

        game.create_timer(timer_id=self.turn_speed_up_timer_id, time=0.1)

        # Animation Finished
        game.subscribe_to_signal(
            source_id=f"{self.entity_id}_animation",
            signal_id="animation_finished",
            subscriber_entity_id=self.entity_id,
            function_name="_on_pacman_animation_finished",
        )

        game.create_signal(
            entity_id=self.entity_id, signal_id=self.power_pellete_eaten_signal_id
        )
        game.create_signal(
            entity_id=self.entity_id, signal_id=self.level_cleared_signal_id
        )
        game.create_signal(entity_id=self.entity_id, signal_id=self.lose_life_signal_id)

    @property
    def pellete_counter(self):
        return self._pellete_counter

    @pellete_counter.setter
    def pellete_counter(self, value):
        self._pellete_counter = value
        # 244 - 70 = 174, 244 - 170 = 74
        if self._pellete_counter == 174 or self._pellete_counter == 74:
            # Spawn fruit
            fruit_entity_id = global_obj.player_stats.get_level_fruit_entity_id()
            if fruit_entity_id:
                game.create_sprite_entity(
                    entity_id=fruit_entity_id,
                    texture_id=fruit_entity_id,
                    position=(global_obj.level_grid.board_position.x + (13 * 16) + 4, global_obj.level_grid.board_position.y + (17 * 16) - 8),
                    layer=5,
                    width=20,
                    height=16,
                    collider_tag="fruit",
                    script_class=("sample_projects.pacman.scripts.fruit", "Fruit")
                )
        elif self._pellete_counter <= 0:
            game.play_animation(entity_id=self.entity_id, animation_name="idle")
            game.emit_signal(
                entity_id=self.entity_id, signal_id=self.level_cleared_signal_id
            )

    def reset(self):
        self.speed = 100
        self.direction = Direction.left
        self.direction_to_turn = self.direction
        self.accumulated_delta = 0.0
        self.on_stop_animation_frame = True
        self.ghost_eaten_score = 0
        game.play_animation(entity_id=self.entity_id, animation_name="idle")

    def __process__(self, delta_time):
        if global_obj.game_started and not global_obj.game_paused:
            collided_entity_id, collider_tag = game.check_entity_collision(
                entity_id=self.entity_id, offset_position=(0, 0)
            )
            if collided_entity_id:
                if collider_tag == "pellete":
                    game.delete_entity(entity_id=collided_entity_id)
                    global_obj.player_stats.score += 10
                    global_obj.player_stats.pelletes += 1
                    self.pellete_counter -= 1
                    game.play_sound(sound_id="pacman-chomp")
                elif collider_tag == "power_pellete":
                    game.delete_entity(entity_id=collided_entity_id)
                    global_obj.player_stats.score += 50
                    global_obj.player_stats.pelletes += 1
                    self.pellete_counter -= 1
                    self.ghost_eaten_score = 0
                    game.emit_signal(
                        entity_id=self.entity_id,
                        signal_id=self.power_pellete_eaten_signal_id,
                    )
                elif collider_tag == "enemy":
                    collided_ghost = scene_tree.get_entity(collided_entity_id)
                    if collided_ghost.state == State.FRIGHTENED:
                        if self.ghost_eaten_score == 0:
                            self.ghost_eaten_score = 200
                        else:
                            self.ghost_eaten_score *= 2
                        global_obj.player_stats.score += self.ghost_eaten_score
                        collided_ghost.eaten(self.ghost_eaten_score)
                        game.start_timer(timer_id=self.eaten_freeze_timer_id)
                        global_obj.game_paused = True
                        game.play_sound(sound_id="pacman-eat-ghost")
                    elif (
                        collided_ghost.state == State.CHASE
                        or collided_ghost.state == State.SCATTER
                    ):
                        game.stop_animation(entity_id=self.entity_id)
                        game.emit_signal(
                            entity_id=self.entity_id, signal_id=self.lose_life_signal_id
                        )
                elif collider_tag == "fruit":
                    fruit_entity = scene_tree.get_entity(collided_entity_id)
                    if not fruit_entity.collected:
                        global_obj.player_stats.score += global_obj.player_stats.get_fruit_score(
                            collided_entity_id
                        )
                        fruit_entity.eaten()

            # Input - determines direction pacman wants to turn
            if game.is_action_pressed(action="left"):
                self.direction_to_turn = Direction.left
            elif game.is_action_pressed(action="right"):
                self.direction_to_turn = Direction.right
            elif game.is_action_pressed(action="up"):
                self.direction_to_turn = Direction.up
            elif game.is_action_pressed(action="down"):
                self.direction_to_turn = Direction.down

            # Determine speed
            if game.has_timer_stopped(timer_id=self.turn_speed_up_timer_id):
                self.speed = 100
            else:
                self.speed = 150
            self.accumulated_delta += delta_time * self.speed
            while self.accumulated_delta >= 1.0:
                self.accumulated_delta -= 1.0
                self.move(self.velocity, 1.0)

            self.update_animations()

            # Play movement sound
            if abs(self.velocity.x) > 0 or abs(self.velocity.y) > 0:
                if game.has_timer_stopped(timer_id=self.chomp_timer_id):
                    pass
                    # game.start_timer(timer_id=self.chomp_timer_id)
            else:
                self.accumulated_delta = 0.0
        elif (
            global_obj.game_paused
            and global_obj.game_state == GameState.PACMAN_LOSE_LIFE
        ):
            pass
        else:
            game.stop_animation(entity_id=self.entity_id)

        if game.is_action_just_pressed(action="exit_game"):
            game.quit()

    def move(self, velocity, speed):
        # Determine if direction should change
        if self.direction_to_turn != self.direction:
            can_turn = (
                (
                    self.direction_to_turn == Direction.left
                    and self.direction == Direction.right
                )
                or (
                    self.direction_to_turn == Direction.right
                    and self.direction == Direction.left
                )
                or (
                    self.direction_to_turn == Direction.up
                    and self.direction == Direction.down
                )
                or (
                    self.direction_to_turn == Direction.down
                    and self.direction == Direction.up
                )
            )
            if can_turn or global_obj.level_grid.has_turn_space(
                int(self.position.x), int(self.position.y)
            ):
                grid_space_x = int(
                    (
                        self.position.x
                        + self.offset.x
                        - global_obj.level_grid.board_position.x
                    )
                    / 16
                )
                grid_space_y = int(
                    (
                        self.position.y
                        + self.offset.y
                        - global_obj.level_grid.board_position.y
                    )
                    / 16
                )
                grid_space = global_obj.level_grid.get(grid_space_x, grid_space_y)
                turn_space_offset_x = 0
                turn_space_offset_y = 0
                if self.direction_to_turn == Direction.left:
                    turn_space_offset_x = -1
                elif self.direction_to_turn == Direction.right:
                    turn_space_offset_x = 1
                elif self.direction_to_turn == Direction.up:
                    turn_space_offset_y = -1
                elif self.direction_to_turn == Direction.down:
                    turn_space_offset_y = 1
                turn_space = global_obj.level_grid.get(
                    grid_space_x + turn_space_offset_x,
                    grid_space_y + turn_space_offset_y,
                )

                if (
                    grid_space == LevelGrid.VALID_SPACE
                    and turn_space == LevelGrid.VALID_SPACE
                ):
                    self.direction = self.direction_to_turn
                    game.start_timer(timer_id=self.turn_speed_up_timer_id)

        # Other Movement
        # Determine velocity from direction and speed
        velocity.x = 0
        velocity.y = 0
        direction_offset_x = 0
        direction_offset_y = 0
        if self.direction == Direction.left:
            velocity.x -= speed
            direction_offset_x = -1
        elif self.direction == Direction.right:
            velocity.x += speed
            direction_offset_x = 1
        elif self.direction == Direction.up:
            velocity.y -= speed
            direction_offset_y = -1
        elif self.direction == Direction.down:
            velocity.y += speed
            direction_offset_y = 1
        # Movement
        grid_space_x = int(
            (
                self.position.x
                + velocity.x
                + self.offset.x
                - global_obj.level_grid.board_position.x
            )
            / 16
        )
        grid_space_y = int(
            (
                self.position.y
                + velocity.y
                + self.offset.y
                - global_obj.level_grid.board_position.y
            )
            / 16
        )
        grid_space = global_obj.level_grid.get(grid_space_x, grid_space_y)
        at_turn_space_wall = (
            global_obj.level_grid.has_turn_space(
                int(self.position.x), int(self.position.y)
            )
            and global_obj.level_grid.get(
                grid_space_x + direction_offset_x, grid_space_y + direction_offset_y
            )
            == LevelGrid.INVALID_SPACE
        )
        if grid_space == LevelGrid.VALID_SPACE and not at_turn_space_wall:
            if not global_obj.level_grid.has_loop_around_space(
                grid_space_x, grid_space_y
            ):
                # Update position
                self.position.x += velocity.x
                self.position.y += velocity.y
                self.on_stop_animation_frame = False
            else:
                # Left
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
        else:
            velocity.x = 0
            velocity.y = 0

    def update_animations(self):
        # Update animations
        if self.velocity.y > 0:
            game.play_animation(entity_id=self.entity_id, animation_name="down")
        elif self.velocity.y < 0:
            game.play_animation(entity_id=self.entity_id, animation_name="up")
        elif self.velocity.x > 0:
            game.play_animation(entity_id=self.entity_id, animation_name="right")
        elif self.velocity.x < 0:
            game.play_animation(entity_id=self.entity_id, animation_name="left")
        else:
            game.stop_animation(entity_id=self.entity_id)
            if not self.on_stop_animation_frame:
                game.set_animation_frame(entity_id=self.entity_id, frame=1)
                self.on_stop_animation_frame = True

    def lose_life(self):
        game.play_animation(entity_id=self.entity_id, animation_name="lose_life")

    def _on_eaten_freeze_timer_timeout(self):
        global_obj.game_paused = False
        game.play_animation(entity_id=self.entity_id, animation_name="left")

    def _on_pacman_animation_finished(self):
        if (
            global_obj.game_paused
            and global_obj.game_state == GameState.PACMAN_LOSE_LIFE
        ):
            game.stop_animation(entity_id=self.entity_id)
            game.set_entity_visibility(entity_id=self.entity_id, visible=False)
