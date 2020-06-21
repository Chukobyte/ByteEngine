# init.py

import game

from sample_projects.pacman.scripts.script_class import ScriptClass
import sample_projects.pacman.scripts.global_obj as global_obj
from sample_projects.pacman.scripts.game_state import GameState


class Init(ScriptClass):
    def __create__(self):
        global_obj.game_state = GameState.INIT
        self.init_timer_id = "init_timer"
        # Start Timer
        game.create_timer(
            timer_id=self.init_timer_id, time=0.1, loops=False, start_on_creation=True
        )
        game.subscribe_to_signal(
            source_id=self.init_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_init_timer_timeout",
        )

    def _on_init_timer_timeout(self):
        game.change_to_scene(
            file_path="./sample_projects/pacman/scenes/transition.json"
        )
