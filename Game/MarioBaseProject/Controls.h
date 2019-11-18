#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Utility.h"
#include "SPRITEBATCH.h"
#include "ControllerManagement.h"

#include <iostream>
using namespace::std;

class Controls
{
public:
	enum menuInput : int { miUnknown = 0, miKeyboard, miJoystick, miMouse};
	struct mouseInfo
	{
		enum mouseState : int {msNone = 0, msLeft, msRight, msMiddle};
		mouseState state = msNone;
		int *x = nullptr, *y = nullptr;
	};

	enum KeyboardKey : int
	{
		Key_Q = 0, Key_W = 1, Key_E, Key_R, Key_T, Key_Y, Key_U, Key_I, Key_O, Key_P, Key_A, Key_S, Key_D, Key_F, Key_G, Key_H, Key_J, Key_K, Key_L, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_N, Key_M,
		Num_1, Num_2, Num_3, Num_4, Num_5, Num_6, Num_7, Num_8, Num_9, Num_0,
		SPACE, RETURN, ESC, BACKSPACE, DOWN, LEFT, RIGHT, UP,
		CTRL_L, CTRL_R,
		Numpad_0, Numpad_1, Numpad_2, Numpad_3, Numpad_4, Numpad_5, Numpad_6, Numpad_7, Numpad_8, Numpad_9, Numpad_Return,
		Numpad_Minus, Numpad_Multiply, Numpad_Plus, Numpad_Period, Numpad_Divide
	};

	enum GameKeys : int
	{
		DPAD_LEFT = 0, DPAD_UP, DPAD_DOWN, DPAD_RIGHT, BTN_A, BTN_B, BTN_START, BTN_SEL, BTN_ESC, BTN_OPTION
	};
private:
	SpriteBatch *m_sb = nullptr;
	ControllerManagement *m_controllerManagement = nullptr;

	int number = 0;
	bool m_bKeyUp[100];
	bool m_bKeyPressed[100];
	bool m_bKeyPressedLastTick[100];
	bool m_bKeyUpLastTick[100];
	bool m_bKeyDown[100];

	bool m_bIsBlank = true;
	SDL_Keycode m_sdlKeycodeLast = SDLK_ESCAPE;
	int m_iKeyAction = -1;

	SDL_Keycode l_sdlKeys[100] =
	{
		SDLK_q, SDLK_w, SDLK_e, SDLK_r, SDLK_t, SDLK_y, SDLK_u, SDLK_i, SDLK_o, SDLK_p, SDLK_a, SDLK_s, SDLK_d, SDLK_f, SDLK_g, SDLK_h, SDLK_j, SDLK_k, SDLK_l, SDLK_z, SDLK_x, SDLK_c, SDLK_v, SDLK_b, SDLK_n, SDLK_m,
		SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_0,
		SDLK_SPACE, SDLK_RETURN, SDLK_ESCAPE, SDLK_BACKSPACE, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_UP,
		SDLK_LCTRL, SDLK_RCTRL,
		SDLK_KP_0, SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9, SDLK_KP_ENTER,
		SDLK_KP_MINUS,SDLK_KP_MULTIPLY,SDLK_KP_PLUS,SDLK_KP_PERIOD,SDLK_KP_DIVIDE
	};

