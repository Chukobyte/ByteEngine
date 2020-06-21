#ifndef ENGINE_H
#define ENGINE_H

#include "./Game.h"

class Game;

class Engine {
	public:
		char* pyScriptClassModulePath;
		char* pyScriptClassName;
		std::string sceneSwitchPath;
		bool switchScene = false;
		Game* game;
		Engine(Game* game) : game(game) {}
		~Engine();
		void SetDefaultPythonScriptClass(std::string pyModule, std::string pyClass);
		void SwitchSceneTo(std::string sceneFilePath);
		void Quit();
};

#endif
