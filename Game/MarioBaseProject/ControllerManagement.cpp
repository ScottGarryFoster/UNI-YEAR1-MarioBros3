#include "ControllerManagement.h"



ControllerManagement::ControllerManagement(SpriteBatch *f_sb)
{

	if (f_sb == nullptr)
		cout << "ControllerManagement (constructor): m_controls was nullptr." << endl;
	m_sb = f_sb;
}


ControllerManagement::~ControllerManagement()
{
}

bool ControllerManagement::Draw()
{
	m_sb->Start();

	if (m_sprBackground != nullptr)
	{
		m_sprBackground->RenderToScreen(m_sb->get_renderer());
		Vector2D *l_v2dBackground = new Vector2D(m_sprBackground->get_locationX(), m_sprBackground->get_locationY());
		l_v2dBackground->add_valueX(-1596);
		m_sprBackground->RenderToScreen(m_sb->get_renderer(), l_v2dBackground);
		delete l_v2dBackground;
	}

	if (m_sprBackgroundPanels != nullptr)
		m_sprBackgroundPanels->RenderToScreen(m_sb->get_renderer());

	if (m_sprControllersMedium != nullptr)
	{
		if (m_iLastClickDevice >= 0)
		{
			m_sprControllersMedium->set_singleLocation(0, 6);
			m_sprControllersMedium->RenderToScreen(m_sb->get_renderer());
			m_sprControllersMedium->set_singleLocation(0, 1);
			m_sprControllersMedium->RenderToScreen(m_sb->get_renderer());
			Vector2D *l_v2dBackground = new Vector2D(m_sprControllersMedium->get_locationX(), m_sprControllersMedium->get_locationY());
			for (int i = 2; i < m_iNumberOfControllersSetup; i++)
			{
				l_v2dBackground->add_valueX(183 + 30);
				m_sprControllersMedium->set_singleLocation(0, 6);
				m_sprControllersMedium->RenderToScreen(m_sb->get_renderer(), l_v2dBackground);

				m_sprControllersMedium->set_singleLocation(0, i);
				m_sprControllersMedium->RenderToScreen(m_sb->get_renderer(), l_v2dBackground);
			}
			delete l_v2dBackground;
		}
	}

	if (m_sprControllersBig != nullptr)
	{
		m_sprControllersBig->set_screenSize(366 + m_fFlareMedSizeModify, 366 + m_fFlareMedSizeModify);
		m_sprControllersBig->set_location(645 - (m_fFlareMedSizeModify / 2), 255 - (m_fFlareMedSizeModify / 2));

		if (m_iLastClickDevice == -1)
		{
			Vector2D *l_v2dBackground = new Vector2D(m_sprControllersBig->get_locationX(), m_sprControllersBig->get_locationY() - 100);
			m_sprControllersBig->set_singleLocation(0, 0);
			m_sprControllersBig->RenderToScreen(m_sb->get_renderer(), l_v2dBackground);
			delete l_v2dBackground;
		}
		else
		{
			Vector2D *l_v2dBackground = new Vector2D(m_sprControllersBig->get_locationX(), m_sprControllersBig->get_locationY() + 15);
			m_sprControllersBig->set_singleLocation(0, m_iLastClickDeviceType);
			m_sprControllersBig->RenderToScreen(m_sb->get_renderer(), l_v2dBackground);
			delete l_v2dBackground;
		}
	}

	if (m_sprPlayerOne != nullptr)
	{
		m_sprPlayerOne->set_singleLocation(0, *s_iPlayerOneType);
		m_sprPlayerOne->RenderToScreen(m_sb->get_renderer());
	}

	if (m_sprPlayerTwo != nullptr)
	{
		m_sprPlayerTwo->set_singleLocation(0, *s_iPlayerTwoType);
		m_sprPlayerTwo->RenderToScreen(m_sb->get_renderer());
	}

	if (m_sprMouseController != nullptr)
	{
		if (m_sprMouseController->get_visable())
		{
			m_sprMouseController->set_location(m_iMouseX - (183 / 2), m_iMouseY - (183 / 2));
			m_sprMouseController->RenderToScreen(m_sb->get_renderer());
		}
	}

	if (m_sprMenuButtons != nullptr)
	{
		m_sprMenuButtons->RenderToScreen(m_sb->get_renderer());
	}

	m_sb->End();
	return true;
}

