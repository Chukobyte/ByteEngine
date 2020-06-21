from sample_projects.pacman.scripts.level_grid import LevelGrid
from sample_projects.pacman.scripts.stats import Stats
from sample_projects.pacman.scripts.game_state import GameState

game_started = False
game_paused = False
level_grid = LevelGrid(32, 31, board_position=(0, 48))
player_stats = Stats()
game_state = GameState.INIT
