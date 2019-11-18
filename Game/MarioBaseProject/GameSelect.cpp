#include "GameSelect.h"
#include "Controls.h"
#include "SPRITEBATCH.h"
#include "TextureSelfManaged.h"
#include <iostream>

GameSelect::GameSelect(int argc, char* args[]) : GameEngine(argc, args)
{
	m_iGameLoaded = 0;
	GameEngine::GraphicsUpdate();
}


GameSelect::~GameSelect()
{
	delete m_sprGameWallpaper; m_sprGameWallpaper = nullptr;
	delete m_sprMenuBackground; m_sprMenuBackground = nullptr;
	delete m_sprMenuButtons; m_sprMenuButtons = nullptr;

	delete m_gameMario; m_gameMario = nullptr;
}

void GameSelect::LoadContent()//Loads once just before the window loads
{

}

void GameSelect::AfterContent()//Loads after window loads
{
	if (m_iTitleScreenMusic == -1)
		m_iTitleScreenMusic = p_audio->find_musicNeverFail("Audio/Music/Super Mario Odyssey OST - Mushroom Kingdom (Peachs Castle).mp3");

	if (m_iUIClick == -1)
		m_iUIClick = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/LQ_Click_Button.wav");

	if (m_iAcceptSound == -1)
		m_iAcceptSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/FA_Select_Button_1_4.wav");

	if(m_iMessagePopupSound == -1)
		m_iMessagePopupSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Error_Button_05_4.wav");

}

bool GameSelect::GameLoop(float f_deltaTime)
{
	m_fDeltaTime = f_deltaTime;
	if (GameEngine::EventUpdate()) { return true; }//Allows this funciton to close the game
	p_controls->UpdateKeyboard();

	if (!p_controls->Update(f_deltaTime))//Controls takes the leed
	{
		//All this is the controller managment stuff
		p_controls->Draw();
		return false;
	}
	
	if (m_iGameLoaded == 0)
	{
		if(m_sprGameWallpaper == nullptr)
			UpdateLoadContent();

		if (m_editor == nullptr)
		{
			UpdateGameSelect(f_deltaTime);
			p_audio->play_music(m_iTitleScreenMusic);
		}
		else
		{
			if (s_iGameSelectMenu == 0)
			{
				m_editor->Update(f_deltaTime);
				if (m_editor->KillMe())
				{
					delete m_editor; m_editor = nullptr;
					m_bDisableControlMethodIcon = false;//We're coming out of the editors
				}
			}
			else
				update_optionsMenu();
		}
	}
	else if (m_iGameLoaded == 1)
	{
		if (s_iGameSelectMenu == 0)
			m_gameMario->Update(f_deltaTime);
		else
			update_optionsMenu();
	}
		

	/*if (m_gameMario == nullptr)
	{
		m_gameMario = new SuperMarioBros3();
		m_gameMario->set_spriteBatch(p_spriteBatch);
		m_gameMario->LoadContent();
	}*/
	if (m_alertBox != nullptr)
		m_alertBox->Update();

	if (!m_bDisableControlMethodIcon)
		update_controlInputChange(f_deltaTime);
	else
		m_iCicLastSeenFrame = 0;//Ensure we hide any icon

	Draw();
	return false;
}

bool GameSelect::Draw()
{
	/*
		TIPS:
		DRAW DRAWS ON TOP OF WHATEVER IS BEFORE IT.
		DRAW LATER -> DRAW HIGHER;
	*/
	p_spriteBatch->Start();

	if (m_iGameLoaded == 0)
	{
		if (m_editor == nullptr)
			UpdateDraw();
		else
			m_editor->Draw();

		if (s_iGameSelectMenu > 0)
			draw_optionsMenu();
	}
	else if (m_iGameLoaded == 1)
	{
		m_gameMario->Draw();

		if (s_iGameSelectMenu > 0)
			draw_optionsMenu();
	}
	if (m_alertBox != nullptr)
		m_alertBox->Draw();

	draw_controlInputChange();

	p_spriteBatch->End();
	
	return true;
}

