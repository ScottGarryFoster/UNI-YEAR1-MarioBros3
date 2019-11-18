#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Utility.h"
#include "SPRITEBATCH.h"
#include "Sprite.h"

#include <string>
#include <iostream>
using namespace std;

class ControllerManagement
{
private:
	SpriteBatch *m_sb = nullptr;
	SDL_Joystick *m_joystick[10] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	SDL_GameController *m_gamecontrollers[10] = { nullptr };
	bool *m_bjoystickIsController[10] = { nullptr };

	Sprite *m_sprBackground = nullptr;
	Sprite *m_sprBackgroundPanels = nullptr;
	Sprite *m_sprControllersMedium = nullptr;
	Sprite *m_sprControllersBig = nullptr;
	Sprite *m_sprMouseController = nullptr;
	Sprite *m_sprPlayerOne = nullptr;
	Sprite *m_sprPlayerTwo = nullptr;
	Sprite *m_sprMenuButtons = nullptr;

	int *s_iPlayerOne = nullptr;//Joypad index or -1 for keyboard
	int *s_iPlayerTwo = nullptr;//Joypad index or -1 for keyboard
	bool *s_bMenuActive = nullptr;
	int *s_iPlayerOneType = nullptr;//0 for keyboard, 1 for nes, 2 for snes, 3 for ps4, 4 for xbox
	int *s_iPlayerTwoType = nullptr;//0 for keyboard, 1 for nes, 2 for snes, 3 for ps4, 4 for xbox

	int m_iNumberOfControllersSetup = 3;//Includes the keyboard
	int m_iLastClickDevice = -1;//-1 for keyboard then joypad index
	int m_iLastClickDeviceType = 0;//0 for keyboard, 1 for nes, 2 for snes, 3 for ps4, 4 for xbox
	int m_iLastClickWithSetControllers = 0;//This will set as a joypad index if we've set them up as player controllers
	bool *s_bAnyKeypressed = nullptr;//Keyboard anything pressed. Controlled by controls but you can't know about that ;)

	//Flare: The controllers are bigger when pressed
	int m_iFlareMedBiggerTick = 0;
	float m_fFlareMedSizeModify = 0;
	int m_iFlarePlayerOneBiggerTick = 0;
	float m_fFlarePlayerOneSizeModify = 0;
	int m_iFlarePlayerTwoBiggerTick = 0;
	float m_fFlarePlayerTwoSizeModify = 0;

	//Mouse Control:
	bool m_bMouseDown = false;
	bool m_bPreviousMouseDown = false;
	int m_iMouseX = -1;
	int m_iMouseY = -1;
	int m_iClickedController = -1;

	//Continue Button:
	bool m_bClickedDownOnContinue = false;
public:
	ControllerManagement(SpriteBatch *f_sb);
	virtual ~ControllerManagement();

	//Defaults
	bool Draw();
	bool Update(float f_deltaTime);

	//Sets
	void set_joysticks(SDL_Joystick *f_joysticks[10]) { for (int i = 0; i < 10; i++)m_joystick[i] = f_joysticks[i]; }
	void set_gamecontroller(SDL_GameController *f_gamecontrollers[10]) { for (int i = 0; i < 10; i++)m_gamecontrollers[i] = f_gamecontrollers[i]; }
	void set_joystickIsController(bool *l_bjoystickIsController[10]) { for (int i = 0; i < 10; i++)m_bjoystickIsController[i] = l_bjoystickIsController[i]; }

	void set_playerOneController(int *f_p1) { s_iPlayerOne = f_p1; }
	void set_playerTwoController(int *f_p2) { s_iPlayerTwo = f_p2; }
	void set_playerControllers(int *f_p1, int *f_p2) { s_iPlayerOne = f_p1;  s_iPlayerTwo = f_p2; }
	void set_menuActive(bool *f_b) { s_bMenuActive = f_b; }
	void set_playerOneControllerType(int *f_p1) { s_iPlayerOneType = f_p1; }
	void set_playerTwoControllerType(int *f_p2) { s_iPlayerTwoType = f_p2; }
	void set_playerTwoControllerTypes(int *f_p1, int *f_p2) { s_iPlayerOneType = f_p1; s_iPlayerTwoType = f_p2; }
	void set_anyKeypressed(bool *f_b) { s_bAnyKeypressed = f_b; }

private:
	bool load_content();

	bool update_backgroundLocation(float f_deltaTime);
	bool update_lastPressed(float f_deltaTime);
	bool update_unknownpadRespond(float f_deltaTime);
	bool update_mouseControl(float f_deltaTime);
};

