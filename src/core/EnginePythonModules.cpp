#include "./EnginePythonModules.h"
#include <tuple>
#include "./Game.h"
#include "./handlers/AudioHandler.h"
#include "./components/TextLabelComponent.h"
#include "./helpers/Helper.h"
#include "./helpers/pyhelper.hpp"

PyObject* EnginePythonModules::game_quit(PyObject* self, PyObject* args) {
	Game::engine->Quit();
	Py_RETURN_TRUE;
}

PyObject* EnginePythonModules::game_get_version(PyObject* self, PyObject* args) {
	return PyLong_FromLong(1);
}

PyObject* EnginePythonModules::game_log_number(PyObject* self, PyObject* args) {
	PyObject* arg;
	if(PyArg_UnpackTuple(args, "", 1, 1, &arg)) {
		printf("C++: log_message(%d)\n", PyLong_AsLong(arg));
	}
	Py_RETURN_TRUE;
}

PyObject* EnginePythonModules::game_delete_entity(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameDeleteEntitykwlist, &pyEntityId)) {
		std::string entityId = pyEntityId;
		if(Game::entityManager->HasEntity(entityId)) {
			Game::signalManager->RemoveEntitySignal(entityId);
			Game::sceneManager->RemoveEntity(entityId);
			Game::entityManager->DestroyEntity(Game::entityManager->GetEntityPointerByName(entityId));
			Py_RETURN_TRUE;
		}
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_create_sprite_entity(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	char* pyTextureId;
	int x, y, layer, width, height;
	bool isClickable = false;
	bool isFixed = false;
	char* pyColliderTag = "";
	char* pyScriptModulePath = Game::engine->pyScriptClassModulePath;
	char* pyScriptClassName = Game::engine->pyScriptClassName;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "ss(ii)iii|bbs(ss)", gameCreateSpriteEntitykwlist,
				&pyEntityId, &pyTextureId, &x, &y, &layer, &width, &height, &isClickable, &isFixed, &pyColliderTag, &pyScriptModulePath, &pyScriptClassName)) {
		std::string entityId = pyEntityId;
		std::string textureId = pyTextureId;
		std::string colliderTag = pyColliderTag;
		std::string modulePath = pyScriptModulePath;
		std::string className = pyScriptClassName;
		if(!Game::entityManager->HasEntity(entityId)) {
			Entity& entity = Game::entityFactory->Create(entityId, layer, isClickable);
			entity.AddComponent<TransformComponent>(x, y, 0, 0, width, height, 1.0f);
            if(Game::animationFrameManager->HasAnimationFrames(textureId)) {
                entity.AddComponent<SpriteComponent>(textureId, isFixed, Game::animationFrameManager->GetAnimationFrames(textureId));
            } else {
                entity.AddComponent<SpriteComponent>(textureId, isFixed);
            }
            if(colliderTag.compare("") != 0 && entity.HasComponent<TransformComponent>()) {
            	TransformComponent* transformComponent = entity.GetComponent<TransformComponent>();
            	entity.AddComponent<ColliderComponent>(colliderTag, transformComponent->position.x, transformComponent->position.y, transformComponent->width, transformComponent->height, 0, 0);
            }
            entity.AddComponent<ScriptClassComponent>(Game::pythonCacheManager, pyScriptModulePath, pyScriptClassName);
            Game::sceneManager->AddEntity(Game::entityManager->GetEntityPointerByName(entityId));
			Py_RETURN_TRUE;
		}
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_create_label_entity(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	char* pyText;
	int x, y;
	char* pyFontId;
	int layer;
	bool isFixed = false;
	int pyWrapLength = 800;
	int pyRed = 255;
	int pyGreen = 255;
	int pyBlue = 255;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "ss(ii)si|bi(iii)", gameCreateLabelEntitykwlist,
				&pyEntityId, &pyText, &x, &y, &pyFontId, &layer, &isFixed, &pyWrapLength, &pyRed, &pyGreen, &pyBlue)) {
		std::string entityId = pyEntityId;
		std::string text = pyText;
		std::string fontId = pyFontId;
		Uint32 wrapLength = static_cast<Uint32>(pyWrapLength);
		Uint8 red = static_cast<Uint8>(pyRed);
		Uint8 green = static_cast<Uint8>(pyGreen);
		Uint8 blue = static_cast<Uint8>(pyBlue);
		SDL_Color color = {red, green, blue, 0};
		bool isClickable = false;
		if(!Game::entityManager->HasEntity(entityId)) {
			Entity& entity = Game::entityFactory->Create(entityId, layer, isClickable);
			entity.AddComponent<TransformComponent>(x, y, 0, 0, 0, 0, 1.0f);
			entity.AddComponent<TextLabelComponent>(text, fontId, color, isFixed, wrapLength);
			Py_RETURN_TRUE;
		}
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_update_label_entity_text(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	char* pyText;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "ss", gameUpdateLabelEntityTextkwlist, &pyEntityId, &pyText)) {
		std::string entityId = pyEntityId;
		std::string text = pyText;
		if(Game::entityManager->HasEntity(entityId)) {
            Entity* entity = Game::entityManager->GetEntityPointerByName(entityId);
            if(entity->HasComponent<TextLabelComponent>()) {
                TextLabelComponent* textLabelComponent = entity->GetComponent<TextLabelComponent>();
                textLabelComponent->SetLabelText(text);
                Py_RETURN_TRUE;
            }
        }
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_update_entity_position(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	int positionX;
	int positionY;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s(ii)", gameUpdateEntityPositionTextkwlist, &pyEntityId, &positionX, &positionY)) {
		std::string entityId = pyEntityId;
		if(Game::entityManager->HasEntity(entityId)) {
            Entity* entity = Game::entityManager->GetEntityPointerByName(entityId);
            if(entity->HasComponent<TransformComponent>()) {
                TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
                transformComponent->position.x = positionX;
                transformComponent->position.y = positionY;
                Py_RETURN_TRUE;
            }
        }
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_set_entity_visibility(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	bool isVisible;
	int positionY;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "sb", gameSetEntityVisibilitykwlist, &pyEntityId, &isVisible)) {
		std::string entityId = pyEntityId;
		if(Game::entityManager->HasEntity(entityId)) {
            Entity* entity = Game::entityManager->GetEntityPointerByName(entityId);
            entity->SetVisibility(isVisible);
            Py_RETURN_TRUE;
        }
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_play_animation(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	char* pyAnimationName;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "ss", gamePlayAnimationkwlist, &pyEntityId, &pyAnimationName)) {
		std::string entityId = pyEntityId;
		std::string animationName = pyAnimationName;
		if(Game::entityManager->HasEntity(entityId)) {
            Entity* entity = Game::entityManager->GetEntityPointerByName(entityId);
            SpriteComponent* spriteComponent = entity->GetComponent<SpriteComponent>();
            spriteComponent->Play(animationName);
            Py_RETURN_TRUE;
        }
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_stop_animation(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	char* pyAnimationName;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameStopAnimationkwlist, &pyEntityId)) {
		std::string entityId = pyEntityId;
		if(Game::entityManager->HasEntity(entityId)) {
            Entity* entity = Game::entityManager->GetEntityPointerByName(entityId);
            SpriteComponent* spriteComponent = entity->GetComponent<SpriteComponent>();
            spriteComponent->Stop();
            Py_RETURN_TRUE;
        }
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_set_animation_frame(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	int animationFrameIndex;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "si", gameSetAnimationFramekwlist, &pyEntityId, &animationFrameIndex)) {
		std::string entityId = pyEntityId;
		if(Game::entityManager->HasEntity(entityId)) {
            Entity* entity = Game::entityManager->GetEntityPointerByName(entityId);
            SpriteComponent* spriteComponent = entity->GetComponent<SpriteComponent>();
            spriteComponent->SetPlayingAnimationIndex(animationFrameIndex);
            Py_RETURN_TRUE;
        }
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_action_pressed(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyAction;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameIsActionInputkwlist, &pyAction)) {
		std::string action = pyAction;
		if(Game::inputManager->IsActionPressed(action)) {
			Py_RETURN_TRUE;
		}
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_action_just_pressed(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyAction;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameIsActionInputkwlist, &pyAction)) {
		std::string action = pyAction;
		if(Game::inputManager->IsActionJustPressed(action)) {
			Py_RETURN_TRUE;
		}
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_action_just_released(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyAction;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameIsActionInputkwlist, &pyAction)) {
		std::string action = pyAction;
		if(Game::inputManager->IsActionJustReleased(action)) {
			Py_RETURN_TRUE;
		}
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_mouse_button_left_pressed(PyObject* self, PyObject* args) {
	if(Game::inputManager->IsMouseButtonLeftPressed()) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_mouse_button_left_just_pressed(PyObject* self, PyObject* args) {
	if(Game::inputManager->IsMouseButtonLeftJustPressed()) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_mouse_button_left_just_released(PyObject* self, PyObject* args) {
	if(Game::inputManager->IsMouseButtonLeftJustReleased()) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_mouse_button_right_pressed(PyObject* self, PyObject* args) {
	if(Game::inputManager->IsMouseButtonRightPressed()) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_mouse_button_right_just_pressed(PyObject* self, PyObject* args) {
	if(Game::inputManager->IsMouseButtonRightJustPressed()) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_is_mouse_button_right_just_released(PyObject* self, PyObject* args) {
	if(Game::inputManager->IsMouseButtonRightJustReleased()) {
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_play_music(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyMusicId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gamePlayMusickwlist, &pyMusicId)) {
		std::string musicId = pyMusicId;
		AudioHandler::PlayMusic(musicId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_stop_music(PyObject* self, PyObject* args) {
	AudioHandler::StopMusic();
	Py_RETURN_TRUE;
}

PyObject* EnginePythonModules::game_pause_music(PyObject* self, PyObject* args) {
	AudioHandler::PauseMusic();
	Py_RETURN_TRUE;
}

PyObject* EnginePythonModules::game_resume_music(PyObject* self, PyObject* args) {
	AudioHandler::ResumeMusic();
	Py_RETURN_TRUE;
}

PyObject* EnginePythonModules::game_play_sound(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pySoundId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gamePlaySoundkwlist, &pySoundId)) {
		std::string soundId = pySoundId;
		AudioHandler::PlaySoundEffect(soundId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_set_volume(PyObject* self, PyObject* args, PyObject* kwargs) {
	int volume;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "i", gameSetVolumekwlist, &volume)) {
		AudioHandler::SetVolume(volume);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;	
}

PyObject* EnginePythonModules::game_create_timer(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyTimerId;
	float pyTime;
	bool loops = false;
	bool startOnCreation = false;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "sf|bb", gameCreateTimerkwlist, &pyTimerId, &pyTime, &loops, &startOnCreation)) {
		std::string timerId = pyTimerId;
		Uint32 time = static_cast<Uint32>(pyTime * 1000);
		Game::timerManager->CreateTimer(timerId, time, loops, startOnCreation);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_start_timer(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyTimerId;
	float pyTime = -1.0f;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s|f", gameStartTimerkwlist, &pyTimerId, &pyTime)) {
		std::string timerId = pyTimerId;
		if(pyTime > 0.0) {
			Uint32 time = static_cast<Uint32>(pyTime * 1000);
			Game::timerManager->SetWaitTime(timerId, time);
		}
		Game::timerManager->StartTimer(timerId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_stop_timer(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyTimerId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameAllTimerkwlist, &pyTimerId)) {
		std::string timerId = pyTimerId;
		Game::timerManager->StopTimer(timerId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_pause_timer(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyTimerId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameAllTimerkwlist, &pyTimerId)) {
		std::string timerId = pyTimerId;
		Game::timerManager->PauseTimer(timerId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_resume_timer(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyTimerId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameAllTimerkwlist, &pyTimerId)) {
		std::string timerId = pyTimerId;
		Game::timerManager->UnPauseTimer(timerId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_has_timer_stopped(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyTimerId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameAllTimerkwlist, &pyTimerId)) {
		std::string timerId = pyTimerId;
		if(Game::timerManager->HasTimerStopped(timerId)) {
			Py_RETURN_TRUE;
		}
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_delete_timer(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyTimerId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameAllTimerkwlist, &pyTimerId)) {
		std::string timerId = pyTimerId;
		Game::timerManager->DeleteTimer(timerId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_create_signal(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	char* pySignalId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "ss", gameCreateSignalkwlist, &pyEntityId, &pySignalId)) {
		std::string entityId = pyEntityId;
		std::string signalId = pySignalId;
		Game::signalManager->CreateSignal(pyEntityId, pySignalId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_subscribe_to_signal(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pySourceId;
	char* pySignalId;
	char* pySubscriberEntityId;
	char* pyFunctionName;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "ssss", gameSubscribeToSignalkwlist, &pySourceId, &pySignalId, &pySubscriberEntityId, &pyFunctionName)) {
		std::string sourceId = pySourceId;
		std::string signalId = pySignalId;
		std::string subscriberEntityId = pySubscriberEntityId;
		std::string functionName = pyFunctionName;
		Game::signalManager->SubscribeToSignal(sourceId, signalId, subscriberEntityId, functionName);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_emit_signal(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	char* pySignalId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "ss", gameEmitSignalkwlist, &pyEntityId, &pySignalId)) {
		std::string entityId = pyEntityId;
		std::string signalId = pySignalId;
		Game::signalManager->EmitSignal(pyEntityId, pySignalId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_remove_signal(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameRemoveSignalkwlist, &pyEntityId)) {
		std::string entityId = pyEntityId;
		Game::signalManager->RemoveEntitySignal(entityId);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_change_to_scene(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyScenePath;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s", gameChangeToScenekwlist, &pyScenePath)) {
		std::string scenePath = pyScenePath;
		Game::engine->SwitchSceneTo(scenePath);
		Py_RETURN_TRUE;
	}
	Py_RETURN_FALSE;
}

PyObject* EnginePythonModules::game_check_entity_collision(PyObject* self, PyObject* args, PyObject* kwargs) {
	char* pyEntityId;
	int offsetX = 0;
	int offsetY = 0;
	if(PyArg_ParseTupleAndKeywords(args, kwargs, "s|(ii)", gameCheckEntityCollisionkwlist, &pyEntityId, &offsetX, &offsetY)) {
		std::string entityId = pyEntityId;
		std::tuple<std::string, std::string> collisionTuple = Game::sceneManager->CheckEntityCollisionWithCollider(entityId, glm::vec2(offsetX, offsetY));
		std::string collissionEntityId = std::get<0>(collisionTuple);
		std::string collissionColliderTag = std::get<1>(collisionTuple);
		if(collissionEntityId.compare("") != 0 && collissionColliderTag.compare("") != 0 ) {
			return Py_BuildValue("(ss)", collissionEntityId.c_str(), collissionColliderTag.c_str());
		}
	}
	return Py_BuildValue("(ss)", "", "");
}