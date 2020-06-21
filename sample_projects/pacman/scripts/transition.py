# transition.py

import game

from sample_projects.pacman.scripts.script_class import ScriptClass
import sample_projects.pacman.scripts.global_obj as global_obj
from sample_projects.pacman.scripts.game_state import GameState


class Transition(ScriptClass):
    def __create__(self):
        self.transition_timer_id = "transition_timer"
        if global_obj.game_state == GameState.TITLE_SCREEN:
            self.transition_time = 0.2
        else:
            self.transition_time = 0.5
        # Start Timer
        game.create_timer(
            timer_id=self.transition_timer_id,
            time=self.transition_time,
            loops=False,
            start_on_creation=True,
        )
        game.subscribe_to_signal(
            source_id=self.transition_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_transition_timer_timeout",
        )

    def _on_transition_timer_timeout(self):
        # Temp initialize for now
        if global_obj.game_state == GameState.INIT or global_obj.game_state == GameState.GAME_OVER:
            global_obj.game_state = GameState.TITLE_SCREEN
            game.change_to_scene(file_path="./sample_projects/pacman/scenes/title_screen.json")
        elif global_obj.game_state == GameState.TITLE_SCREEN:
            global_obj.game_state = GameState.BEGIN_GAME
            global_obj.player_stats.reset()
            game.change_to_scene(file_path="./sample_projects/pacman/scenes/main.json")
        elif global_obj.game_state == GameState.LEVEL_COMPLETE:
            game.change_to_scene(file_path="./sample_projects/pacman/scenes/main.json")
