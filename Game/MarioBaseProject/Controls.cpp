#pragma once
#include "Controls.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>



Controls::Controls()
{

	for (int i = 0; i < 100; i++)
	{
		m_bKeyDown[i] = false;
		m_bKeyUp[i] = false;
		m_bKeyPressed[i] = false;
		m_bKeyPressedLastTick[i] = false;
	}

	m_iPlayerOne = new int(-1);
	m_iPlayerTwo = new int(-1);

	m_iPlayerOneControllerType = new int(0);
	m_iPlayerTwoControllerType = new int(0);

	for (int i = 0; i < 10; i++)
	{
		m_joystick[i] = nullptr;
		m_gameController[i] = nullptr;
		m_bjoystickIsController[i] = new bool(false);
	}
}


Controls::~Controls()
{
}

bool Controls::Draw()
{
	if (m_bAreInControllerManMenu)
	{
		if (m_controllerManagement != nullptr)
			m_controllerManagement->Draw();
		return false;
	}
	return true;
}

bool Controls::Update(float f_deltaTime)
{
	if (m_bAreInControllerManMenu)
	{
		if (m_controllerManagement != nullptr)
		{
			m_bAnyButtonDown = is_anyButtonDown();
			m_controllerManagement->Update(f_deltaTime);
		}
		return false;
	}
	return true;
}

bool Controls::isGameKeyDown(GameKeys f_gk, int f_iPlayer)
{
	switch (f_gk)
	{
	case DPAD_LEFT:
		switch (f_iPlayer)
		{
		case 2:
			if(*m_iPlayerTwoControllerType == 0)
				if (isKeyDown(Numpad_4)) return true;
			if (isJoypadButtonDown(DPAD_LEFT, f_iPlayer)) return true;
			return false;
		default:
		case 1:
			if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
				if (isKeyDown(Key_A)) return true;
			if (isJoypadButtonDown(DPAD_LEFT, f_iPlayer)) return true;
			return false;
		}
	case DPAD_RIGHT:
		switch (f_iPlayer)
		{
		case 2:
			if (*m_iPlayerTwoControllerType == 0)
				if (isKeyDown(Numpad_6)) return true;
			if (isJoypadButtonDown(DPAD_RIGHT, f_iPlayer)) return true;
			return false;
		default:
		case 1:
			if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
				if (isKeyDown(Key_D)) return true;
			if (isJoypadButtonDown(DPAD_RIGHT, f_iPlayer)) return true;
			return false;
		}
	case DPAD_UP:
		switch (f_iPlayer)
		{
		case 2:
			if (*m_iPlayerTwoControllerType == 0)
				if (isKeyDown(Numpad_8)) return true;
			if (isJoypadButtonDown(DPAD_UP, f_iPlayer)) return true;
			return false;
		default:
		case 1:
			if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
				if (isKeyDown(Key_W)) return true;
			if (isJoypadButtonDown(DPAD_UP, f_iPlayer)) return true;
			return false;
		}
	case DPAD_DOWN:
		switch (f_iPlayer)
		{
		case 2:
			if (*m_iPlayerTwoControllerType == 0)
				if (isKeyDown(Numpad_2)) return true;
			if (isJoypadButtonDown(DPAD_DOWN, f_iPlayer)) return true;
			return false;
		default:
		case 1:
			if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
				if (isKeyDown(Key_S)) return true;
			if (isJoypadButtonDown(DPAD_DOWN, f_iPlayer)) return true;
			return false;
		}
	case BTN_A:
		switch (f_iPlayer)
		{
		case 2:
			if (*m_iPlayerTwoControllerType == 0)
				if (isKeyDown(Numpad_Return)) return true;
			if (isJoypadButtonDown(BTN_A, f_iPlayer)) return true;
			return false;
		default:
		case 1:
			if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
				if (isKeyDown(SPACE)) return true;
			if (isJoypadButtonDown(BTN_A, f_iPlayer)) return true;
			return false;
		}
	case BTN_START:
		if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
			if (isKeyDown(RETURN)) return true;
		if (isJoypadButtonDown(BTN_START, f_iPlayer)) return true;
		return false;
	case BTN_SEL:
		if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
			if (isKeyDown(BACKSPACE)) return true;
		if (isJoypadButtonDown(BTN_SEL, f_iPlayer)) return true;
		return false;
	case BTN_B:
		switch (f_iPlayer)
		{
		case 2:
			if (*m_iPlayerTwoControllerType == 0)
				if (isKeyDown(Numpad_1)) return true;
			if (isJoypadButtonDown(BTN_B, f_iPlayer)) return true;
			return false;
		default:
		case 1:
			if (*m_iPlayerOneControllerType == 0 || m_menuInput == miKeyboard)
				if (isKeyDown(Controls::KeyboardKey::CTRL_L)) return true;
			if (isJoypadButtonDown(BTN_B, f_iPlayer)) return true;
			return false;
		}
	case BTN_ESC:
		if (isKeyDown(ESC)) return true;
		if (isJoypadButtonDown(BTN_ESC, f_iPlayer)) return true;
		return false;
	case BTN_OPTION:
		if (isKeyDown(CTRL_R)) return true;
		if (isJoypadButtonDown(BTN_OPTION, f_iPlayer)) return true;
		return false;
	}
	return false;
}

