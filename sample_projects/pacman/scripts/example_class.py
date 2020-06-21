# main.py

import game
from sample_projects.test01.scripts.script_class import ScriptClass
import sample_projects.test01.scripts.scene_tree as scene_tree


class ExampleClass(ScriptClass):
    # Call to initialize Script Class (use __create__ for cleaner syntax)
    def __init__(self, entity_id, position_x, position_y):
        super().__init__(entity_id, position_x, position_y)

    def __create__(self):
        # Vars
        self.speed = 100

        # Play Music
        game.stop_music()
        game.pause_music()
        game.resume_music()
        game.play_music(music_id="dd-music")

        # Create Text Label Entity
        text_label_entity_id = "test_text"
        game.create_label_entity(
            entity_id=text_label_entity_id,
            text="Test",
            position=(100, 100),
            font_id="charriot",
            layer=2,
            fixed=False,
            wrap_length=800,
            color=(100, 100, 100),
        )

        # Update Label Text
        game.update_label_entity_text(entity_id=text_label_entity_id, text="New Text!")

        # Create sprite entity
        helicopter_entity_id = "helicopter"
        game.create_sprite_entity(
            entity_id=helicopter_entity_id,
            texture_id="chopper",
            position=(400, 400),
            layer=3,
            width=32,
            height=32,
            clickable=False,
            fixed=False,
            collider_tag="",  # Emtpy as default set to add collider
        )

        # Setup signal for animation finished
        game.subscribe_to_signal(
            source_id=f"{helicopter_entity_id}_animation",
            signal_id="animation_finished",
            subscriber_entity_id=self.entity_id,
            function_name="_on_helicopter_animation_finished",
        )
        # Setup signal for animation frame changed
        game.subscribe_to_signal(
            source_id=f"{helicopter_entity_id}_animation",
            signal_id="frame_changed",
            subscriber_entity_id=self.entity_id,
            function_name="_on_helicopter_frame_changed",
        )

        # Delete sprite entity (after creation)
        delete_entity_id = "delete_entity"
        game.create_sprite_entity(
            entity_id=delete_entity_id,
            texture_id="chopper",
            position=(100, 100),
            layer=3,
            width=32,
            height=32,
            clickable=False,
            fixed=False,
        )
        game.delete_entity(entity_id=delete_entity_id)

        # Create timer
        self.counted_seconds = 0
        self.count_timer_id = "count_timer"
        game.create_timer(
            timer_id=self.count_timer_id, time=1.0, loops=True, start_on_creation=True
        )
        # Stop Timer
        game.stop_timer(self.count_timer_id)
        # Start Timer
        game.start_timer(self.count_timer_id)

        # Connect signal
        game.subscribe_to_signal(
            source_id=self.count_timer_id,
            signal_id="timeout",
            subscriber_entity_id=self.entity_id,
            function_name="_on_test_timer_timeout",
        )

    # Called every frame
    def __process__(self, delta_time):
        # Play Sound
        if game.is_action_just_pressed(action="confirm"):
            game.game_play_sound(sound_id="card-sound")

        # Movement example with keyboard input checks
        if game.is_action_just_pressed(action="left"):
            game.play_animation(entity_id="helicopter", animation_name="left")
            self.position.x -= 1 * self.speed * delta_time
        elif game.is_action_just_pressed(action="right"):
            game.play_animation(entity_id="helicopter", animation_name="right")
            self.position.x += 1 * self.speed * delta_time

        if game.is_action_just_pressed(action="up"):
            game.play_animation(entity_id="helicopter", animation_name="up")
            self.position.y -= 1 * self.speed * delta_time
        elif game.is_action_just_pressed(action="down"):
            game.play_animation(entity_id="helicopter", animation_name="down")
            self.position.y += 1 * self.speed * delta_time

        # Exit Game
        if game.is_action_just_pressed(action="exit_game"):
            game.quit()

    # Example signal called from timer
    def _on_test_timer_timeout(self):
        self.counted_seconds += 1
        print(f"Seconds = {self.counted_seconds}")

    def _on_helicopter_animation_finished(self):
        print("Animation has finished")

    def _on_helicopter_frame_changed(self):
        print("Animation has changed")

    # Called when method is destroyed
    def __destroy__(self):
        pass

    # Final call to clean up object (use __destroy__ instead)
    def __del__(self):
        super().__del__()
