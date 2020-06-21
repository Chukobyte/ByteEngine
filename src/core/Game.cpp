#include <stdio.h>
#include <Python.h>
#include <iostream>
#include <fstream>

#include "../../lib/json/json.hpp"

#include "./Game.h"
#include "./Scene.h"
#include "./helpers/pyhelper.hpp"
#include "./helpers/Helper.h"
#include "./components/ScriptClassComponent.h"
#include "./components/TransformComponent.h"
#include "./components/KeyboardControlComponent.h"
#include "./components/MouseControlComponent.h"

EntityManager* Game::entityManager = new EntityManager();
SceneManager* Game::sceneManager = new SceneManager(entityManager);
EntityFactory* Game::entityFactory = new EntityFactory(entityManager);
InputManager* Game::inputManager = new InputManager();
AssetManager* Game::assetManager = new AssetManager();
AnimationFrameManager* Game::animationFrameManager = new AnimationFrameManager();
SignalManager* Game::signalManager = new SignalManager(entityManager);
TimerManager* Game::timerManager = new TimerManager(signalManager);
PythonCacheManager* Game::pythonCacheManager = new PythonCacheManager();
Engine* Game::engine;

SDL_Renderer* Game::renderer;
SDL_Event Game::event;
Camera Game::camera;

CPyInstance* pyInstance;

Game::Game() {
	this->running = false;
}

Game::~Game() {}

void Game::Initialize() {
	Py_SetPath(L"./python37.zip");

    engine = new Engine(this);

	if(!LoadConfig()) {
		return;
	}

	if(!InitializeSDL()) {
		return;
	}

	assetManager->ConfigureAssets(configManager.GetAssetConfigs());
	animationFrameManager->SetAnimationFrames(configManager.GetAnimationFramesFromConfig());
	inputManager->ConfigureInput(configManager.GetInputConfigs());

    pyInstance = new CPyInstance();

	LoadScene(configManager.GetInitialScenePath());
	
	Entity& gameEntity = entityFactory->CreateSystemEntity("game");
	gameEntity.AddComponent<TransformComponent>(0, 0, 0, 0, 0, 0, 1.0);
	gameEntity.AddComponent<KeyboardControlComponent>(inputManager);
	gameEntity.AddComponent<MouseControlComponent>(inputManager);

	running = true;
}

bool Game::InitializeSDL() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL.\n");
        return false;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize!\n");
        return false;
    }

    if(TTF_Init() != 0) {
        printf("Error initializing SDL TTF\n");
        return false;
    }

    window = SDL_CreateWindow(
                 configManager.GetGameTitle().c_str(),
                 SDL_WINDOWPOS_CENTERED,
                 SDL_WINDOWPOS_CENTERED,
                 configManager.windowWidth,
                 configManager.windowHeight,
                 SDL_WINDOW_SHOWN
             );
    if(!window) {
        printf("Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        printf("Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

bool Game::LoadConfig() {
	configManager = ConfigManager(Helper::LoadJsonFile("byte_config.json"));
    engine->SetDefaultPythonScriptClass(configManager.pyScriptClassModulePath, configManager.pyScriptClassName);
    camera.zoom = configManager.cameraZoom;
	return configManager.ValidateConfig();
}

void Game::ProcessInput() {
	inputManager->sdlPollEventCode = SDL_PollEvent(&event);
    if (inputManager->sdlPollEventCode != 1) {
        return;
    }
    switch(event.type) {
    	case SDL_QUIT:
        	running = false;
        	break;
    	default:
	        break;
    }
}

void Game::Update() {
	// Sleep until FRAME_TARGET_TIME has elapsed since last frame
    unsigned int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
    if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    // Calculate elapsed time between previous and current frame
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / MILLISECONDS_PER_TICK;

    // Clamp delta time to maximum value
    deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;

    ticksLastFrame = SDL_GetTicks();

	entityManager->Update(deltaTime);

    inputManager->CleanUpInputActions();

    timerManager->CheckTimerTimeouts();

    entityManager->DestroyInactiveEntities();

    if(engine->switchScene) {
        SwitchSceneTo(engine->sceneSwitchPath);
        engine->switchScene = false;
    }
}

void Game::Render() {
    // Set background color
    SDL_SetRenderDrawColor(renderer, configManager.backgroundRed, configManager.backgroundGreen, configManager.backgroundBlue, 255);

    // Clear back buffer
    SDL_RenderClear(renderer);

    // Render all entities
    if(entityManager->HasNoEntities()) {
        return;
    }
    entityManager->Render();

    // Swap front and back buffers
    SDL_RenderPresent(renderer);
}

bool Game::IsRunning() const {
	return this->running;
}

void Game::Destroy() {
	delete pyInstance;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::LoadScene(std::string sceneFilePath) {
	Scene newScene = Scene(Helper::LoadJsonFile(sceneFilePath),
                           entityFactory, pythonCacheManager,
                           animationFrameManager,
                           engine->pyScriptClassModulePath,
                           engine->pyScriptClassName
    );
	sceneManager->InitializeScene(newScene);
}

void Game::CleanUpScene() {
    sceneManager->CleanUpScene();
    timerManager->ClearAllTimers();
    std::vector<std::string> entityIds = entityManager->ClearAllEntities();
    for(auto& entityId : entityIds) {
        signalManager->RemoveEntitySignal(entityId);
    }
    entityManager->DestroyInactiveEntities();
}

void Game::SwitchSceneTo(std::string sceneFilePath) {
	CleanUpScene();
	LoadScene(sceneFilePath);
}

void Game::Exit() {
    this->running = false;
}