bool GameSelect::UpdateLoadContent()
{

	/*
		Keeping this in here. If you want to scale the width or height this is the code:

	float l_fAspectRatio_H = (l_iHeight / l_iWidth); //Used to work out width
	float l_fNewWidth = l_fAspectRatio_H * SCREEN_HEIGHT;


	float l_fAspectRatio_W = (l_iWidth / l_iHeight); //Used to work out height
	float l_fNewHeight = l_fAspectRatio_H * SCREEN_WIDTH;

	then:
	m_spriteGameArt[l_iCurrentSprite]->set_screenSize(l_fNewWidth, SCREEN_HEIGHT);
	*/

	//We no longer scale these things because I don't really want the res to change anymore.
	//These images were made for 1280 by 720

	if (m_sprGameWallpaper == nullptr)
	{
		m_sprGameWallpaper = new Sprite();
		m_sprGameWallpaper->set_screenSize(1920, 773);
		m_sprGameWallpaper->set_singleSize(1920, 773);
		m_sprGameWallpaper->LoadTextureFromFile(p_spriteBatch->get_renderer(), "Images/GameSelectMain/SMB_wallpaper.jpg");
		m_sprGameWallpaper->set_locks(false, false, false, false);
	}

	if (m_sprMenuBackground == nullptr)
	{
		m_sprMenuBackground = new Sprite();
		m_sprMenuBackground->set_screenSize(371, 720);
		m_sprMenuBackground->set_singleSize(371, 720);
		m_sprMenuBackground->LoadTextureFromFile(p_spriteBatch->get_renderer(), "Images/GameSelectMain/SMB_selectionBackground.png");
	}
	
	if (m_sprMenuButtons == nullptr)
	{
		m_sprMenuButtons = new Sprite();
		m_sprMenuButtons->set_screenSize(340, 55);
		m_sprMenuButtons->set_singleSize(340, 55);
		m_sprMenuButtons->LoadTextureFromFile(p_spriteBatch->get_renderer(), "Images/GameSelectMain/SMB_buttons.png");
	}

	if (m_sprCicIcons == nullptr)
	{
		m_sprCicIcons = new Sprite();
		m_sprCicIcons->set_screenSize(70, 70);
		m_sprCicIcons->set_singleSize(70, 70);
		m_sprCicIcons->LoadTextureFromFile(p_spriteBatch->get_renderer(), "Images/GameSelectMain/Inputs.png");
		m_sprCicIcons->set_locks(false, false, false, false);
	}

	if (m_sprControllerManagementIcon == nullptr)
	{
		m_sprControllerManagementIcon = new Sprite();
		m_sprControllerManagementIcon->set_textureSelfManageFromPointer(m_sprCicIcons->get_textureSelfManageFromPointer());
		m_sprControllerManagementIcon->set_screenSize(70, 70);
		m_sprControllerManagementIcon->set_singleSize(70, 70);
		m_sprControllerManagementIcon->set_locks(false, false, false, false);
		m_sprControllerManagementIcon->set_location(SCREEN_WIDTH - 5 - m_sprCicIcons->get_actualSizeWidth(), SCREEN_HEIGHT - 5 - m_sprCicIcons->get_actualSizeHeight());
		m_sprControllerManagementIcon->set_singleLocation(0, 3);
		m_sprControllerManagementIcon->set_visable(false);
	}

	m_bMMOptionReady = false;
	return true;
}

bool GameSelect::UpdateGameSelect(float f_deltaTime)
{
	p_spriteBatch->set_color(0, 0, 0);//Black
	update_backgroundLocation(f_deltaTime);
	if (m_bLEMonMenu) return update_levelEditorMenu();
	update_inputSelection();
	update_controllerManagement();
	return true;
}

bool GameSelect::update_inputSelection()
{
	if (p_controls->get_menuInput() == Controls::miKeyboard || p_controls->get_menuInput() == Controls::miJoystick)
	{
		m_iMouseStateRecorded = 0;
		update_inputKeyboard();
	}
	else if (p_controls->get_menuInput() == Controls::miMouse)
	{
		update_inputMouse();
		update_inputMouseSelection();
	}


	return true;
}

