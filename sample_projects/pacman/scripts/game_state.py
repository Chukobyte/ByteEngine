from enum import Enum


class GameState(Enum):
    INIT = 0
    TITLE_SCREEN = 1
    BEGIN_GAME = 2
    PLAYING = 3
    PACMAN_LOSE_LIFE = 4
    LEVEL_COMPLETE = 5
    GAME_OVER = 6
