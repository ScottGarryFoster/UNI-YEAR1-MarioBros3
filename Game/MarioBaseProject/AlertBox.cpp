#include "AlertBox.h"



AlertBox::AlertBox(Controls *f_controls, SpriteBatch *f_sb)
{
	m_controls = f_controls;
	m_sb = f_sb;
}


AlertBox::~AlertBox()
{
	delete m_sprBlackfade;
	delete m_sprAlertBackground;
	delete m_lettersTitle;
	delete m_lettersContent;
	delete m_sprButtons;
	delete m_lettersLeftButton;
	delete m_lettersRightButton;
}


bool AlertBox::Update()
{
	if (m_sprBlackfade == nullptr)
		LoadContent();

	if (m_controls != nullptr)
		update_input();
	return true;
}

bool AlertBox::Draw()
{
	if (m_sprBlackfade != nullptr)
		m_sprBlackfade->RenderToScreen(m_sb->get_renderer());

	if (m_sprAlertBackground != nullptr)
		m_sprAlertBackground->RenderToScreen(m_sb->get_renderer());

	if (m_lettersTitle != nullptr)
		m_lettersTitle->RenderAsLetters(m_sb->get_renderer(), nullptr, m_sTitle);
	
	if (m_lettersContent != nullptr)
		m_lettersContent->RenderAsLetters(m_sb->get_renderer(), nullptr, m_sContent);

	if (m_sprButtons != nullptr)
	{
		if (m_iOptionSelected == 0)
			m_sprButtons->set_singleLocation(0, 0);
		else
			m_sprButtons->set_singleLocation(1, 0);
		
		m_sprButtons->RenderToScreen(m_sb->get_renderer());

		if (m_iOptionSelected == 1)
			m_sprButtons->set_singleLocation(0, 0);
		else
			m_sprButtons->set_singleLocation(2, 0);

		m_sprButtons->RenderToScreen(m_sb->get_renderer(), &m_v2dSecondButton);

		if (m_lettersLeftButton != nullptr)
			m_lettersLeftButton->RenderAsLetters(m_sb->get_renderer(), nullptr, m_sLeftButton);

		if (m_lettersRightButton != nullptr)
			m_lettersRightButton->RenderAsLetters(m_sb->get_renderer(), nullptr, m_sRightButton);
	}

	return true;
}

bool AlertBox::LoadContent()
{
	if (m_sprBlackfade == nullptr)
	{
		m_sprBlackfade = new Sprite();
		m_sprBlackfade->LoadTextureFromFile(m_sb->get_renderer(), "Images/AlertBox/Black75.png");
		m_sprBlackfade->set_screenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		m_sprBlackfade->set_singleSize(10, 10);

		m_sprAlertBackground = new Sprite();
		//If we have other backgrounds:
		m_sprAlertBackground->LoadTextureFromFile(m_sb->get_renderer(), "Images/AlertBox/AlertBackground_QuestionMark.png");
		m_sprAlertBackground->set_screenSize(493, 281);
		m_sprAlertBackground->set_singleSize(493, 281);
		m_sprAlertBackground->set_location(400, 225);

		m_lettersTitle = new SprLetters();
		//If we have other backgrounds:
		m_lettersTitle->LoadTextureFromFile(m_sb->get_renderer(), "Images/Mario3/Letters.png");
		m_lettersTitle->set_singleSize(16, 16);
		m_lettersTitle->set_lettersLocation(0, 0);
		m_lettersTitle->set_numbersLocation(1, 0);
		m_lettersTitle->set_location(m_sprAlertBackground->get_locationX() + 20, m_sprAlertBackground->get_locationY() + 16);

		m_lettersContent = new SprLetters();
		m_lettersContent->set_textureFromPointer(m_lettersTitle->get_texturePointer(), "Images/Mario3/Letters.png");
		m_lettersContent->set_singleSize(16, 16);
		m_lettersContent->set_lettersLocation(0, 0);
		m_lettersContent->set_numbersLocation(1, 0);
		m_lettersContent->set_location(m_lettersTitle->get_locationX(), m_lettersTitle->get_locationY() + 50);
		m_lettersContent->set_lineWidth(23);

		m_sprButtons = new Sprite();
		m_sprButtons->LoadTextureFromFile(m_sb->get_renderer(), "Images/AlertBox/Buttons.png");
		m_sprButtons->set_singleSize(178, 47);
		m_sprButtons->set_location(m_sprAlertBackground->get_locationX() + 20, m_sprAlertBackground->get_locationY() + 220);
		m_v2dSecondButton.set_value(m_sprAlertBackground->get_locationX() + 300, m_sprButtons->get_locationY());

		m_lettersLeftButton = new SprLetters();
		//If we have other backgrounds:
		m_lettersLeftButton->set_textureFromPointer(m_lettersTitle->get_texturePointer(), "Images/Mario3/Letters.png");
		m_lettersLeftButton->set_singleSize(16, 16);
		m_lettersLeftButton->set_lettersLocation(0, 0);
		m_lettersLeftButton->set_numbersLocation(1, 0);
		m_lettersLeftButton->set_location(m_sprButtons->get_locationX() + 10, m_sprButtons->get_locationY() + 3);
		m_lettersLeftButton->set_lineWidth(7);

		m_lettersRightButton = new SprLetters();
		//If we have other backgrounds:
		m_lettersRightButton->set_textureFromPointer(m_lettersTitle->get_texturePointer(), "Images/Mario3/Letters.png");
		m_lettersRightButton->set_singleSize(16, 16);
		m_lettersRightButton->set_lettersLocation(0, 0);
		m_lettersRightButton->set_numbersLocation(1, 0);
		m_lettersRightButton->set_location(m_sprButtons->get_locationX() + 290, m_sprButtons->get_locationY() + 3);
		m_lettersRightButton->set_lineWidth(7);
	}
	if (m_audio != nullptr)
	{
		//Audio
		if (m_iUIClickSound == -1)
			m_iUIClickSound = m_audio->add_soundEffect("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/LQ_Click_Button.wav");

		if (m_iAcceptSound == -1)
			m_iAcceptSound = m_audio->add_soundEffect("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/FA_Select_Button_1_4.wav");
	}
	return true;
}