bool GameSelect::update_inputKeyboard()
{
	//Player One always controls the menu
	if (p_controls->isGameKeyDown(Controls::DPAD_DOWN, 1) || p_controls->isJoypadButtonDown(Controls::DPAD_DOWN, 1))
	{
		if (m_bMMOptionReady)
		{
			m_iMMOptionSelected++;
			p_audio->play_soundEffect(m_iUIClick);
		}
		m_bMMOptionReady = false;
	}
	else if (p_controls->isGameKeyDown(Controls::DPAD_UP, 1) || p_controls->isJoypadButtonDown(Controls::DPAD_UP, 1))
	{
		if (m_bMMOptionReady)
		{
			m_iMMOptionSelected--;
			p_audio->play_soundEffect(m_iUIClick);
		}
		m_bMMOptionReady = false;
	}
	else if (p_controls->isGameKeyDown(Controls::BTN_START, 1) || p_controls->isGameKeyDown(Controls::BTN_A, 1) || p_controls->isJoypadButtonDown(Controls::BTN_A, 1) || p_controls->isJoypadButtonDown(Controls::BTN_START, 1))
	{
		if (m_bMMOptionReady)
		{
			update_sucessfulSelection();
		}
		m_bMMOptionReady = false;
	}
	else
		m_bMMOptionReady = true;

	if (m_iMMOptionSelected > 4)
		m_iMMOptionSelected = 0;
	else if (m_iMMOptionSelected < 0)
		m_iMMOptionSelected = 4;
	return true;
}
bool GameSelect::update_sucessfulSelection()
{
	if (m_iMMOptionSelected == 0)
		LoadNewGame(1);
	else if (m_iMMOptionSelected == 1)
		setup_editor(1);
	else if(m_iMMOptionSelected == 3)
		p_controls->open_controllerManagement();
	else if (m_iMMOptionSelected == 4)
		PressQuit();

	p_audio->play_soundEffect(m_iAcceptSound, 1);
	return true;
}

bool GameSelect::update_inputMouse()
{
	if (m_cMouseInfo == nullptr)
		m_cMouseInfo = p_controls->get_mouseInfo();

	if (m_iSelectionLocation == nullptr)
		m_iSelectionLocation = &m_iMMOptionSelected;

	if (m_cMouseInfo->state == Controls::mouseInfo::msNone)
	{
		SDL_Rect l_rectMouse;
		l_rectMouse.x = *m_cMouseInfo->x - 1;
		l_rectMouse.y = *m_cMouseInfo->y - 1;
		l_rectMouse.h = 2;
		l_rectMouse.w = 2;

		m_sprMenuButtons->set_location(20, 325);
		SDL_Rect l_rectButton;
		l_rectButton.h = m_sprMenuButtons->get_actualSizeHeight();
		l_rectButton.w = m_sprMenuButtons->get_actualSizeWidth();
		l_rectButton.x = m_sprMenuButtons->get_locationX();
		l_rectButton.y = m_sprMenuButtons->get_locationY();
		
		bool l_bFOundCollision = false;
		for (int i = 0; i < m_iNumberOfButtons; i++)
		{
			if (Utility::ReturnCollision(l_rectMouse, l_rectButton))
			{
				*m_iSelectionLocation = m_iOptionNumbers[i];

				if (m_iPreviousLocation != i)
					p_audio->play_soundEffect(m_iUIClick, 0);
				m_iPreviousLocation = i;

				l_bFOundCollision = true;
				break;
			}
			l_rectButton.y += 80;
		}
		if (!l_bFOundCollision)
		{
			*m_iSelectionLocation = -1;
			m_iPreviousLocation = -1;
		}

	}
	return true;
}

bool GameSelect::update_inputMouseSelection()
{
	if (m_cMouseInfo == nullptr) return false;
	
	if (m_cMouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if(m_iMouseStateRecorded > 0)//Not the first click since switching
			m_iMouseStateRecorded = 1;
	}
	else if (m_cMouseInfo->state == Controls::mouseInfo::msNone)
	{
		if (m_iMouseStateRecorded == 1)
		{
			update_sucessfulSelection();
		}
		m_iMouseStateRecorded = 2;
	}
	else
		m_iMouseStateRecorded = 2;
}