bool Controls::WipeKeys()
{
	for (int i = 0; i < 100; i++)
		m_bKeyUp[i] = false;

	return true;
}

bool Controls::UpdateKeyboard()
{
	update_mouseInfo();
	//WipeKeys();
	UpdateKeys();
	return true;
}

bool Controls::UpdateKeys()
{
	int *numkeys = nullptr;
	const Uint8 *state = SDL_GetKeyboardState(numkeys);
	
	for (int i = 0; i < 100; i++)
	{
		if (m_scancode[i] == NULL) continue;
		if (state[m_scancode[i]])
		{
			m_bKeyDown[i] = true;//Set key down
			m_menuInput = miKeyboard;
		}
		else
		{
			m_bKeyDown[i] = false;//Set key down
		}
	}

	for (int i = 0; i < 100; i++)
	{
		if (m_bKeyDown[i] == NULL) continue;
		if (m_bKeyDown[i])
		{
			m_bKeyPressed[i] = false;
			if (m_bKeyPressedLastTick[i])
			{
				m_bKeyPressed[i] = true;
				m_bKeyPressedLastTick[i] = false;
			}
		}
		else
		{
			m_bKeyPressedLastTick[i] = true;
		}
	}

	for(int i = 0; i < 10; i++)
		if(isJoypadButtonDown((GameKeys)i,1))
			m_menuInput = miJoystick;

	return true;
}

int Controls::convert_keyactiontoint(SDL_Event* f_kc)
{
	switch (f_kc->key.state)
	{
	case SDL_PRESSED:
		return 0;
	case SDL_RELEASED:
		return 1;
	default:
		return -2;
	}
}
bool Controls::UpdateAfterEvent()
{
	for (int i = 0; i < 100; i++)
	{
		//Is the key we're looking in the one in the event?
		if (l_sdlKeys[i] != m_sdlKeycodeLast) continue; //If not skip this

		switch (m_iKeyAction)
		{
		case 0://Key down
			m_bKeyDown[i] = true;//Set key down
			m_bKeyUp[i] = false;//Set key up
			return true;//Only one per
		case 1://Key up
			m_bKeyDown[i] = false;//Set key down
			m_bKeyUp[i] = true;//Set key up
			return true;//Only one per
		}
	}
	return false;

}

bool Controls::update_mouseInfo()
{
	if (m_mouseInfo == nullptr)
	{
		m_mouseInfo = new mouseInfo();
		m_mouseInfo->x = new int();
		m_mouseInfo->y = new int();
	}
		

	SDL_PumpEvents();
	int mouseState = SDL_GetMouseState(m_mouseInfo->x, m_mouseInfo->y);//Technically a Unit32 however an int will do just fine
	if (SDL_BUTTON(SDL_BUTTON_LEFT) == mouseState)
		m_mouseInfo->state = mouseInfo::msLeft;
	else if (SDL_BUTTON(SDL_BUTTON_MIDDLE) == mouseState)
		m_mouseInfo->state = mouseInfo::msMiddle;
	else if (SDL_BUTTON(SDL_BUTTON_RIGHT) == mouseState)
		m_mouseInfo->state = mouseInfo::msRight;
	else
		m_mouseInfo->state = mouseInfo::msNone;


	if (m_mouseInfo->x == NULL)
		*m_mouseInfo->x = -1;

	if (m_mouseInfo->y == NULL)
		*m_mouseInfo->y = -1;

	/*if(m_mouseInfoCompare != nullptr)	//Uncomment for debugging
		if (m_mouseInfoCompare->state != m_mouseInfo->state)
		{
			if (m_mouseInfo->state == mouseInfo::msNone)
				cout << "MOUSE NONE" << endl;
			else if (m_mouseInfo->state == mouseInfo::msLeft)
				cout << "MOUSE LEFT" << endl;
			else if (m_mouseInfo->state == mouseInfo::msRight)
				cout << "MOUSE RIGHT" << endl;
			else if (m_mouseInfo->state == mouseInfo::msMiddle)
				cout << "MOUSE MIDDLE" << endl;
		}*/
		

	update_menuInput();

	return true;
}

