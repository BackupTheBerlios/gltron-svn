#ifndef NEBU_INPUT_SYSTEM_H
#define NEBU_INPUT_SYSTEM_H

#include <SDL.h>
#include <SDL_types.h>

#define SYSTEM_KEY_DOWN SDLK_DOWN
#define SYSTEM_KEY_UP SDLK_UP
#define SYSTEM_KEY_LEFT SDLK_LEFT
#define SYSTEM_KEY_RIGHT SDLK_RIGHT
#define SYSTEM_KEY_F1 SDLK_F1
#define SYSTEM_KEY_F2 SDLK_F2
#define SYSTEM_KEY_F3 SDLK_F3
#define SYSTEM_KEY_F4 SDLK_F4
#define SYSTEM_KEY_F5 SDLK_F5
#define SYSTEM_KEY_F6 SDLK_F6
#define SYSTEM_KEY_F7 SDLK_F7
#define SYSTEM_KEY_F10 SDLK_F10
#define SYSTEM_KEY_F11 SDLK_F11
#define SYSTEM_KEY_F12 SDLK_F12

#define SYSTEM_KEY_ENTER SDLK_ENTER
#define SYSTEM_KEY_RETURN SDLK_RETURN

#define SYSTEM_MOUSEUP SDL_MOUSEBUTTONUP
#define SYSTEM_MOUSEDOWN SDL_MOUSEBUTTONDOWN

#define SYSTEM_MOUSEPRESSED SDL_PRESSED
#define SYSTEM_MOUSERELEASED SDL_RELEASED

#define SYSTEM_MOUSEBUTTON_LEFT SDL_BUTTON_LEFT
#define SYSTEM_MOUSEBUTTON_RIGHT SDL_BUTTON_RIGHT

#define SYSTEM_KEY_TAB SDLK_TAB

#define SYSTEM_JOY_AXIS_MAX 32767

enum {
	SYSTEM_JOY_OFFSET = 32,
	SYSTEM_CUSTOM_KEYS = 512,
	SYSTEM_JOY_LEFT = 512,
	SYSTEM_JOY_RIGHT,
	SYSTEM_JOY_UP,
	SYSTEM_JOY_DOWN,
	SYSTEM_JOY_BUTTON_0,
	SYSTEM_JOY_BUTTON_1,
	SYSTEM_JOY_BUTTON_2,
	SYSTEM_JOY_BUTTON_3,
	SYSTEM_JOY_BUTTON_4,
	SYSTEM_JOY_BUTTON_5,
	SYSTEM_JOY_BUTTON_6,
	SYSTEM_JOY_BUTTON_7,
	SYSTEM_JOY_BUTTON_8,
	SYSTEM_JOY_BUTTON_9,
	SYSTEM_JOY_BUTTON_10,
	SYSTEM_JOY_BUTTON_11,
	SYSTEM_JOY_BUTTON_12,
	SYSTEM_JOY_BUTTON_13,
	SYSTEM_JOY_BUTTON_14,
	SYSTEM_JOY_BUTTON_15,
	SYSTEM_JOY_BUTTON_16,
	SYSTEM_JOY_BUTTON_17,
	SYSTEM_JOY_BUTTON_18,
	SYSTEM_JOY_BUTTON_19
};

extern void SystemGrabInput();
extern void SystemUngrabInput();

extern char* SystemGetKeyName(int key);

extern void SystemMouse(int buttons, int state, int x, int y);
extern void SystemMouseMotion(int x, int y);

#endif