bool GameSelect::update_backgroundLocation(float f_deltaTime)
{
	if (m_sprGameWallpaper != nullptr)
	{
		m_sprGameWallpaper->move_location(0.03f * f_deltaTime, 0);
		if (m_sprGameWallpaper->get_locationX() > 1920)
			m_sprGameWallpaper->set_location(0, m_sprGameWallpaper->get_locationY());

		if (m_bBackgroundYSway)
		{
			m_sprGameWallpaper->move_location(0, 0.01f * f_deltaTime);
			if (m_sprGameWallpaper->get_locationY() > 0)
			{
				m_sprGameWallpaper->set_location(m_sprGameWallpaper->get_locationX(), 0);
				m_bBackgroundYSway = false;
			}
		}
		else
		{
			m_sprGameWallpaper->move_location(0, (0.01f * f_deltaTime) * -1);
			if (m_sprGameWallpaper->get_locationY() < -53)
			{
				m_sprGameWallpaper->set_location(m_sprGameWallpaper->get_locationX(), -53);
				m_bBackgroundYSway = true;
			}
		}
	}
	return true;
}

bool GameSelect::UpdateDraw()
{
	m_sprGameWallpaper->RenderToScreen(p_spriteBatch->get_renderer());
	Vector2D *l_v2dBackground = new Vector2D(m_sprGameWallpaper->get_locationX(), m_sprGameWallpaper->get_locationY());
	l_v2dBackground->add_valueX(-1920);
	m_sprGameWallpaper->RenderToScreen(p_spriteBatch->get_renderer(), l_v2dBackground);
	delete l_v2dBackground;

	m_sprMenuBackground->RenderToScreen(p_spriteBatch->get_renderer());

	if (m_bLEMonMenu) return draw_levelEditorMenu();

	m_sprMenuButtons->set_location(20, 325);
	for (int i = 0; i < 5; i++)
	{
		if (m_iMMOptionSelected == i)
			m_sprMenuButtons->set_singleLocation(i, 1);
		else
			m_sprMenuButtons->set_singleLocation(i, 0);
		m_sprMenuButtons->RenderToScreen(p_spriteBatch->get_renderer());
		m_sprMenuButtons->move_location(0, 80);
	}

	if (!p_controls->get_areInControllerManMenu() && m_sprControllerManagementIcon != nullptr)
		m_sprControllerManagementIcon->RenderToScreen(p_spriteBatch->get_renderer());
	
	return true;
}

bool GameSelect::LoadNewGame(int f_iGame)
{
	if (f_iGame < 0) f_iGame = m_iGameSelected;

	switch (f_iGame)
	{
	case 1:
		if (m_gameMario == nullptr)
			m_gameMario = new SuperMarioBros3(p_spriteBatch);
		s_iGameSelectMenu = 0;
		m_gameMario->set_controls(p_controls);
		m_gameMario->set_audio(p_audio);
		m_gameMario->set_gameSelectMenu(&s_iGameSelectMenu);
		m_iGameLoaded = 1;
		m_gameMario->LoadContent();
		break;
	default:
		return false;
	}
	//We free resources from the gameselect last because the new game loading could fail in theory.
	if (!FreeResources())
	{
		cout << "GameSelect::LoadNewGame(int f_iGame): Could not free resources." << endl;
		return false;
	}
}

bool GameSelect::FreeResources()
{
	delete m_sprGameWallpaper; m_sprGameWallpaper = nullptr;
	//delete m_sprMenuBackground; m_sprMenuBackground = nullptr;
	//delete m_sprMenuButtons; m_sprMenuButtons = nullptr;

	return true;
}

bool GameSelect::update_optionsMenu()
{
	p_audio->pause_music();
	update_controllerManagement();
	if (m_sprGameWallpaper == nullptr)
		UpdateLoadContent();

	if (m_iSelectionLocation != &m_iOptionsOptionSelected)
	{
		m_iSelectionLocation = &m_iOptionsOptionSelected;
		m_iMouseStateRecorded = 0;
	}

	if (m_alertBox == nullptr)
	{
		if (p_controls->get_menuInput() == Controls::miKeyboard || p_controls->get_menuInput() == Controls::miJoystick)
		{
			m_iMouseStateRecorded = 0;
			update_optionsKeyboard();
		}
		else if (p_controls->get_menuInput() == Controls::miMouse)
		{
			update_inputMouse();
			update_optionsMouse();
		}
	}
	else
	{
		if (m_alertBox->get_enteredSelection() > 0)
		{
			if (m_alertBox->get_enteredSelection() == 2)
			{
				if (s_iGameSelectMenu == 1)//They're in a game:
				{
					m_iGameLoaded = 0;
					delete m_gameMario; m_gameMario = nullptr;
				}
				else//They're in a game instance like a level editor
				{
					delete m_editor; m_editor = nullptr;
					m_bDisableControlMethodIcon = false;//We're coming out of the editors
				}
				s_iGameSelectMenu = 0;
				load_mainMenuMouseOptions();
			}
			else if (m_alertBox->get_enteredSelection() == 3)
			{
				PressQuit();
			}

			delete m_alertBox; m_alertBox = nullptr;
		}
	}

	return true;
}