bool ControllerManagement::Update(float f_deltaTime)
{
	if (m_sprBackground == nullptr)load_content();
	SDL_JoystickUpdate();

	update_backgroundLocation(f_deltaTime);
	update_lastPressed(f_deltaTime);
	update_unknownpadRespond(f_deltaTime);
	update_mouseControl(f_deltaTime);
	return true;
}

bool ControllerManagement::update_lastPressed(float f_deltaTime)
{
	if (s_bAnyKeypressed == nullptr) return false;
	
	if (*s_bAnyKeypressed)
	{
		m_iLastClickDeviceType = 0;
		m_iFlareMedBiggerTick = 1000;
		m_iLastClickDevice = -1;
		return true;
	}
	
	int l_iPrevioudClickDevice = m_iLastClickDevice;
	int l_iNewClickDevice = -1;
	int l_iNumberJoysticks = SDL_NumJoysticks();
	if (l_iNumberJoysticks > 10) l_iNumberJoysticks = 10;

	for (int index = 0; index < l_iNumberJoysticks; index++)
	{
		if (m_joystick[index] == nullptr)
		{
			continue;
		}
		for (int j = 0; j < SDL_JoystickNumButtons(m_joystick[index]); j++)
		{
			if (SDL_JoystickGetButton(m_joystick[index], j) == 1)
			{
				l_iNewClickDevice = index;
			}
		}

		if (SDL_JoystickGetAxis(m_joystick[index], 0) == 32767 || SDL_JoystickGetAxis(m_joystick[index], 0) == -32768)
			l_iNewClickDevice = index;
		else if (SDL_JoystickGetAxis(m_joystick[index], 1) == 32767 || SDL_JoystickGetAxis(m_joystick[index], 1) == -32768)
			l_iNewClickDevice = index;

		if(l_iNewClickDevice != -1)
		{
			m_iLastClickDevice = l_iNewClickDevice;
			m_iFlareMedBiggerTick = 1000;
		}

		if (l_iPrevioudClickDevice != m_iLastClickDevice)
			m_iLastClickDeviceType = 5;
	}

	return true;
}

bool ControllerManagement::update_unknownpadRespond(float f_deltaTime)
{
	if (m_iFlareMedBiggerTick <= 0) return true;

	m_iFlareMedBiggerTick -= f_deltaTime;
	m_fFlareMedSizeModify = m_iFlareMedBiggerTick * 0.05f;
	return true;
}