bool Controls::update_menuInput()
{
	if (m_mouseInfoCompare == nullptr)
	{
		m_mouseInfoCompare = new mouseInfo();
		m_mouseInfoCompare->x = new int();
		*m_mouseInfoCompare->x = -1;
		m_mouseInfoCompare->y = new int();
		*m_mouseInfoCompare->y = -1;
	}

	if (m_mouseInfo->state != m_mouseInfoCompare->state)
		m_menuInput = miMouse;
	
	*m_mouseInfoCompare->x = *m_mouseInfo->x;
	*m_mouseInfoCompare->y = *m_mouseInfo->y;
	m_mouseInfoCompare->state = m_mouseInfo->state;

	return true;
}

bool Controls::initialise_joysticks()
{
	SDL_JoystickUpdate();
	if (!m_bAdded)
	{
		int l_iMappingsAdded = SDL_GameControllerAddMappingsFromFile("GameFiles/Controllers/gamecontrollerdb.txt");
		if (l_iMappingsAdded > 0) cout << "ADDED: " << l_iMappingsAdded << " mappings." << endl;
		m_bAdded = true;
	}
	cout << "SDL_NumJoysticks(): " << SDL_NumJoysticks() << endl;

	for (int i = 0; i < 10; i++)
	{
		if (m_joystick[i] != nullptr)
		{
			if (*m_bjoystickIsController[i])
				gameController_removeIndex(i);
			else
				joysticks_removeIndex(i);
		}
	}

	int l_iNumberJoysticks = SDL_NumJoysticks();
	if (l_iNumberJoysticks > 10) l_iNumberJoysticks = 10;
	for (int i = 0; i < l_iNumberJoysticks; i++)
	{
		if (SDL_IsGameController(i))
			gameController_addedIndex(i);
		else
			joysticks_addedIndex(i);
	}
	return true;
}

bool Controls::isJoypadButtonDown(GameKeys f_gameKey, int f_iPlayer)
{
	SDL_JoystickUpdate();
	//if (m_joystick[0] == nullptr) return false;

	int l_iJoystickID = -1;
	int l_iControllerType = 0;
	switch (f_iPlayer)
	{
	case 1:
		if (*m_iPlayerOneControllerType == 0)
			return false;//Can't press a button on a controller that isn't set
		if (*m_iPlayerOne == -1)
			return false;//Can't press a button if the controller is the keyboard
		l_iJoystickID = *m_iPlayerOne;
		l_iControllerType = *m_iPlayerOneControllerType;
		break;
	case 2:
		if (*m_iPlayerTwoControllerType == 0)
			return false;//Can't press a button on a controller that isn't set
		if (*m_iPlayerTwo == -1)
			return false;//Can't press a button if the controller is the keyboard
		l_iJoystickID = *m_iPlayerTwo;
		l_iControllerType = *m_iPlayerTwoControllerType;
		break;
	}

	//for (int i = 0; i < 10; i++)
	//{
	if (m_joystick[l_iJoystickID] == nullptr) return false;

	/*
		NES:
		A - 1	B - 0	START - 9	SEL - 8
		
		SNES
		A - 0	B - 1	X - 2	Y - 3	L - 4	 R - 5	START - 7	SEL - 6
	*/
	switch (f_gameKey)
	{
	case BTN_A:
		if(l_iControllerType == 1)//NES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 1) == 1)
				return true;
		if (l_iControllerType == 2)//SNES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 0) == 1)
				return true;
		break;
	case BTN_B:
		if (l_iControllerType == 1)//NES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 0) == 1)
				return true;
		if (l_iControllerType == 2)//SNES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 1) == 1)
				return true;
		break;
	case BTN_START:
		if (l_iControllerType == 1)//NES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 9) == 1)
				return true;
		if (l_iControllerType == 2)//SNES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 7) == 1)
				return true;
		break;
	case BTN_SEL:
		if (l_iControllerType == 1)//NES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 8) == 1)
				return true;
		if (l_iControllerType == 2)//SNES
			if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 6) == 1)
				return true;
		break;
	case BTN_OPTION:
		if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 3) == 1)
			return true;
		break;
	case BTN_ESC:
		if (SDL_JoystickGetButton(m_joystick[l_iJoystickID], 4) == 1)
			return true;
		break;
	case DPAD_LEFT:
		if (SDL_JoystickGetAxis(m_joystick[l_iJoystickID], 0) == -32768)
			return true;
		break;
	case DPAD_RIGHT:
		if (SDL_JoystickGetAxis(m_joystick[l_iJoystickID], 0) == 32767)
			return true;
		break;
	case DPAD_DOWN:
		if (SDL_JoystickGetAxis(m_joystick[l_iJoystickID], 1) == 32767)
			return true;
		break;
	case DPAD_UP:
		if (SDL_JoystickGetAxis(m_joystick[l_iJoystickID], 1) == -32768)
			return true;
		break;
	}
	//}
	return false;
}