	SDL_Scancode m_scancode[100] = 
	{
		SDL_SCANCODE_Q, SDL_SCANCODE_W,	SDL_SCANCODE_E,	SDL_SCANCODE_R,	SDL_SCANCODE_T,	SDL_SCANCODE_Y,	SDL_SCANCODE_U,	SDL_SCANCODE_I,	SDL_SCANCODE_O,	SDL_SCANCODE_P,	SDL_SCANCODE_A,	SDL_SCANCODE_S,	SDL_SCANCODE_D,	SDL_SCANCODE_F,	SDL_SCANCODE_G,	SDL_SCANCODE_H,	SDL_SCANCODE_J,	SDL_SCANCODE_K,	SDL_SCANCODE_L,	SDL_SCANCODE_Z,	SDL_SCANCODE_X,	SDL_SCANCODE_C,	SDL_SCANCODE_V,	SDL_SCANCODE_B,	SDL_SCANCODE_N,	SDL_SCANCODE_M,
		SDL_SCANCODE_1,SDL_SCANCODE_2,SDL_SCANCODE_3,SDL_SCANCODE_4,SDL_SCANCODE_5,SDL_SCANCODE_6,SDL_SCANCODE_7,SDL_SCANCODE_8,SDL_SCANCODE_9,SDL_SCANCODE_0,
		SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE, SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP,
		SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL,
		SDL_SCANCODE_KP_0, SDL_SCANCODE_KP_1,SDL_SCANCODE_KP_2,SDL_SCANCODE_KP_3,SDL_SCANCODE_KP_4,SDL_SCANCODE_KP_5,SDL_SCANCODE_KP_6,SDL_SCANCODE_KP_7,SDL_SCANCODE_KP_8,SDL_SCANCODE_KP_9, SDL_SCANCODE_KP_ENTER,
		SDL_SCANCODE_KP_MINUS,SDL_SCANCODE_KP_MULTIPLY,SDL_SCANCODE_KP_PLUS,SDL_SCANCODE_KP_PERIOD,SDL_SCANCODE_KP_DIVIDE
	};

	SDL_GameController *m_controller = nullptr;
	int m_iOldJoystickCount = 0;
	mouseInfo *m_mouseInfo = nullptr;
	mouseInfo *m_mouseInfoCompare = nullptr;
	menuInput m_menuInput = miKeyboard;
	SDL_Joystick *m_joystick[10];
	SDL_GameController  *m_gameController[10];
	bool *m_bjoystickIsController[10] = { nullptr };

	int *m_iPlayerOne = nullptr;
	int *m_iPlayerTwo = nullptr;
	int *m_iPlayerOneControllerType = nullptr;
	int *m_iPlayerTwoControllerType = nullptr;

	bool m_bAdded = false; 

	bool m_bAreInControllerManMenu = false;
	bool m_bAnyButtonDown = false;

	bool m_bMouseCursor = true;
public:
	Controls();
	~Controls();

	//Defaults
	bool Draw();
	bool Update(float f_deltaTime);

	//Sets
	void set_spriteBatch(SpriteBatch *f_sb) { m_sb = f_sb; }
	
	//Get
	menuInput get_menuInput() { return m_menuInput; }
	bool get_areInControllerManMenu() { return m_bAreInControllerManMenu; }

	//Output//
	bool isKeyUp(KeyboardKey *f_kk){int l_kk = *f_kk;return m_bKeyUp[l_kk];}
	bool isKeyUp(KeyboardKey f_kk) { int l_kk = f_kk; return m_bKeyUp[l_kk]; }
	bool isKeyDown(KeyboardKey *f_kk){int l_kk = *f_kk;return m_bKeyDown[l_kk];}
	bool isKeyDown(KeyboardKey f_kk){int l_kk = f_kk;return m_bKeyDown[l_kk];}
	bool isKeyPressed(KeyboardKey f_kk){int l_kk = f_kk;return m_bKeyPressed[l_kk];}

	bool isGameKeyDown(GameKeys f_gk, int f_iPlayer = 1);
	
	mouseInfo* get_mouseInfo() { return m_mouseInfo; }

	bool isJoypadButtonDown(GameKeys f_gameKey, int f_iPlayer);

	///Resets all key presses///
	bool WipeKeys();

	///Update Keys///
	//bool UpdateKeyboard(SDL_Event *f_sdlEvent);
	bool UpdateKeyboard();

	//Joysticks:
	bool initialise_joysticks();
	bool joysticks_removeIndex(int f_iIndex);
	bool joysticks_addedIndex(int f_iIndex);
	bool gameController_addedIndex(int f_iIndex);
	bool gameController_removeIndex(int f_iIndex);

	//Controller Management
	bool open_controllerManagement();

	//Mouse Cursor
	bool toggle_mouseCursor();
	bool toggle_mouseCursor(bool f_bNew);
private:
	//bool UpdateKeys(SDL_Event *f_sdlEvent);
	bool UpdateKeys();
	int convert_keyactiontoint(SDL_Event* f_kc);
	bool UpdateAfterEvent();
	bool update_mouseInfo();
	bool update_menuInput();

	bool is_anyButtonDown();
};