bool GameSelect::update_optionsKeyboard()
{
	if (m_alertBox == nullptr)//We'll be using this for quit
	{
		if (p_controls->isGameKeyDown(Controls::DPAD_DOWN, 1) || p_controls->isJoypadButtonDown(Controls::DPAD_DOWN, 1))
		{
			if (m_bMMOptionReady)
			{
				m_iOptionsOptionSelected++;
				p_audio->play_soundEffect(m_iUIClick);
			}
			m_bMMOptionReady = false;
		}
		else if (p_controls->isGameKeyDown(Controls::DPAD_UP, 1) || p_controls->isJoypadButtonDown(Controls::DPAD_UP, 1))
		{
			if (m_bMMOptionReady)
			{
				m_iOptionsOptionSelected--;
				p_audio->play_soundEffect(m_iUIClick);
			}
			m_bMMOptionReady = false;
		}
		else if (p_controls->isGameKeyDown(Controls::BTN_START, 1) || p_controls->isGameKeyDown(Controls::BTN_A, 1) || p_controls->isJoypadButtonDown(Controls::BTN_A, 1) || p_controls->isJoypadButtonDown(Controls::BTN_START, 1))
		{
			if (m_bMMOptionReady)
				update_optionsSelection();
			m_bMMOptionReady = false;
		}
		else
			m_bMMOptionReady = true;

		if (m_iOptionsOptionSelected > 3)
			m_iOptionsOptionSelected = 0;
		else if (m_iOptionsOptionSelected < 0)
			m_iOptionsOptionSelected = 3;
	}
	
	return true;
}

bool GameSelect::update_optionsSelection()
{
	if (m_iOptionsOptionSelected == 0)
	{
		s_iGameSelectMenu = 0;
		if (p_audio != nullptr && m_iAcceptSound > -1)
			p_audio->play_soundEffect(m_iAcceptSound, 1);
		if (p_audio != nullptr)
			p_audio->resume_music();
	}
	else if (m_iOptionsOptionSelected == 1)
	{
		if(p_audio != nullptr && m_iMessagePopupSound > -1)
			p_audio->play_soundEffect(m_iMessagePopupSound, 1);

		m_alertBox = new AlertBox(p_controls, p_spriteBatch);
		m_alertBox->set_title("Alert");
		m_alertBox->set_content("Your progress will not be saved if you return to the menu. ||Are you sure?");
		m_alertBox->set_leftButton("Return to game");//Good One
		m_alertBox->set_rightButton("Quit to Main Menu");//Bad One
		m_alertBox->set_audio(p_audio);
		//Left - 1, Right - 2
	}
	else if (m_iOptionsOptionSelected == 3)
	{
		if (p_audio != nullptr && m_iMessagePopupSound > -1)
			p_audio->play_soundEffect(m_iMessagePopupSound, 1);

		m_alertBox = new AlertBox(p_controls, p_spriteBatch);
		m_alertBox->set_title("Alert");
		m_alertBox->set_content("Your progress will not be saved if you quit ||Are you sure?");
		m_alertBox->set_leftButton("Return to game");//Good One
		m_alertBox->set_rightButton("Close Game");//Bad One
		m_alertBox->set_responceRight(3);
		m_alertBox->set_audio(p_audio);
		//Left - 1, Right - 3
	}
	return true;
}

bool GameSelect::update_optionsMouse()
{
	if (m_cMouseInfo == nullptr) return false;

	if (m_cMouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (m_iMouseStateRecorded > 0)//Not the first click since switching
			m_iMouseStateRecorded = 1;
	}
	else if (m_cMouseInfo->state == Controls::mouseInfo::msNone)
	{
		if (m_iMouseStateRecorded == 1)
		{
			update_optionsSelection();
		}
		m_iMouseStateRecorded = 2;
	}
	else
		m_iMouseStateRecorded = 2;
	return true;
}

