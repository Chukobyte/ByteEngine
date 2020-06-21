import game

import sample_projects.pacman.scripts.scene_tree as scene_tree
import sample_projects.pacman.scripts.global_obj as global_obj
from sample_projects.pacman.scripts.game_state import GameState


class Stats:
    LIVES_ENTITY_ID = "pacman_lives"
    FRUIT_SCORES = {
        "cherry": 100,
        "strawberry": 300,
        "orange": 500,
        "apple": 700,
        "melon": 1000,
        "galaxy_boss": 2000,
        "bell": 3000,
        "key": 5000,
    }

    def __init__(self):
        self._score = 0
        self._high_score = 0
        self.pelletes = 0
        self._lives = 0
        self._level = 0
        self.extra_life_earned = False

    @property
    def score(self):
        return self._score

    @score.setter
    def score(self, value):
        self._score = value
        game.update_label_entity_text(
            entity_id="player_one_point_value_label", text=f"{self._score}"
        )
        if self._score > self.high_score:
            self.high_score = self._score
        if self._score >= 10000 and not self.extra_life_earned:
            self.lives += 1
            self.extra_life_earned = True
            game.play_sound(sound_id="pacman-extra-life")

    @property
    def high_score(self):
        return self._high_score

    @high_score.setter
    def high_score(self, value):
        self._high_score = value
        game.update_label_entity_text(
            entity_id="high_score_point_value_label", text=f"{self._high_score}"
        )

    @property
    def lives(self):
        return self._lives

    @lives.setter
    def lives(self, value):
        previous_size = self._lives
        self._lives = value
        if self.is_in_valid_playing_state():
            if self._lives >= previous_size:
                for i in range(self._lives):
                    live_index = i
                    entity_index = f"{self.LIVES_ENTITY_ID}_{live_index}"
                    game.create_sprite_entity(
                        entity_id=entity_index,
                        texture_id="pacman-life",
                        position=(
                            36 + (36 * i),
                            global_obj.level_grid.board_position.y + 496
                        ),
                        layer=5,
                        width=16,
                        height=16,
                    )
            elif self._lives < previous_size:
                for i in range(previous_size - self._lives):
                    live_index = i + self._lives
                    entity_index = f"{self.LIVES_ENTITY_ID}_{live_index}"
                    index_id = entity_index
                    game.delete_entity(entity_id=index_id)

    @property
    def level(self):
        return self._level

    # Set once at the beginning of a level
    @level.setter
    def level(self, value):
        self._level = value
        if self._level > 0 and self.is_in_valid_playing_state():
            for index in range(min(self._level, 8)):
                current_level = index + 1
                fruit_entity_id = ""
                if current_level == 1:
                    fruit_entity_id = "cherry"
                elif current_level == 2:
                    fruit_entity_id = "strawberry"
                elif current_level == 3:
                    fruit_entity_id = "orange"
                elif current_level == 4:
                    fruit_entity_id = "apple"
                elif current_level == 5:
                    fruit_entity_id = "melon"
                elif current_level == 6:
                    fruit_entity_id = "galaxy_boss"
                elif current_level == 7:
                    fruit_entity_id = "bell"
                elif current_level == 8:
                    fruit_entity_id = "key"

                if fruit_entity_id:
                    game.create_sprite_entity(
                        entity_id=f"{fruit_entity_id}_level",
                        texture_id=fruit_entity_id,
                        position=(
                            384 - (36 * index),
                            global_obj.level_grid.board_position.y + 496,
                        ),
                        layer=5,
                        width=20,
                        height=16,
                    )

    def get_fruit_score(self, fruit_entity_id):
        return self.FRUIT_SCORES.get(fruit_entity_id, 0)

    def get_level_fruit_entity_id(self):
        if self._level == 1:
            return "cherry"
        elif self._level == 2:
            return "strawberry"
        elif self._level == 3:
            return "orange"
        elif self._level == 4:
            return "apple"
        elif self._level == 5:
            return "melon"
        elif self._level == 6:
            return "galaxy_boss"
        elif self._level == 7:
            return "bell"
        elif self._level >= 8:
            return "key"
        return ""

    def force_update_ui(self):
        self.high_score = self.high_score
        if self.is_in_valid_playing_state():
            self.score = self.score
            self.lives = self.lives

    def is_in_valid_playing_state(self):
        return global_obj.game_state == GameState.BEGIN_GAME or global_obj.game_state == GameState.LEVEL_COMPLETE or global_obj.game_state == GameState.PLAYING or global_obj.game_state == GameState.PACMAN_LOSE_LIFE

    def reset(self):
        self.level = 1
        self.lives = 3
        self.score = 0
        self.extra_life_earned = False
