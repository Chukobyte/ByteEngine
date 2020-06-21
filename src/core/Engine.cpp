#include "./Engine.h"

void Engine::SwitchSceneTo(std::string sceneFilePath) {
	sceneSwitchPath = sceneFilePath;
	switchScene = true;
}

void Engine::SetDefaultPythonScriptClass(std::string pyModule, std::string pyClass) {
	pyScriptClassModulePath = strcpy(new char[pyModule.length() + 1], pyModule.c_str());
	pyScriptClassName = strcpy(new char[pyClass.length() + 1], pyClass.c_str());
}

void Engine::Quit() {
	game->Exit();
}