bool GameSelect::draw_optionsMenu()
{
	m_sprMenuBackground->RenderToScreen(p_spriteBatch->get_renderer());

	m_sprMenuButtons->set_location(20, 325);
	for (int i = 0; i < 3; i++)
	{
		if (m_iOptionsOptionSelected == i)
			m_sprMenuButtons->set_singleLocation(i + 5, 1);
		else
			m_sprMenuButtons->set_singleLocation(i + 5, 0);
		m_sprMenuButtons->RenderToScreen(p_spriteBatch->get_renderer());
		m_sprMenuButtons->move_location(0, 80);
	}
	if (m_iOptionsOptionSelected == 3)
		m_sprMenuButtons->set_singleLocation(4, 1);
	else
		m_sprMenuButtons->set_singleLocation(4, 0);
	m_sprMenuButtons->RenderToScreen(p_spriteBatch->get_renderer());

	if (!p_controls->get_areInControllerManMenu() && m_sprControllerManagementIcon != nullptr)
		m_sprControllerManagementIcon->RenderToScreen(p_spriteBatch->get_renderer());
	return true;
}

bool GameSelect::draw_levelEditorMenu()
{
	m_sprMenuButtons->set_location(20, 325);
	for (int i = 0; i < 3; i++)
	{
		if (m_iLEMOptionSelected == i)
			m_sprMenuButtons->set_singleLocation(i + 8, 1);
		else
			m_sprMenuButtons->set_singleLocation(i + 8, 0);
		m_sprMenuButtons->RenderToScreen(p_spriteBatch->get_renderer());
		m_sprMenuButtons->move_location(0, 80);
	}
	if (m_iLEMOptionSelected == 3)
		m_sprMenuButtons->set_singleLocation(6, 1);
	else
		m_sprMenuButtons->set_singleLocation(6, 0);
	m_sprMenuButtons->RenderToScreen(p_spriteBatch->get_renderer());
	return true;
}

bool GameSelect::update_levelEditorMenu()
{
	if (m_iSelectionLocation != &m_iLEMOptionSelected)
	{
		m_iSelectionLocation = &m_iLEMOptionSelected;
		m_iMouseStateRecorded = 0;
	}

	if (p_controls->get_menuInput() == Controls::miKeyboard)
	{
		m_iMouseStateRecorded = 0;
		update_levelEditorKeyboard();
	}
	else if (p_controls->get_menuInput() == Controls::miMouse)
	{
		update_inputMouse();
		update_levelEditorMouse();
	}


	return true;
}

bool GameSelect::update_levelEditorKeyboard()
{
	if (p_controls->isGameKeyDown(Controls::DPAD_DOWN, 1))
	{
		if (m_iLEMCanPress)
			m_iLEMOptionSelected++;
		m_iLEMCanPress = false;
	}
	else if (p_controls->isGameKeyDown(Controls::DPAD_UP, 1))
	{
		if (m_iLEMCanPress)
			m_iLEMOptionSelected--;
		m_iLEMCanPress = false;
	}
	else if (p_controls->isGameKeyDown(Controls::BTN_A, 1) || p_controls->isGameKeyDown(Controls::BTN_START, 1))
	{
		if (m_iLEMCanPress)
		{
			update_levelEditorSelection();
		}
		m_iLEMCanPress = false;
	}
	else
		m_iLEMCanPress = true;

	if (m_iLEMOptionSelected > 3)
		m_iLEMOptionSelected = 0;
	else if (m_iLEMOptionSelected < 0)
		m_iLEMOptionSelected = 3;
	return true;
}

bool GameSelect::update_levelEditorMouse()
{
	if (m_cMouseInfo == nullptr) return false;

	if (m_cMouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (m_iMouseStateRecorded > 0)//Not the first click since switching
			m_iMouseStateRecorded = 1;
	}
	else if (m_cMouseInfo->state == Controls::mouseInfo::msNone)
	{
		if (m_iMouseStateRecorded == 1)
		{
			update_levelEditorSelection();
		}
		m_iMouseStateRecorded = 2;
	}
	else
		m_iMouseStateRecorded = 2;

	return true;
}

