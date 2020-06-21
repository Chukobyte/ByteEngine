# title_screen.py

import game

from sample_projects.pacman.scripts.script_class import ScriptClass
import sample_projects.pacman.scripts.global_obj as global_obj
from sample_projects.pacman.scripts.game_state import GameState


class TitleScreen(ScriptClass):
    def __create__(self):
        self.confirm_instruction_visible = True
        self.one_up_visible = True
        self.confirm_instructions_flash_timer_id = "confirm_instructions_flash_timer"
        self.one_up_flash_timer_id = "one_up_flash_timer"

        # Confirm Instruction Flash Timer
        game.create_timer(
            timer_id=self.confirm_instructions_flash_timer_id, time=0.5, loops=True, start_on_creation=True
        )
        game.subscribe_to_signal(
            source_id=self.confirm_instructions_flash_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_confirm_instructions_flash_timer_timeout",
        )

        # 1UP Flash Timer
        game.create_timer(
            timer_id=self.one_up_flash_timer_id, time=0.4, loops=True, start_on_creation=True
        )
        game.subscribe_to_signal(
            source_id=self.one_up_flash_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_one_up_flash_timer_timeout",
        )
        global_obj.player_stats.force_update_ui()

    def __process__(self, delta_time):
        if game.is_action_just_pressed(action="confirm"):
            game.play_sound(sound_id="pacman-chomp")
            game.change_to_scene(
                file_path="./sample_projects/pacman/scenes/transition.json"
            )

        if game.is_action_just_pressed(action="exit_game"):
            game.quit()

    def _on_confirm_instructions_flash_timer_timeout(self):
        self.confirm_instruction_visible = not self.confirm_instruction_visible
        game.set_entity_visibility(
            entity_id="play_instruction", visible=self.confirm_instruction_visible
        )

    def _on_one_up_flash_timer_timeout(self):
        self.one_up_visible = not self.one_up_visible
        game.set_entity_visibility(
            entity_id="player_one_point_label", visible=self.one_up_visible
        )
