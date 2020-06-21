# Byte Game Engine

![Byte Engine Splash Logo](https://github.com/Chukobyte/ByteEngine/blob/master/assets/byte_splash_smaller.png)

A simple 2D game engine made with C++ and SDL2.  Utilizes python 3 for scripting game logic.

### Instructions to Build & Compile Engine

Dependencies:
* SDL2
* SDL2_image
* SDL2_ttf
* SDL2_mixer
* Python 3.7

1. Set environment variable `SDL2_HOME` to path containing SDL2 dependencies.
2. Set environment variable `PYTHON_HOME` to path containing Python 3.7 dependencies.
3. In project directory execute `make`.  Will compile a binary named `byte_engine.exe`.
4. Run `byte_engine.exe` in project root directory containing `byte_config.json`.

### Examples

[Pacman Clone](https://github.com/Chukobyte/PacmanClone)

![Pacman Clone Example](https://github.com/Chukobyte/PacmanClone/blob/master/pacman_example.gif)


### Engine API

To use the engine API import the `game` module into any python script.  Below is a list of engine API functions:

|             Function Name             |                                                 Parameters                                                              |                          Description                                 |
|:-------------------------------------:|:-----------------------------------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------:|
| quit                                  | N/A                                                                                                                     | Quits the game.                                                      |
| create_sprite_entity                  | "entity_id", "texture_id", "position", "layer", "width", "height", "clickable", "fixed", "collider_tag", "script_class" | Creates a sprite entity.                                             |
| create_label_entity                   | "entity_id", "text", "position", "font_id", "layer", "fixed", "wrap_length", "color"                                    | Creates a label entity.                                              |
| update_label_entity_text              | "entity_id", "text"                                                                                                     | Update a label entity's text.                                        |
| update_entity_position                | "entity_id", "position"                                                                                                 | Update an entity's position.                                         |
| set_entity_visibility                 | "entity_id", "visible"                                                                                                  | Set an entity's visibility.                                          |
| delete_entity                         | "entity_id"                                                                                                             | Delete an entity.                                                    |
| play_animation                        | "entity_id", "animation_name"                                                                                           | Plays an entity's animation.                                         |
| stop_animation                        | "entity_id"                                                                                                             | Stops an entity's currently playing animation.                       |
| set_animation_frame                   | "entity_id", "frame"                                                                                                    | Set an entity's currently playing animation frame.                   |
| is_action_pressed                     | "action"                                                                                                                | Check if configured 'action' input is pressed.                       |
| is_action_just_pressed                | "action"                                                                                                                | Check if configured 'action' input has triggered initial press.      |
| is_action_just_released               | "action"                                                                                                                | Check if configured 'action' input has triggered released.           |
| is_mouse_button_left_pressed          | N/A                                                                                                                     | Check if left mouse button is pressed.                               |
| is_mouse_button_left_just_pressed     | N/A                                                                                                                     | Check for left mouse button initial press.                           |
| is_mouse_button_left_just_released    | N/A                                                                                                                     | Check if left mouse button has just released.                        |
| is_mouse_button_right_pressed         | N/A                                                                                                                     | Check if right mouse button is pressed.                              |
| is_mouse_button_right_just_pressed    | N/A                                                                                                                     | Check for right mouse button initial press.                          |
| is_mouse_button_right_just_released   | N/A                                                                                                                     | Check if right mouse button has just released.                       |
| play_music                            | "music_id"                                                                                                              | Plays music based on preconfigured "music_id".                       |
| stop_music                            | N/A                                                                                                                     | Stops currently playing music.                                       |
| pause_music                           | N/A                                                                                                                     | Pauses currently playing music.                                      |
| resume_music                          | N/A                                                                                                                     | Resumes currently paused music.                                      |
| play_sound                            | "sound_id"                                                                                                              | Plays sound based on preconfigured "sound_id".                       |
| set_volume                            | "volume"                                                                                                                | Set volume for sound and music. (Max 128)                            |
| create_timer                          | "timer_id", "time", "loops", "start_on_creation"                                                                        | Creates a timer.                                                     |
| start_timer                           | "timer_id"                                                                                                              | Start a timer.                                                       |
| stop_timer                            | "timer_id"                                                                                                              | Stops a timer.                                                       |
| pause_timer                           | "timer_id"                                                                                                              | Pauses a timer.                                                      |
| resume_timer                          | "timer_id"                                                                                                              | Resumes a timer.                                                     |
| has_timer_stopped                     | "timer_id"                                                                                                              | Check if a timer has stopped.                                        |
| create_signal                         | "entity_id", "signal_id"                                                                                                | Creates a signal.                                                    |
| subscribe_to_signal                   | "source_id", "signal_id", "subscriber_entity_id", "function_name"                                                       | Subscribes to a signal.                                              |
| emit_signal                           | "entity_id", "signal_id"                                                                                                | Emit a signal.                                                       |
| remove_signal                         | "entity_id"                                                                                                             | Removes all signals created by entity.                               |
| change_scene_to                       | "file_path"                                                                                                             | Change to new scene.                                                 |
| check_entity_collision                | "entity_id", "offset_position"                                                                                          | Check for collision on entity.                                       |

To have a python script tied to a game entity, add this python script to your project and inherit from the `ScriptClass` class:

```python
# script_class.py

import math
import game

class Vector2:
    def __init__(self, x, y, **kwargs):
        self.entity_id = kwargs.get("entity_id", None)
        self._x = x
        self._y = y

    @property
    def x(self):
        return self._x

    @x.setter
    def x(self, value):
        if not math.isclose(self._x, value, abs_tol=0.0000001):
            self._x = value
            if self.entity_id is not None:
                game.update_entity_position(
                    entity_id=self.entity_id, position=(int(self.x), int(self.y))
                )

    @property
    def y(self):
        return self._y

    @y.setter
    def y(self, value):
        if not math.isclose(self._y, value, abs_tol=0.0000001):
            self._y = value
            if self.entity_id is not None:
                game.update_entity_position(
                    entity_id=self.entity_id, position=(int(self.x), int(self.y))
                )

    def tuple(self, divisor=1):
        return f"({int(self.x / divisor)}, {int(self.y / divisor)})"

    @staticmethod
    def distance(source_vector, target_vector):
        return math.sqrt(
            ((target_vector.x - source_vector.x) * (target_vector.x - source_vector.x))
            + (
                (target_vector.y - source_vector.y)
                * (target_vector.y - source_vector.y)
            )
        )

    def __str__(self):
        return f"({self.x}, {self.y})"

    def __repr__(self):
        return f"({self.x}, {self.y})"


class SceneTree:
    def __init__(self):
        self.entities = {}

    def add_entity(self, entity_id, entity):
        entities[entity_id] = entity

    def get_entity(self, entity_id):
        if entity_id in entities:
            return entities[entity_id]
        else:
            return None

    def remove_entity(self, entity_id):
        if entities is not None:
            entity = entities[entity_id]
            del entities[entity_id]
            del entity


# MAIN

class ScriptClass:

	scene_tree = SceneTree()

    def __init__(self, entity_id, position_x, position_y):
        self.entity_id = entity_id
        self._position = Vector2(position_x, position_y, entity_id=entity_id)
        self.velocity = Vector2(0.0, 0.0)
        self.scene_tree.add_entity(self.entity_id, self)

    @property
    def position(self):
        return self._position

    @position.setter
    def position(self, value):
        self._position.x += value.x
        self._position.y += value.y

    @property
    def animation_signal_id(self):
        return f"{self.entity_id}_animation"

    def __delete__(self):
        self.scene_tree.remove_entity(self.entity_id)

    def __str__(self):
        return f"(entity_id = {self.entity_id}, position = {self.position})"

    def __repr__(self):
        return f"(entity_id = {self.entity_id}, position = {self.position})"

```
