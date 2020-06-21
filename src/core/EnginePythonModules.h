#ifndef ENGINEPYTHONMODULES_H
#define ENGINEPYTHONMODULES_H

#include <Python.h>
#include <string>

class EnginePythonModules {
	public:
		static PyObject* game_quit(PyObject* self, PyObject* args);

		static PyObject* game_get_version(PyObject* self, PyObject* args);

		static PyObject* game_log_number(PyObject* self, PyObject* args);

		// ENTITY
		static PyObject* game_create_sprite_entity(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_create_label_entity(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_update_label_entity_text(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_update_entity_position(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_delete_entity(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_set_entity_visibility(PyObject* self, PyObject* args, PyObject* kwargs);

		static PyObject* game_subscribe_to(PyObject* self, PyObject* args, PyObject* kwargs);

		// ANIMATION
		static PyObject* game_play_animation(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_stop_animation(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_set_animation_frame(PyObject* self, PyObject* args, PyObject* kwargs);

		// INPUT
		static PyObject* game_is_action_pressed(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_is_action_just_pressed(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_is_action_just_released(PyObject* self, PyObject* args, PyObject* kwargs);

		static PyObject* game_is_mouse_button_left_pressed(PyObject* self, PyObject* args);
		static PyObject* game_is_mouse_button_left_just_pressed(PyObject* self, PyObject* args);
		static PyObject* game_is_mouse_button_left_just_released(PyObject* self, PyObject* args);
		static PyObject* game_is_mouse_button_right_pressed(PyObject* self, PyObject* args);
		static PyObject* game_is_mouse_button_right_just_pressed(PyObject* self, PyObject* args);
		static PyObject* game_is_mouse_button_right_just_released(PyObject* self, PyObject* args);

		// AUDIO
		static PyObject* game_play_music(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_stop_music(PyObject* self, PyObject* args);
		static PyObject* game_pause_music(PyObject* self, PyObject* args);
		static PyObject* game_resume_music(PyObject* self, PyObject* args);
		static PyObject* game_play_sound(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_set_volume(PyObject* self, PyObject* args, PyObject* kwargs);

		// TIMER
		static PyObject* game_create_timer(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_start_timer(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_stop_timer(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_pause_timer(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_resume_timer(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_has_timer_stopped(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_delete_timer(PyObject* self, PyObject* args, PyObject* kwargs);

		// SIGNALS
		static PyObject* game_create_signal(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_subscribe_to_signal(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_emit_signal(PyObject* self, PyObject* args, PyObject* kwargs);
		static PyObject* game_remove_signal(PyObject* self, PyObject* args, PyObject* kwargs);

		// SCENE
		static PyObject* game_change_to_scene(PyObject* self, PyObject* args, PyObject* kwargs);

		// COLLISION
		static PyObject* game_check_entity_collision(PyObject* self, PyObject* args, PyObject* kwargs);
};

static struct PyMethodDef gameMethods[] = {
	{ "quit", EnginePythonModules::game_quit, METH_VARARGS, "Quit byte engine" },
	{ "get_version", EnginePythonModules::game_get_version, METH_VARARGS, "Returns game version number" },
	{ "log_number", EnginePythonModules::game_log_number, METH_VARARGS, "Show a number" },

	{ "create_sprite_entity", (PyCFunction) EnginePythonModules::game_create_sprite_entity, METH_VARARGS | METH_KEYWORDS, "Create a sprite entity" },
	{ "create_label_entity", (PyCFunction) EnginePythonModules::game_create_label_entity, METH_VARARGS | METH_KEYWORDS, "Create a label entity" },
	{ "update_label_entity_text", (PyCFunction) EnginePythonModules::game_update_label_entity_text, METH_VARARGS | METH_KEYWORDS, "Update a label entity's text" },
	{ "update_entity_position", (PyCFunction) EnginePythonModules::game_update_entity_position, METH_VARARGS | METH_KEYWORDS, "Update an entity's position" },
	{ "delete_entity", (PyCFunction) EnginePythonModules::game_delete_entity, METH_VARARGS | METH_KEYWORDS, "Delete an entity" },
	{ "set_entity_visibility", (PyCFunction) EnginePythonModules::game_set_entity_visibility, METH_VARARGS | METH_KEYWORDS, "Determines if an entity should render" },

	{ "play_animation", (PyCFunction) EnginePythonModules::game_play_animation, METH_VARARGS | METH_KEYWORDS, "Plays an animation" },
	{ "stop_animation", (PyCFunction) EnginePythonModules::game_stop_animation, METH_VARARGS | METH_KEYWORDS, "Stops an animation" },
	{ "set_animation_frame", (PyCFunction) EnginePythonModules::game_set_animation_frame, METH_VARARGS | METH_KEYWORDS, "Sets frame index for animation" },

	{ "is_action_pressed", (PyCFunction) EnginePythonModules::game_is_action_pressed, METH_VARARGS | METH_KEYWORDS, "Check if input action is pressed" },
	{ "is_action_just_pressed", (PyCFunction) EnginePythonModules::game_is_action_just_pressed, METH_VARARGS | METH_KEYWORDS, "Check if input action is just pressed" },
	{ "is_action_just_released", (PyCFunction) EnginePythonModules::game_is_action_just_released, METH_VARARGS | METH_KEYWORDS, "Check if input action is just released" },

	{ "is_mouse_button_left_pressed", EnginePythonModules::game_is_mouse_button_left_pressed, METH_VARARGS, "Check if left mouse button is pressed" },
	{ "is_mouse_button_left_just_pressed", EnginePythonModules::game_is_mouse_button_left_just_pressed, METH_VARARGS, "Check if left mouse button is just pressed" },
	{ "is_mouse_button_left_just_released", EnginePythonModules::game_is_mouse_button_left_just_released, METH_VARARGS, "Check if left mouse button is just released" },
	{ "is_mouse_button_right_pressed", EnginePythonModules::game_is_mouse_button_right_pressed, METH_VARARGS, "Check if right mouse button is pressed" },
	{ "is_mouse_button_right_just_pressed", EnginePythonModules::game_is_mouse_button_right_just_pressed, METH_VARARGS, "Check if right mouse button is just pressed" },
	{ "is_mouse_button_right_just_released", EnginePythonModules::game_is_mouse_button_right_just_released, METH_VARARGS, "Check if right mouse button is just released" },

	{ "play_music", (PyCFunction) EnginePythonModules::game_play_music, METH_VARARGS | METH_KEYWORDS, "Play music" },
	{ "stop_music", EnginePythonModules::game_stop_music, METH_VARARGS, "Stop all music" },
	{ "pause_music", EnginePythonModules::game_pause_music, METH_VARARGS, "Pause music" },
	{ "resume_music", EnginePythonModules::game_resume_music, METH_VARARGS, "Resume music" },
	{ "play_sound", (PyCFunction) EnginePythonModules::game_play_sound, METH_VARARGS | METH_KEYWORDS, "Play sound" },
	{ "set_volume", (PyCFunction) EnginePythonModules::game_set_volume, METH_VARARGS | METH_KEYWORDS, "Set audio volume" },

	{ "create_timer", (PyCFunction) EnginePythonModules::game_create_timer, METH_VARARGS | METH_KEYWORDS, "Creates a timer" },
	{ "start_timer", (PyCFunction) EnginePythonModules::game_start_timer, METH_VARARGS | METH_KEYWORDS, "Starts a timer" },
	{ "stop_timer", (PyCFunction) EnginePythonModules::game_stop_timer, METH_VARARGS | METH_KEYWORDS, "Stops a timer" },
	{ "pause_timer", (PyCFunction) EnginePythonModules::game_pause_timer, METH_VARARGS | METH_KEYWORDS, "Pauses a timer" },
	{ "resume_timer", (PyCFunction) EnginePythonModules::game_resume_timer, METH_VARARGS | METH_KEYWORDS, "Resumes a timer" },
	{ "has_timer_stopped", (PyCFunction) EnginePythonModules::game_has_timer_stopped, METH_VARARGS | METH_KEYWORDS, "Check if a timer has stopped" },
	{ "delete_timer", (PyCFunction) EnginePythonModules::game_delete_timer, METH_VARARGS | METH_KEYWORDS, "Deletes a timer" },

	{ "create_signal", (PyCFunction) EnginePythonModules::game_create_signal, METH_VARARGS | METH_KEYWORDS, "Creates a signal" },
	{ "subscribe_to_signal", (PyCFunction) EnginePythonModules::game_subscribe_to_signal, METH_VARARGS | METH_KEYWORDS, "Subscribes to signal." },
	{ "emit_signal", (PyCFunction) EnginePythonModules::game_emit_signal, METH_VARARGS | METH_KEYWORDS, "Emits a signal" },
	{ "remove_signal", (PyCFunction) EnginePythonModules::game_remove_signal, METH_VARARGS | METH_KEYWORDS, "Removes a signal" },

	{ "change_to_scene", (PyCFunction) EnginePythonModules::game_change_to_scene, METH_VARARGS | METH_KEYWORDS, "Switches to a new scene" },
	{ "check_entity_collision", (PyCFunction) EnginePythonModules::game_check_entity_collision, METH_VARARGS | METH_KEYWORDS, "Returns a collision tag if collided with entity" },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef gameModDef = {
	PyModuleDef_HEAD_INIT, "game", NULL, -1, gameMethods,
	NULL, NULL, NULL, NULL
};

static char *gameCreateSpriteEntitykwlist[] = {"entity_id", "texture_id", "position", "layer", "width", "height", "clickable", "fixed", "collider_tag", "script_class", NULL};

static char *gameCreateLabelEntitykwlist[] = {"entity_id", "text", "position", "font_id", "layer", "fixed", "wrap_length", "color", NULL};

static char *gameUpdateLabelEntityTextkwlist[] = {"entity_id", "text", NULL};

static char *gameUpdateEntityPositionTextkwlist[] = {"entity_id", "position", NULL};

static char *gameDeleteEntitykwlist[] = {"entity_id", NULL};

static char *gameSetEntityVisibilitykwlist[] = {"entity_id", "visible", NULL};

static char *gamePlayAnimationkwlist[] = {"entity_id", "animation_name", NULL};

static char *gameStopAnimationkwlist[] = {"entity_id", NULL};

static char *gameSetAnimationFramekwlist[] = {"entity_id", "frame", NULL};

static char *gameIsActionInputkwlist[] = {"action", NULL};

static char *gamePlayMusickwlist[] = {"music_id", NULL};

static char *gamePlaySoundkwlist[] = {"sound_id", NULL};

static char *gameSetVolumekwlist[] = {"volume", NULL};

static char *gameCreateTimerkwlist[] = {"timer_id", "time", "loops", "start_on_creation", NULL};

static char *gameStartTimerkwlist[] = {"timer_id", "time", NULL};

static char *gameAllTimerkwlist[] = {"timer_id", NULL};

static char *gameCreateSignalkwlist[] = {"entity_id", "signal_id", NULL};

static char *gameSubscribeToSignalkwlist[] = {"source_id", "signal_id", "subscriber_entity_id", "function_name", NULL};

static char *gameEmitSignalkwlist[] = {"entity_id", "signal_id", NULL};

static char *gameRemoveSignalkwlist[] = {"entity_id", NULL};

static char *gameChangeToScenekwlist[] = {"file_path", NULL};

static char *gameCheckEntityCollisionkwlist[] = {"entity_id", "offset_position", NULL};

static PyObject* PyInit_game(void) {
	return PyModule_Create(&gameModDef);
}

#endif
