# fruit.py

import game

from sample_projects.pacman.scripts.script_class import ScriptClass


class Fruit(ScriptClass):
    def __create__(self):
        self.life_timer_id = f"{self.entity_id}_life_timer"
        self.collected = False

        game.create_timer(
            timer_id=self.life_timer_id, time=10.0
        )
        game.subscribe_to_signal(
            source_id=self.life_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_fruit_timer_timeout",
        )
        game.start_timer(self.life_timer_id, time=10.0)

    def eaten(self):
        game.play_animation(entity_id=self.entity_id, animation_name="score")
        game.stop_timer(timer_id=self.life_timer_id)
        game.start_timer(timer_id=self.life_timer_id, time=1.0)
        self.collected = True
        game.play_sound(sound_id="pacman-eat-fruit")

    def _on_fruit_timer_timeout(self):
        game.delete_entity(entity_id=self.entity_id)
