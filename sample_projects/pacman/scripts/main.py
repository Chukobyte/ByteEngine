# main.py

import game
import sample_projects.pacman.scripts.scene_tree as scene_tree

from sample_projects.pacman.scripts.script_class import ScriptClass
from sample_projects.pacman.scripts.level_grid import LevelGrid
import sample_projects.pacman.scripts.global_obj as global_obj
from sample_projects.pacman.scripts.game_state import GameState


class Main(ScriptClass):
    def __create__(self):
        self.power_pellete_ids = []
        self.power_pelletes_visible = True
        self.one_up_visible = True
        self.start_timer_id = "start_timer"
        self.level_reset_timer_id = "level_reset_timer"
        self.level_cleared_timer_id = "level_cleared_timer"
        self.level_cleared_transtion_timer_id = "level_cleared_transtion_timer"
        power_pellete_flash_timer_id = "power_pellete_timer"
        self.pacman_lose_life_start_timer_id = "pacman_lose_life_start_timer"
        self.ghost_frightened_timer_id = "ghost_frightened_timer"
        self.show_entities_timer_id = "show_entities_timer"

        # Start Timer
        game.create_timer(
            timer_id=self.start_timer_id, time=4.2, loops=False, start_on_creation=False
        )
        game.subscribe_to_signal(
            source_id=self.start_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_start_timer_timeout",
        )

        # Level Reset Timer
        game.create_timer(
            timer_id=self.level_reset_timer_id,
            time=2.0,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.level_reset_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_level_reset_timer_timeout",
        )

        # Level Cleared Timer
        game.create_timer(
            timer_id=self.level_cleared_timer_id,
            time=1.5,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.level_cleared_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_level_cleared_timer_timeout",
        )

        # Level Cleared Transtion Timer
        game.create_timer(
            timer_id=self.level_cleared_transtion_timer_id,
            time=2.0,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.level_cleared_transtion_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_level_cleared_transition_timer_timeout",
        )

        # Pellete Flash Timer
        game.create_timer(
            timer_id=power_pellete_flash_timer_id,
            time=0.2,
            loops=True,
            start_on_creation=True,
        )
        game.subscribe_to_signal(
            source_id=power_pellete_flash_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_power_pellete_flash_timer_timeout",
        )

        # Pacman Lose Life Start Timer
        game.create_timer(
            timer_id=self.pacman_lose_life_start_timer_id,
            time=1.0,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.pacman_lose_life_start_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_pacman_lose_life_start_timer_timeout",
        )

        # 1UP Flash Timer
        one_up_flash_timer_id = "one_up_flash_timer"
        game.create_timer(
            timer_id=one_up_flash_timer_id, time=0.4, loops=True, start_on_creation=True
        )
        game.subscribe_to_signal(
            source_id=one_up_flash_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_one_up_flash_timer_timeout",
        )

        # Ghost Frightened Timer
        game.create_timer(
            timer_id=self.ghost_frightened_timer_id,
            time=7.0,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.ghost_frightened_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_ghost_frightened_timer_timeout",
        )

        # Show Entities Timer
        game.create_timer(
            timer_id=self.show_entities_timer_id,
            time=2.0,
            loops=False,
            start_on_creation=False,
        )
        game.subscribe_to_signal(
            source_id=self.show_entities_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_show_entities_timer_timeout",
        )

        # Lose Life Signal
        pacman_entity = scene_tree.get_entity("pacman")
        game.subscribe_to_signal(
            source_id=pacman_entity.entity_id,
            signal_id="lose_life",
            subscriber_entity_id=self.entity_id,
            function_name="_on_pacman_lose_life",
        )
        # Power Pellet
        game.subscribe_to_signal(
            source_id=pacman_entity.entity_id,
            signal_id="power_pellete_eaten",
            subscriber_entity_id=self.entity_id,
            function_name="_on_pacman_power_pellete_eaten",
        )
        # Level Cleared Signal
        game.subscribe_to_signal(
            source_id=pacman_entity.entity_id,
            signal_id="level_cleared",
            subscriber_entity_id=self.entity_id,
            function_name="_on_pacman_level_cleared",
        )

        self.create_level_pellets()

        self.set_all_entities_visibility(False)

        self.reset_entity_positions()

        global_obj.player_stats.force_update_ui()

        if global_obj.game_state == GameState.BEGIN_GAME:
            global_obj.player_stats.force_update_ui()
            global_obj.game_state = GameState.PLAYING
            game.start_timer(timer_id=self.start_timer_id, time=4.2)
            game.start_timer(timer_id=self.show_entities_timer_id, time=2.0)
            game.play_sound(sound_id="pacman-begin")
            global_obj.player_stats.pelletes = 0
        elif global_obj.game_state == GameState.LEVEL_COMPLETE:
            global_obj.player_stats.level += 1
            global_obj.player_stats.force_update_ui()
            global_obj.game_state = GameState.PLAYING
            game.start_timer(timer_id=self.start_timer_id, time=1.0)
            self._on_show_entities_timer_timeout()
            global_obj.player_stats.pelletes = 0

    def reset_entity_positions(self):
        game.update_entity_position(
            entity_id=global_obj.level_grid.ENTITY_ID,
            position=(
                global_obj.level_grid.board_position.x,
                global_obj.level_grid.board_position.y,
            ),
        )

        pacman_entity = scene_tree.get_entity("pacman")
        if pacman_entity:
            pacman_position_x = global_obj.level_grid.board_position.x + (13 * 16) + 4
            pacman_position_y = global_obj.level_grid.board_position.y + (23 * 16) - 8
            pacman_entity.position.x = pacman_position_x
            pacman_entity.position.y = pacman_position_y

        blinky_entity = scene_tree.get_entity("blinky")
        if blinky_entity:
            blinky_position_x = global_obj.level_grid.board_position.x + (13 * 16) + 4
            blinky_position_y = global_obj.level_grid.board_position.y + (11 * 16) - 10
            blinky_entity.position.x = blinky_position_x
            blinky_entity.position.y = blinky_position_y

        pinky_entity = scene_tree.get_entity("pinky")
        if pinky_entity:
            pinky_position_x = global_obj.level_grid.board_position.x + (13 * 16) + 4
            pinky_position_y = global_obj.level_grid.board_position.y + (14 * 16) - 8
            pinky_entity.position.x = pinky_position_x
            pinky_entity.position.y = pinky_position_y

        inky_entity = scene_tree.get_entity("inky")
        if inky_entity:
            inky_position_x = global_obj.level_grid.board_position.x + (11 * 16) + 4
            inky_position_y = global_obj.level_grid.board_position.y + (14 * 16) - 8
            inky_entity.position.x = inky_position_x
            inky_entity.position.y = inky_position_y

        clyde_entity = scene_tree.get_entity("clyde")
        if clyde_entity:
            clyde_position_x = global_obj.level_grid.board_position.x + (15 * 16) + 4
            clyde_position_y = global_obj.level_grid.board_position.y + (14 * 16) - 8
            clyde_entity.position.x = clyde_position_x
            clyde_entity.position.y = clyde_position_y

    def create_level_pellets(self):
        index = 0
        offset_x = 10
        offset_y = 6
        for x, y in global_obj.level_grid.get_pellete_spaces():
            position_x = (x * 16) + global_obj.level_grid.board_position.x + offset_x
            position_y = (y * 16) + global_obj.level_grid.board_position.y + offset_y
            game.create_sprite_entity(
                entity_id=f"pellete-{index}",
                texture_id="pellete",
                position=(position_x, position_y),
                layer=2,
                width=2,
                height=2,
                clickable=False,
                fixed=False,
                collider_tag="pellete",
            )
            index += 1

        index = 0
        offset_x = 4
        offset_y = 0
        for x, y in global_obj.level_grid.get_power_pellete_spaces():
            position_x = (x * 16) + global_obj.level_grid.board_position.x + offset_x
            position_y = (y * 16) + global_obj.level_grid.board_position.y + offset_y
            power_pellete_entity_id = f"power_pellete-{index}"
            game.create_sprite_entity(
                entity_id=power_pellete_entity_id,
                texture_id="power_pellete",
                position=(position_x, position_y),
                layer=2,
                width=8,
                height=8,
                clickable=False,
                fixed=False,
                collider_tag="power_pellete",
            )
            self.power_pellete_ids.append(power_pellete_entity_id)
            game.set_entity_visibility(entity_id=power_pellete_entity_id, visible=False)
            index += 1

    def level_reset(self):
        pacman_entity = scene_tree.get_entity("pacman")
        pacman_entity.reset()

        for ghost_entity_id in ["blinky", "pinky", "inky", "clyde"]:
            ghost_entity = scene_tree.get_entity(ghost_entity_id)
            ghost_entity.reset()

    def set_all_entities_visibility(self, is_visible):
        for current_entity_id in ["pacman", "blinky", "pinky", "inky", "clyde"]:
            game.set_entity_visibility(
                entity_id=current_entity_id, visible=is_visible
            )

    def _on_start_timer_timeout(self):
        global_obj.player_stats.lives -= 1
        global_obj.game_started = True
        global_obj.game_paused = False
        global_obj.game_state = GameState.PLAYING
        game.delete_entity(entity_id="game_status_label")
        game.play_music(music_id="siren1")

    def _on_level_reset_timer_timeout(self):
        if global_obj.player_stats.lives > 0:
            self.reset_entity_positions()
            game.start_timer(timer_id=self.show_entities_timer_id, time=0.1)
            self.level_reset()
            game.start_timer(timer_id=self.start_timer_id, time=0.5)
        else:
            global_obj.game_state = GameState.GAME_OVER
            game.change_to_scene(
                file_path="./sample_projects/pacman/scenes/transition.json"
            )

    def _on_level_cleared_timer_timeout(self):
        for ghost_entity_id in ["blinky", "pinky", "inky", "clyde"]:
            game.set_entity_visibility(entity_id=ghost_entity_id, visible=False)
        game.play_animation(entity_id="level", animation_name="level_completed")
        game.start_timer(timer_id=self.level_cleared_transtion_timer_id)

    def _on_level_cleared_transition_timer_timeout(self):
        global_obj.player_stats.lives += 1
        game.change_to_scene(
            file_path="./sample_projects/pacman/scenes/transition.json"
        )

    def _on_power_pellete_flash_timer_timeout(self):
        if self.power_pellete_ids:
            copied_pellete_ids = self.power_pellete_ids.copy()
            self.power_pelletes_visible = not self.power_pelletes_visible
            for pp_id in copied_pellete_ids:
                if scene_tree.get_entity(entity_id=pp_id) == None:
                    self.power_pellete_ids.remove(pp_id)
                else:
                    game.set_entity_visibility(
                        entity_id=pp_id, visible=self.power_pelletes_visible
                    )

    def _on_pacman_lose_life_start_timer_timeout(self):
        for ghost_entity_id in ["blinky", "pinky", "inky", "clyde"]:
            game.set_entity_visibility(entity_id=ghost_entity_id, visible=False)
        pacman_entity = scene_tree.get_entity("pacman")
        pacman_entity.lose_life()
        game.start_timer(timer_id=self.level_reset_timer_id)
        game.play_sound(sound_id="pacman-death")
        if global_obj.player_stats.lives <= 0:
            game.create_label_entity(
                entity_id="game_over_label",
                text="GAME  OVER",
                position=(147, 318),
                font_id="pacman-pixel",
                layer=5,
                fixed=False,
                wrap_length=600,
                color=(255, 0, 0)
            )

    def _on_one_up_flash_timer_timeout(self):
        self.one_up_visible = not self.one_up_visible
        game.set_entity_visibility(
            entity_id="player_one_point_label", visible=self.one_up_visible
        )

    def _on_ghost_frightened_timer_timeout(self):
        game.stop_music()
        if global_obj.game_state == GameState.PLAYING:
            game.play_music(music_id="siren1")

    def _on_show_entities_timer_timeout(self):
        game.delete_entity(entity_id="player_one_display_label")
        self.set_all_entities_visibility(True)

    def _on_pacman_power_pellete_eaten(self):
        game.stop_music()
        game.play_music(music_id="power-pellete")
        game.start_timer(timer_id=self.ghost_frightened_timer_id)

    def _on_pacman_lose_life(self):
        global_obj.game_paused = True
        global_obj.game_state = GameState.PACMAN_LOSE_LIFE
        game.start_timer(timer_id=self.pacman_lose_life_start_timer_id)
        game.stop_music()

    def _on_pacman_level_cleared(self):
        global_obj.game_paused = True
        global_obj.game_state = GameState.LEVEL_COMPLETE
        game.start_timer(timer_id=self.level_cleared_timer_id)
        game.stop_music()