bool Controls::joysticks_removeIndex(int f_iIndex)
{
	if (m_joystick[f_iIndex] != nullptr)
		SDL_JoystickClose(m_joystick[f_iIndex]);
	m_joystick[f_iIndex] = nullptr;
	return true;
}

bool Controls::joysticks_addedIndex(int f_iIndex)
{
	if (m_joystick[f_iIndex] != nullptr)
	{
		cout << "Joystick already loaded in index: " << f_iIndex << endl;
		//joysticks_removeIndex(f_iIndex);
		return false;
	}
		
	m_joystick[f_iIndex] = SDL_JoystickOpen(f_iIndex);
	cout << "Found a joystick in slot: " << f_iIndex << endl;
	cout << "Name: " << SDL_JoystickName(m_joystick[f_iIndex]) << endl;
	cout << "Buttons: " << SDL_JoystickNumButtons(m_joystick[f_iIndex]) << endl;
	//SDL_JoystickGUID l_guid = SDL_JoystickGetGUID(m_joystick[f_iIndex]);
	//cout << "GUID: " << (int)l_guid.data << endl;
	return true;
}

bool Controls::gameController_addedIndex(int f_iIndex)
{
	if (m_gameController[f_iIndex] != nullptr) return false;

	m_gameController[f_iIndex] = SDL_GameControllerOpen(f_iIndex);
	*m_bjoystickIsController[f_iIndex] = true;

	cout << "Found a Game Controller in slot: " << f_iIndex << endl;
	cout << "Name: " << SDL_JoystickName(m_joystick[f_iIndex]) << endl;
	cout << "Buttons: " << SDL_JoystickNumButtons(m_joystick[f_iIndex]) << endl;
	m_joystick[f_iIndex] = SDL_GameControllerGetJoystick(m_gameController[f_iIndex]);
	return true;
}

bool Controls::gameController_removeIndex(int f_iIndex)
{
	if (m_gameController[f_iIndex] != nullptr) return false;
	if(m_joystick[f_iIndex] != nullptr)
		SDL_JoystickClose(m_joystick[f_iIndex]);
	SDL_GameControllerClose(m_gameController[f_iIndex]);
	m_gameController[f_iIndex] = nullptr;
	return true;
}

bool Controls::open_controllerManagement()
{
	if (m_controllerManagement == nullptr)
	{
		m_controllerManagement = new ControllerManagement(m_sb);
		m_controllerManagement->set_playerControllers(m_iPlayerOne, m_iPlayerTwo);
		m_controllerManagement->set_menuActive(&m_bAreInControllerManMenu);
		m_controllerManagement->set_playerTwoControllerTypes(m_iPlayerOneControllerType, m_iPlayerTwoControllerType);
		m_controllerManagement->set_anyKeypressed(&m_bAnyButtonDown);
	}
	initialise_joysticks();

	m_controllerManagement->set_joysticks(m_joystick);
	m_controllerManagement->set_gamecontroller(m_gameController);
	m_controllerManagement->set_joystickIsController(m_bjoystickIsController);

	m_bAreInControllerManMenu = true;
	*m_iPlayerOne = -1;
	*m_iPlayerOneControllerType = 0;
	*m_iPlayerTwo = -1;
	*m_iPlayerTwoControllerType = 0;
	return true;
}

bool Controls::is_anyButtonDown()
{
	int *numkeys = nullptr;
	const Uint8 *state = SDL_GetKeyboardState(numkeys);
	for (int i = 0; i < 100; i++)
	{
		if (m_scancode[i] == NULL) continue;
		if (state[m_scancode[i]])
		{
			return true;
		}
			
	}
	return false;
}

bool Controls::toggle_mouseCursor()
{
	if (m_bMouseCursor)
		m_bMouseCursor = false;
	else
		m_bMouseCursor = true;
	return toggle_mouseCursor(m_bMouseCursor);
}

bool Controls::toggle_mouseCursor(bool f_bNew)
{
	m_bMouseCursor = f_bNew;
	if(m_bMouseCursor)
		SDL_ShowCursor(SDL_ENABLE);
	else
		SDL_ShowCursor(SDL_DISABLE);
	return m_bMouseCursor;
}