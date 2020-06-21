#include "./core/Game.h"

int main(int argv, char** args) {
	Game* game = new Game();

	game->Initialize();

	while(game->IsRunning()) {
		game->ProcessInput();
		game->Update();
		game->Render();
	}

	game->Destroy();

	return 0;
}