bool ControllerManagement::load_content()
{
	if (m_sprBackground == nullptr)
	{
		m_sprBackground = new Sprite();
		if (!m_sprBackground->LoadTextureFromFile(m_sb->get_renderer(), "Images/GameSelectMain/controllerm_background.png"))
			cout << "ControllerManagement::load_content(): m_sprBackground texture not loaded" << endl;
		m_sprBackground->set_screenSize(1596, 720);
		m_sprBackground->set_singleSize(m_sprBackground->get_actualSizeWidth(), m_sprBackground->get_actualSizeHeight());
	}

	if (m_sprBackgroundPanels == nullptr)
	{
		m_sprBackgroundPanels = new Sprite();
		if (!m_sprBackgroundPanels->LoadTextureFromFile(m_sb->get_renderer(), "Images/GameSelectMain/controllerm_backgroundPanels.png"))
			cout << "ControllerManagement::load_content(): m_sprBackgroundPanels texture not loaded" << endl;
		m_sprBackgroundPanels->set_screenSize(1280, 720);
		m_sprBackgroundPanels->set_singleSize(m_sprBackgroundPanels->get_actualSizeWidth(), m_sprBackgroundPanels->get_actualSizeHeight());
	}

	if (m_sprControllersMedium == nullptr)
	{
		m_sprControllersMedium = new Sprite();
		if (!m_sprControllersMedium->LoadTextureFromFile(m_sb->get_renderer(), "Images/GameSelectMain/controllerm_iconsMedium.png"))
			cout << "ControllerManagement::load_content(): m_controllersMedium texture not loaded" << endl;
		m_sprControllersMedium->set_screenSize(183, 183);
		m_sprControllersMedium->set_singleSize(183, 183);
		m_sprControllersMedium->set_location(416, 130);
	}

	if (m_sprControllersBig == nullptr)
	{
		m_sprControllersBig = new Sprite();
		if (!m_sprControllersBig->LoadTextureFromFile(m_sb->get_renderer(), "Images/GameSelectMain/controllerm_iconsBig.png"))
			cout << "ControllerManagement::load_content(): m_controllersBig texture not loaded" << endl;
		m_sprControllersBig->set_screenSize(366, 366);
		m_sprControllersBig->set_singleSize(366, 366);
		m_sprControllersBig->set_location(645, 255);
		m_sprControllersBig->set_singleLocation(0, 4);
	}

	if (m_sprPlayerOne == nullptr)
	{
		m_sprPlayerOne = new Sprite();
		m_sprPlayerOne->set_textureSelfManageFromPointer(m_sprControllersMedium->get_textureSelfManageFromPointer());
		m_sprPlayerOne->set_screenSize(183, 183);
		m_sprPlayerOne->set_singleSize(183, 183);
		m_sprPlayerOne->set_location(95, 35);
		m_sprPlayerOne->set_singleLocation(0, 0);
	}

	if (m_sprPlayerTwo == nullptr)
	{
		m_sprPlayerTwo = new Sprite();
		m_sprPlayerTwo->set_textureSelfManageFromPointer(m_sprControllersMedium->get_textureSelfManageFromPointer());
		m_sprPlayerTwo->set_screenSize(183, 183);
		m_sprPlayerTwo->set_singleSize(183, 183);
		m_sprPlayerTwo->set_location(95, 350);
		m_sprPlayerTwo->set_singleLocation(0, 0);
	}

	if (m_sprMouseController == nullptr)
	{
		m_sprMouseController = new Sprite();
		m_sprMouseController->set_textureSelfManageFromPointer(m_sprControllersMedium->get_textureSelfManageFromPointer());
		m_sprMouseController->set_screenSize(183, 183);
		m_sprMouseController->set_singleSize(183, 183);
		//m_sprMouseController->set_location(95, 350);
		m_sprMouseController->set_singleLocation(0, 0);
		m_sprMouseController->set_visable(false);
	}

	if (m_sprMenuButtons == nullptr)
	{
		m_sprMenuButtons = new Sprite();
		if (!m_sprMenuButtons->LoadTextureFromFile(m_sb->get_renderer(), "Images/GameSelectMain/SMB_buttons.png"))
			cout << "ControllerManagement::load_content(): m_sprMenuButtons texture not loaded" << endl;
		m_sprMenuButtons->set_screenSize(340, 55);
		m_sprMenuButtons->set_singleSize(340, 55);
		m_sprMenuButtons->set_singleLocation(5, 0);
		m_sprMenuButtons->set_location(15, 620);
	}
	return true;
}

bool ControllerManagement::update_backgroundLocation(float f_deltaTime)
{
	if (m_sprBackground == nullptr) return false;

	m_sprBackground->move_location(f_deltaTime * 0.03f, 0);
	if (m_sprBackground->get_locationX() > 1596)
		m_sprBackground->set_location(m_sprBackground->get_locationX() - 1596, m_sprBackground->get_locationY());

	return true;
}