bool AlertBox::update_input()
{
	if (m_controls->get_menuInput() == Controls::miKeyboard)
	{
		m_iMouseStateRecorded = 0;
		update_keyboardInput();
	}
	else if (m_controls->get_menuInput() == Controls::miMouse)
	{
		update_mouseInput();
	}
	return true;
}
bool AlertBox::update_keyboardInput()
{
	if (m_controls->isGameKeyDown(Controls::DPAD_LEFT, 1))
	{
		if (m_bReadyForInput)
		{
			m_iOptionSelected--;
			m_audio->play_soundEffect(m_iUIClickSound, 0);
		}
		m_bReadyForInput = false;
	}
	else if (m_controls->isGameKeyDown(Controls::DPAD_RIGHT, 1))
	{
		if (m_bReadyForInput)
		{
			m_iOptionSelected++;
			m_audio->play_soundEffect(m_iUIClickSound, 0);
		}
		m_bReadyForInput = false;
	}
	else if (m_controls->isGameKeyDown(Controls::BTN_A, 1) || m_controls->isGameKeyDown(Controls::BTN_START, 1))
	{
		if (m_bReadyForInput)
		{
			update_selectionInput();
		}
		m_bReadyForInput = false;
	}
	else
		m_bReadyForInput = true;

	if (m_iOptionSelected < 0)
		m_iOptionSelected = 1;
	else if (m_iOptionSelected > 1)
		m_iOptionSelected = 0;

	return true;
}

bool AlertBox::update_selectionInput()
{
	switch (m_iOptionSelected)
	{
	case 1:
		m_iEnteredSelection = m_iReponceRight;
		break;
	default:
	case 0:
		m_iEnteredSelection = m_iReponceLeft;
		break;
	}
	if (m_audio != nullptr && m_iAcceptSound > -1)
		m_audio->play_soundEffect(m_iAcceptSound, 1);
	return true;
}

bool AlertBox::update_mouseInput()
{
	if (m_cMouseInfo == nullptr)
	{
		m_cMouseInfo = m_controls->get_mouseInfo();
		m_iMouseStateRecorded = 0;
	}

	if (m_cMouseInfo->state == Controls::mouseInfo::msNone)
	{
		SDL_Rect l_rectMouse;
		l_rectMouse.x = *m_cMouseInfo->x - 1;
		l_rectMouse.y = *m_cMouseInfo->y - 1;
		l_rectMouse.h = 2;
		l_rectMouse.w = 2;

		SDL_Rect l_rectButton;
		l_rectButton.h = m_sprButtons->get_actualSizeHeight();
		l_rectButton.w = m_sprButtons->get_actualSizeWidth();
		l_rectButton.x = m_sprButtons->get_locationX();
		l_rectButton.y = m_sprButtons->get_locationY();

		bool l_bFOundCollision = false;
			
		if (Utility::ReturnCollision(l_rectMouse, l_rectButton))
		{
			m_iOptionSelected = 0;
			if(m_audio != nullptr && m_iPreviousOptionSelected != m_iOptionSelected)
				m_audio->play_soundEffect(m_iUIClickSound, 0);

			l_bFOundCollision = true;
		}
		l_rectButton.x = m_v2dSecondButton.m_iX;
		l_rectButton.y = m_v2dSecondButton.m_iY;
		if (Utility::ReturnCollision(l_rectMouse, l_rectButton))
		{
			m_iOptionSelected = 1;
			if (m_audio != nullptr &&m_iPreviousOptionSelected != m_iOptionSelected)
				m_audio->play_soundEffect(m_iUIClickSound, 0);

			l_bFOundCollision = true;
		}

		if (!l_bFOundCollision)
			m_iOptionSelected = -1;

		m_iPreviousOptionSelected = m_iOptionSelected;
	}
	
	if (m_cMouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (m_iMouseStateRecorded > 0)//Not the first click since switching
			m_iMouseStateRecorded = 1;
	}
	else if (m_cMouseInfo->state == Controls::mouseInfo::msNone)
	{
		if (m_iMouseStateRecorded == 1)
		{
			update_selectionInput();
			m_bReadyForInput = false;
		}
		m_iMouseStateRecorded = 2;
	}
	else
		m_iMouseStateRecorded = 2;

	return true;
}