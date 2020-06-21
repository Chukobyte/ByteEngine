#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "./models/Constants.h"
#include "./managers/ConfigManager.h"
#include "./managers/SceneManager.h"
#include "./managers/EntityManager.h"
#include "./managers/InputManager.h"
#include "./managers/AssetManager.h"
#include "./managers/AnimationFrameManager.h"
#include "./managers/TimerManager.h"
#include "./managers/SignalManager.h"
#include "./managers/PythonCacheManager.h"
#include "./EntityFactory.h"
#include "./Camera.h"
#include "./Engine.h"

class Engine;

class Game {
	private:
		bool running;
		ConfigManager configManager;
		int ticksLastFrame;
		SDL_Window *window;
	public:
		static SceneManager* sceneManager;
		static EntityManager* entityManager;
		static EntityFactory* entityFactory;
		static InputManager* inputManager;
		static AssetManager* assetManager;
		static AnimationFrameManager* animationFrameManager;
		static TimerManager* timerManager;
		static SignalManager* signalManager;
		static PythonCacheManager* pythonCacheManager;
		static Engine* engine;

		static SDL_Renderer* renderer;
		static SDL_Event event;
		static Camera camera;

		Game();
    	~Game();
		void Initialize();
		bool InitializeSDL();
		bool LoadConfig();
		void ProcessInput();
		void Update();
		void Render();
		bool IsRunning() const;
		void Destroy();
		void LoadScene(std::string sceneFilePath);
		void CleanUpScene();
		void SwitchSceneTo(std::string sceneFilePath);
		void Exit();
};

#endif