bool ControllerManagement::update_mouseControl(float f_deltaTime)
{
	SDL_PumpEvents();
	if (SDL_GetMouseState(&m_iMouseX, &m_iMouseY))
	{
		if (SDL_BUTTON(SDL_BUTTON_LEFT))
			m_bMouseDown = true;
		else
			m_bMouseDown = false;
	}
	else
		m_bMouseDown = false;

	if (m_iMouseX == NULL)
		m_iMouseX = -1;

	if (m_iMouseY == NULL)
		m_iMouseY = -1;

	SDL_Rect l_rectMouse = { m_iMouseX,m_iMouseY,1,1 };
	SDL_Rect l_rectContinueButton = { m_sprMenuButtons->get_locationX(), m_sprMenuButtons->get_locationY(), m_sprMenuButtons->get_actualSizeWidth(), m_sprMenuButtons->get_actualSizeHeight() };

	if (!m_bPreviousMouseDown && m_bMouseDown)//First click:
	{
		if (m_iLastClickDevice == -1)
		{
			SDL_Rect l_rectController = { m_sprControllersBig->get_locationX(), m_sprControllersBig->get_locationY(), m_sprControllersBig->get_actualSizeWidth(), m_sprControllersBig->get_actualSizeHeight() };
			if (Utility::ReturnCollision(l_rectController, l_rectMouse))
			{
				m_iClickedController = 0;
				m_sprMouseController->set_visable(true);
				m_sprMouseController->set_singleLocation(0, 0);
			}
			if (m_iClickedController == -1)
				m_sprMouseController->set_visable(false);
		}
		else
		{
			SDL_Rect l_iController = { m_sprControllersMedium->get_locationX(), m_sprControllersMedium->get_locationY(), m_sprControllersMedium->get_actualSizeWidth(), m_sprControllersMedium->get_actualSizeHeight() };
			for (int i = 1; i < m_iNumberOfControllersSetup; i++)
			{
				if (Utility::ReturnCollision(l_iController, l_rectMouse))
				{
					m_iClickedController = i;
					m_sprMouseController->set_visable(true);
					m_sprMouseController->set_singleLocation(0, i);
				}
				l_iController.x += 183 + 30;
			}
			
			if (m_iClickedController == -1)
				m_sprMouseController->set_visable(false);
		}

		//This could have gone in it's own function however I'd rather use the wheel than re-write it
		if (Utility::ReturnCollision(l_rectContinueButton, l_rectMouse))
			m_bClickedDownOnContinue = true;
		else
			m_bClickedDownOnContinue = false;
	}
	else if (m_bPreviousMouseDown && !m_bMouseDown)//Release
	{
		SDL_Rect l_rectPlayer = {m_sprPlayerOne->get_locationX(),m_sprPlayerOne->get_locationY(),m_sprPlayerOne->get_actualSizeWidth(),m_sprPlayerOne->get_actualSizeHeight()};
		if (Utility::ReturnCollision(l_rectPlayer, l_rectMouse))
		{
			*s_iPlayerOne = m_iLastClickDevice;
			*s_iPlayerOneType = m_sprMouseController->get_sourceCol();
			if (m_iLastClickDevice != -1 && *s_iPlayerOne == *s_iPlayerTwo)
			{
				//So you've mapped a controller to both players
				*s_iPlayerTwo = -1;
				*s_iPlayerTwoType = 0;
			}
		}

		l_rectPlayer = { m_sprPlayerTwo->get_locationX(),m_sprPlayerTwo->get_locationY(),m_sprPlayerTwo->get_actualSizeWidth(),m_sprPlayerTwo->get_actualSizeHeight() };
		if (Utility::ReturnCollision(l_rectPlayer, l_rectMouse))
		{
			*s_iPlayerTwo = m_iLastClickDevice;
			*s_iPlayerTwoType = m_sprMouseController->get_sourceCol();
			if (m_iLastClickDevice != -1 && *s_iPlayerOne == *s_iPlayerTwo)
			{
				//So you've mapped a controller to both players
				*s_iPlayerOne = -1;
				*s_iPlayerOneType = 0;
			}
		}

		//This could have gone in it's own function however I'd rather use the wheel than re-write it
		if (Utility::ReturnCollision(l_rectContinueButton, l_rectMouse))
		{
			if (m_bClickedDownOnContinue)
				*s_bMenuActive = false;
		}

		m_sprMouseController->set_visable(false);
		m_iClickedController = -1;
	}

	if (Utility::ReturnCollision(l_rectContinueButton, l_rectMouse))
		m_sprMenuButtons->set_singleLocation(m_sprMenuButtons->get_sourceRow(), 1);
	else
		m_sprMenuButtons->set_singleLocation(m_sprMenuButtons->get_sourceRow(), 0);

	m_bPreviousMouseDown = m_bMouseDown;
	return true;
}