bool GameSelect::update_levelEditorSelection()
{
	if (m_iLEMOptionSelected == 0)
	{
		setup_editor(m_iLEMOptionSelected);
	}
	else if (m_iLEMOptionSelected == 1)
	{
		setup_editor(m_iLEMOptionSelected);
	}
	else if (m_iLEMOptionSelected == 3)//MainMenu
	{
		load_mainMenuMouseOptions();
		m_bLEMonMenu = false;
	}
	return true;
}

bool GameSelect::setup_editor(int f_iType)
{
	switch (f_iType)
	{
	case 0:
		if(m_editor == nullptr)
			m_editor = new Platformer_TilesetEditor(p_spriteBatch, p_audio, &s_iGameSelectMenu);
		break;
	case 1:
		if (m_editor == nullptr)
			m_editor = new LevelEditor(p_spriteBatch, p_audio, &s_iGameSelectMenu);
		break;
	}
	if (m_editor != nullptr)
	{
		m_editor->set_controls(p_controls);
	}
	m_bDisableControlMethodIcon = true;//We use a combo of mouse and keyboard in these 'games'
	return false;
}

bool GameSelect::load_mainMenuMouseOptions()
{
	for (int i = 0; i < 5; i++)
		m_iOptionNumbers[i] = i;
	m_iNumberOfButtons = 5;

	m_iSelectionLocation = &m_iMMOptionSelected;
	return true;
}

bool GameSelect::update_controlInputChange(float f_deltaTime)
{
	if (m_miCicLastSeen != p_controls->get_menuInput())
	{
		if (m_iCicLastSeenFrame == -1 || m_miCicDisplaying != p_controls->get_menuInput())
		{
			m_iCicLastSeenFrame = 2000;
			m_miCicDisplaying = p_controls->get_menuInput();
			switch (p_controls->get_menuInput())
			{
			case Controls::miMouse:
				m_sprCicIcons->set_singleLocation(0, 0);
				break;
			case Controls::miKeyboard:
				m_sprCicIcons->set_singleLocation(0, 1);
				break;
			case Controls::miJoystick:
				m_sprCicIcons->set_singleLocation(0, 2);
				break;
			}
			m_sprCicIcons->set_location(SCREEN_WIDTH - 5 - m_sprCicIcons->get_actualSizeWidth(), 5);
		}

		m_iCicLastSeenFrame -= f_deltaTime;
		if (m_iCicLastSeenFrame <= -1)
		{
			m_miCicLastSeen = p_controls->get_menuInput();
			m_iCicLastSeenFrame = -1;
		}
	}
	if (m_miCicLastSeen == p_controls->get_menuInput())
		if (m_iCicLastSeenFrame > -1)
		{
			m_iCicLastSeenFrame = -1;
			m_miCicLastSeen = Controls::miUnknown;
		}
			
			
			
	return true;
}

bool GameSelect::draw_controlInputChange()
{
	if (m_iCicLastSeenFrame > 0)
	{
		m_sprCicIcons->RenderToScreen(p_spriteBatch->get_renderer());
	}
	return true;
}

bool GameSelect::update_controllerManagement()
{
	if (p_controls->get_areInControllerManMenu()) return false;
	Controls::mouseInfo *l_cmiInfo = p_controls->get_mouseInfo();

	if (l_cmiInfo->state == Controls::mouseInfo::msLeft)
		m_bControllerLeftClick = true;
	else
	{
		if (m_bControllerLeftClick == true)
		{
			SDL_Rect l_rectMouse = { *l_cmiInfo->x, *l_cmiInfo->y, 1, 1 };
			SDL_Rect l_rectClickableSprite = { m_sprControllerManagementIcon->get_locationX(), m_sprControllerManagementIcon->get_locationY(), m_sprControllerManagementIcon->get_actualSizeWidth(), m_sprControllerManagementIcon->get_actualSizeHeight() };
			if(Utility::ReturnCollision(l_rectMouse, l_rectClickableSprite))
				p_controls->open_controllerManagement();
		}
		m_bControllerLeftClick = false;
	}
	m_sprControllerManagementIcon->set_visable(true);

	return true;
}