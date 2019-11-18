#include "LevelEditor.h"



LevelEditor::LevelEditor(SpriteBatch *f_sb, Audio *f_audio, int *f_iSelectMenu) : GameInstance(f_sb, f_audio, f_iSelectMenu)
{
	
}


LevelEditor::~LevelEditor()
{
	p_controls->toggle_mouseCursor(true);
	delete m_fileSelectorMenu;
	delete m_fileSelectorCompletePackage;
	
	delete m_tmTilemap;
	delete m_cameraWhole;
	delete m_panelUI;
	delete m_customiseUI;
	delete m_topBarBackground;
	delete m_alertBox;
	delete m_sprMouseCursor;
	delete m_sprTools;
	delete m_llTilesInTopBar;
	delete m_sprTiles;
	//delete s_llTileInformation//Should be deleted along side tilemap
	delete m_ttFETilemapLocation;
	delete m_llFEFlags;
	delete m_sprFlagBackground;
	//delete m_llFlagTilemapEditing; //Only links to tilemap I don't create it
	delete m_sprSaveButton;
	delete m_sprPlayButton;
	delete m_level;
	delete m_sprMario;
	delete m_sprWorldSizeBackground;
	delete m_sprWorldIcons;
	delete m_sprEndOfWorldPole;
	delete m_sprMenuButton;
	delete m_tooltip;
}

bool LevelEditor::Update(float f_deltatime)
{
	if (m_fileSelectorCompletePackage == nullptr) m_fileSelectorCompletePackage = new FileSelectorMenu::FileSelectorCompletePackage();
	if (!m_fileSelectorCompletePackage->m_bComplete)
	{
		p_controls->toggle_mouseCursor(true);
		SelectFile(f_deltatime);
		return true;
	}
	else if (!m_fileSelectorCompletePackage->m_bConfirm)
	{
		KillMeNow();
	}
	else if (m_level != nullptr)
	{
		m_level->Update(f_deltatime);
		update_play();
	}
	else if(m_level == nullptr)
	{
		p_controls->toggle_mouseCursor(false);
		if (m_alertBox != nullptr)
			return update_alertbox(f_deltatime);

		if(m_fileSelectorMenu != nullptr)delete m_fileSelectorMenu; m_fileSelectorMenu = nullptr;//Save a little memory
		
		if (!m_bDebug)
		{
			cout << "m_sTopFolderLocation: " << m_fileSelectorCompletePackage->m_sTopFolderLocation << endl;
			cout << "m_sTopFolderSelection: " << m_fileSelectorCompletePackage->m_sTopFolderSelection << endl;
			cout << "m_sSubFolderLocation: " << m_fileSelectorCompletePackage->m_sSubFolderLocation << endl;
			cout << "m_sSubFolderSelection: " << m_fileSelectorCompletePackage->m_sSubFolderSelection << endl;
		}
		m_bDebug = true;

		main_update(f_deltatime);
	}
		
	return true;
}

bool LevelEditor::mouse_updateCursor()
{
	if (m_sprMouseCursor == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	//m_sprMouseCursor
	LevelEditorCursors l_lecLocal = (LevelEditorCursors)m_leTool;
	if (m_leSelection != lesSingle)
	{
		l_lecLocal = (LevelEditorCursors)((int)m_leSelection + m_letToolsCount - 1);
	}
	if (m_uiStage != leuisNoSubMenu)
		l_lecLocal = lecPointer;
	switch (l_lecLocal)
	{
	case lecMove:
		m_sprMouseCursor->set_singleLocation(2, 0);
		break;
	case lecPen:
		m_sprMouseCursor->set_singleLocation(2, 1);
		break;
	case lecEraser:
		m_sprMouseCursor->set_singleLocation(2, 2);
		break;
	case lecFlag:
		m_sprMouseCursor->set_singleLocation(2, 3);
		break;
	case lecEyedrop:
		m_sprMouseCursor->set_singleLocation(2, 4);
		break;
	case lecArea:
		m_sprMouseCursor->set_singleLocation(2, 5);
		break;
	case lecPointer:
		m_sprMouseCursor->set_singleLocation(2, 6);
		break;
	}
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
		m_sprMouseCursor->set_singleLocation(3, m_sprMouseCursor->get_sourceCol());

	m_sprMouseCursor->set_location(*l_mouseInfo->x, *l_mouseInfo->y);

	return true;
}

bool LevelEditor::Draw()
{
	if (m_fileSelectorCompletePackage == nullptr) m_fileSelectorCompletePackage = new FileSelectorMenu::FileSelectorCompletePackage();
	if (!m_fileSelectorCompletePackage->m_bComplete)
	{
		SelectFileDraw();
		return true;
	}
	else if (m_level != nullptr)
	{
		m_level->Draw();
		draw_play();
	}
	else if (m_level == nullptr)
	{
		main_draw();
		if (m_alertBox != nullptr)
			m_alertBox->Draw();
	}
	return true;
}

bool LevelEditor::SelectFile(float f_deltatime)
{
	if (m_fileSelectorMenu == nullptr)
	{
		m_fileSelectorMenu = new FileSelectorMenu(p_sb, p_audio, &m_sFileStructure, m_fileSelectorCompletePackage);
		if (p_controls != nullptr)
			m_fileSelectorMenu->set_controls(p_controls);
	}
	m_fileSelectorMenu->Update(f_deltatime);
	return true;
}
bool LevelEditor::SelectFileDraw()
{
	if (m_fileSelectorMenu == nullptr) return false;
	m_fileSelectorMenu->Draw();
	return true;
}

bool LevelEditor::main_draw()
{
	if (m_tmTilemap != nullptr)
	{
		m_tmTilemap->Draw();
		//m_tmTilemap->Draw_specialBlocks();
	}
	if (m_sprEndOfWorldPole != nullptr)
		m_sprEndOfWorldPole->RenderToScreen(p_sb->get_renderer());

	if (m_sprMario != nullptr)
		m_sprMario->RenderToScreen(p_sb->get_renderer());
		
	if (m_bShowUI && m_panelUI != nullptr)// && (m_uiStage == leuisNoSubMenu || m_uiStage == leuisFlagScreen))
		m_panelUI->RenderToScreen(p_sb->get_renderer());

	if (m_bShowUI && m_sprTools != nullptr)
		draw_tools();

	if (m_bShowUI && m_sprPlayButton != nullptr)
		m_sprPlayButton->RenderToScreen(p_sb->get_renderer());

	if (m_bShowUI && m_sprSaveButton != nullptr)
		draw_save();

	if (m_bShowUI && m_sprWorldSizeBackground != nullptr)
		draw_worldSize();

	if (m_bShowUI && m_sprMenuButton != nullptr)
		m_sprMenuButton->RenderToScreen(p_sb->get_renderer());

	if (m_sprFlagBackground != nullptr)
		main_drawFlagScreen();

	if(m_bShowUI && m_topBarBackground != nullptr)
		main_topBarDraw();

	if (m_iTileselected > -1)
		draw_topBarElements(m_iTileSelectedRow - 1, m_iTileselected, m_v2dSelectedLocation.m_iX, m_v2dSelectedLocation.m_iY);

	if (m_bShowUI && m_tooltip)
		m_tooltip->Draw();

	if (m_sprMouseCursor != nullptr)
		m_sprMouseCursor->RenderToScreen(p_sb->get_renderer());
	return true;
}

bool LevelEditor::main_update(float f_deltatime)
{
	if (p_audio != nullptr)
		p_audio->play_music(m_iDesignMusic);

	if (m_tmTilemap == nullptr)
		main_loadContent(f_deltatime);

	m_tmTilemap->Update(f_deltatime);

	if (m_bShowUI && m_tooltip)
		m_tooltip->Update(f_deltatime);
	
	keyboard_shortcuts();
	update_tooltips();
	update_menuButton();
	update_tools();
	update_play();
	update_worldSize();
	control_universalActions(f_deltatime);
	control_movementOfWorld(f_deltatime);
	control_topBarInteraction(f_deltatime);
	control_drawToTilemap(f_deltatime);
	control_eraser();
	control_blobber();
	control_flagEdit(f_deltatime);
	update_save();
	update_flagScreen();
	drag_playerPosition();
	mouse_updateCursor();

	if (p_controls != nullptr && !m_bFileDebug)
	{
		if (p_controls->isKeyDown(Controls::KeyboardKey::Key_A))
		{
			m_bFileDebug = m_tmTilemap->write_tilemapToFile("GameFiles/Mario3/Tilemaps/test");
			if (!m_bFileDebug) cout << "FALSE" << endl;
		}
	}
	return true;
}

bool LevelEditor::main_loadContent(float f_deltatime)
{
	p_sb->set_color(156, 252, 240);//Blue
	if (m_cameraWhole == nullptr)
	{
		//CameraWhole:
		m_cameraWhole = new Camera();
		m_cameraWhole->set_worldHeight(1650);
		m_cameraWhole->set_worldWidth(1920 * 10);
		m_cameraWhole->set_lockToWorld(true);
		m_cameraWhole->set_screenX(0);
		m_cameraWhole->set_screenWidth(1280);
		m_cameraWhole->set_screenY(0);
		m_cameraWhole->set_screenHeight(720);
		m_cameraWhole->set_visable(true);
		m_cameraWhole->set_isLevelEditor(true);
	}

	int l_iCameraLowestY = 0;
	if (m_tmTilemap == nullptr)
	{
		//Setup Tilemap:
		m_tmTilemap = new Tilemap();
		m_tmTilemap->set_spritebatch(p_sb);
		m_tmTilemap->set_controls(p_controls);
		string l_sName = m_fileSelectorCompletePackage->m_sSubFolderLocation + "/" + m_fileSelectorCompletePackage->m_sSubFolderSelection;
		m_tmTilemap->load_tilemap(Tilemap::FromFile, l_sName);
		//cout << "load_tilemap: " << l_sName << endl;//If you want to debug
		m_tmTilemap->set_camera(m_cameraWhole, true);
		l_sName = m_fileSelectorCompletePackage->m_sTopFolderLocation + "/" + m_fileSelectorCompletePackage->m_sTopFolderSelection;
		m_tmTilemap->load_tileset(Tilemap::FromFile, l_sName);
		//cout << "load_tileset: " << l_sName << endl;//If you want to debug

		//Camera default is the lowest point of the tileset
		m_tmTilemap->update_cameraYLowest();
		l_iCameraLowestY = m_tmTilemap->get_cameraYLowest();
		m_cameraWhole->set_yLowest(l_iCameraLowestY);
		m_cameraWhole->set_y((float)l_iCameraLowestY);
		//m_tmTilemap->debug_dumpTilemap();//If you want to debug
		
		m_tmTilemap->set_renderSpecialBlocks(false);//We want to render special blocks as blocks as we'll be overiding them and allowing deletion
	}

	if (m_sprTiles == nullptr)
	{
		m_sprTiles = new Sprite();
		m_sprTiles->set_textureSelfManageFromPointer(m_tmTilemap->get_textureSelfManaged());
		m_sprTiles->set_singleSize(16, 16);
		m_sprTiles->set_scaleSize(m_tmTilemap->get_scaleSize());
	}

	if (m_panelUI == nullptr)
	{
		m_panelUI = new Sprite();
		if (!m_panelUI->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/noSubMenu.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/noSubMenu.png" << endl;
		m_panelUI->set_singleSize(1280, 720);
		m_panelUI->set_screenSize(1280, 720);
	}

	if (m_customiseUI == nullptr)
	{
		m_customiseUI = new Sprite();
		if (!m_customiseUI->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/customiseMenu.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/customiseMenu.png" << endl;
		m_customiseUI->set_singleSize(1280, 720);
		m_customiseUI->set_screenSize(1280, 720);
	}

	if (m_topBarBackground == nullptr)
	{
		m_topBarBackground = new Sprite();
		if (!m_topBarBackground->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/TopBarNumbers.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/TopBarNumbers.png" << endl;
		m_topBarBackground->set_singleSize(62, 62);
		m_topBarBackground->set_location(9, 4);
	}

	if (m_sprMouseCursor == nullptr)
	{
		m_sprMouseCursor = new Sprite();
		if (!m_sprMouseCursor->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/ToolIcons.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/ToolIcons.png" << endl;
		m_sprMouseCursor->set_singleSize(75, 75);
		m_sprMouseCursor->set_location(0, 6);
	}

	if (m_sprTools == nullptr)
	{
		m_sprTools = new Sprite();
		m_sprTools->set_textureSelfManageFromPointer(m_sprMouseCursor->get_textureSelfManageFromPointer());
		m_sprTools->set_singleSize(m_sprMouseCursor->get_actualSizeWidth(), m_sprMouseCursor->get_actualSizeHeight());
		m_sprTools->set_location(1210, 75);
	}

	if (m_sprFlagBackground == nullptr)
	{
		m_sprFlagBackground = new Sprite();
		if (!m_sprFlagBackground->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/flagScreenMenu.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/flagScreenMenu.png" << endl;
		m_sprFlagBackground->set_singleSize(1280, 720);
		m_sprFlagBackground->set_screenSize(1280, 720);
	}

	if (m_sprSaveButton == nullptr)
	{
		m_sprSaveButton = new Sprite();
		m_sprSaveButton->set_textureSelfManageFromPointer(m_sprMouseCursor->get_textureSelfManageFromPointer());
		m_sprSaveButton->set_singleSize(m_sprMouseCursor->get_actualSizeWidth(), m_sprMouseCursor->get_actualSizeHeight());
		m_sprSaveButton->set_location(SCREEN_WIDTH - m_sprSaveButton->get_actualSizeWidth(), -5);
		m_sprSaveButton->set_singleLocation(0, 6);
	}

	if (m_sprPlayButton == nullptr)
	{
		m_sprPlayButton = new Sprite();
		if (!m_sprPlayButton->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/playClapper.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/playClapper.png" << endl;
		m_sprPlayButton->set_singleSize(117, 121);
		m_sprPlayButton->set_screenSize(117, 121);
		m_sprPlayButton->set_location(0, SCREEN_HEIGHT - m_sprPlayButton->get_actualSizeHeight());
	}

	if (m_sprMario == nullptr)
	{
		m_sprMario = new Sprite();
		if (!m_sprMario->LoadTextureFromFile(p_sb->get_renderer(), "Images/Mario3/Char_MarioSmall.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/Mario3/Char_MarioSmall.png" << endl;
		m_sprMario->set_singleSize(18, 18);
		m_sprMario->set_singleLocation(0, 2);
		m_sprMario->set_camera(m_cameraWhole, true);
		m_sprMario->set_location(64, l_iCameraLowestY + (10*55));
		m_sprMario->set_scaleSize(m_tmTilemap->get_scaleSize());
	}

	if (m_tooltip == nullptr)
	{
		m_tooltip = new ToolTip(p_sb, p_controls);
		//m_tooltip->set_showToolTip(true);
		//m_tooltip->set_words("HELLO");
	}

	if (m_sprWorldSizeBackground == nullptr)
	{
		m_sprWorldSizeBackground = new Sprite();
		if (!m_sprWorldSizeBackground->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/WorldSizeUI.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/WorldSizeUI.png" << endl;
		m_sprWorldSizeBackground->set_singleSize(377, 32);
		m_sprWorldSizeBackground->set_location((SCREEN_WIDTH / 2) - (m_sprWorldSizeBackground ->get_actualSizeWidth()/2), SCREEN_HEIGHT - 28);

		m_sprWorldIcons = new Sprite();
		m_sprWorldIcons->set_textureSelfManageFromPointer(m_sprWorldSizeBackground->get_textureSelfManageFromPointer());
		m_sprWorldIcons->set_singleSize(32, 32);
		m_sprWorldIcons->set_singleLocation(1, 0);

		m_sprEndOfWorldPole = new Sprite();
		if (!m_sprEndOfWorldPole->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/endOfWorldFlag.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/endOfWorldFlag.png" << endl;
		m_sprEndOfWorldPole->set_singleSize(97, 620);
		m_sprEndOfWorldPole->set_singleLocation(0, 0);
		m_sprEndOfWorldPole->set_camera(m_cameraWhole, true);
		m_iEndOfWorldLocation = m_tmTilemap->get_worldSize();
		//m_iEndOfWorldLocation = m_tmTilemap->get_actualWandH() * 173;
		m_sprEndOfWorldPole->set_location(m_iEndOfWorldLocation, l_iCameraLowestY + (10 * 55) - m_sprEndOfWorldPole->get_actualSizeHeight());
		
	}

	if (m_sprMenuButton == nullptr)
	{
		m_sprMenuButton = new Sprite();
		if (!m_sprMenuButton->LoadTextureFromFile(p_sb->get_renderer(), "Images/LevelEditor/menuButton.png"))
			cout << "LevelEditor::main_loadContent: Couldn't load: " << "Images/LevelEditor/menuButton.png" << endl;
		m_sprMenuButton->set_singleSize(69, 69);
		m_sprMenuButton->set_screenSize(69, 69);
		m_sprMenuButton->set_location(SCREEN_WIDTH - m_sprMenuButton->get_actualSizeWidth() + 5, SCREEN_HEIGHT - m_sprMenuButton->get_actualSizeHeight() + 5);
	}

	load_audio();
	main_loadTopBar();
	return true;
}

bool LevelEditor::load_audio()
{
	if (p_audio == nullptr) return false;
	if (m_iDesignMusic == -1)
		m_iDesignMusic = p_audio->find_musicNeverFail("Audio/Music/Super Mario Maker - Edit-Mode (SMB  Overworld) - Music.mp3");
	if (m_iDesignMusic == -1) cout << "LevelEditor::load_audio: Could not find: " << "Super Mario Maker - Edit-Mode (SMB  Overworld) - Music.mp3" << endl;

	if (m_iUIClick == -1)
		m_iUIClick = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/LQ_Click_Button.wav");
	if (m_iUIClick == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/LQ_Click_Button.wav" << endl;

	if(m_iActionClick == -1)
		m_iActionClick = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Select_Button_01_4.wav");
	if (m_iActionClick == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Select_Button_01_4.wav" << endl;

	if (m_iAcceptSound == -1)
		m_iAcceptSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/FA_Select_Button_1_4.wav");
	if (m_iAcceptSound == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/FA_Select_Button_1_4.wav" << endl;

	if (m_iMessagePopupSound == -1)
		m_iMessagePopupSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Error_Button_05_4.wav");
	if (m_iMessagePopupSound == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Error_Button_05_4.wav" << endl;

	if (m_iDrawSounds[0] == -1)
	{
		m_iDrawSounds[0] = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Crop_Produce_01_1.wav");
		if (m_iDrawSounds[0] == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Crop_Produce_01_1.wav" << endl;
		m_iDrawSounds[1] = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Crop_Produce_01_2.wav");
		if (m_iDrawSounds[1] == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Crop_Produce_01_2.wav" << endl;
		m_iDrawSounds[2] = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Crop_Produce_01_3.wav");
		if (m_iDrawSounds[2] == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Crop_Produce_01_3.wav" << endl;
	}

	if (m_iEraseSounds[0] == -1)
	{
		m_iEraseSounds[0] = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Cute_Chirpy_Button_01_1.wav");
		if (m_iEraseSounds[0] == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Cute_Chirpy_Button_01_1.wav" << endl;
		m_iEraseSounds[1] = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Cute_Chirpy_Button_01_2.wav");
		if (m_iEraseSounds[1] == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Cute_Chirpy_Button_01_2.wav" << endl;
		m_iEraseSounds[2] = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Cute_Chirpy_Button_01_3.wav");
		if (m_iEraseSounds[2] == -1) cout << "LevelEditor::load_audio: Could not find: " << "UI/CGM3_Cute_Chirpy_Button_01_3.wav" << endl;
	}

	return true;
}

bool LevelEditor::main_loadTopBar()
{
	string l_sFileLocation = m_fileSelectorCompletePackage->m_sTopFolderLocation + "/" + m_fileSelectorCompletePackage->m_sTopFolderSelection;
	string l_sLeft = l_sFileLocation, l_sRight = "";
	if (Utility::SplitStringAtPoint(&l_sLeft, &l_sRight, '.'))
		if (l_sLeft != "")
			l_sFileLocation = l_sLeft;
	l_sFileLocation += "_levelEditor.txt";

	ifstream infile(l_sFileLocation);
	if (!infile.is_open())
	{
		cout << "bool  FileSelectorMenu::main_loadTopBar(string " << l_sFileLocation << "):  Could not open file." << endl;
		return false;
	}

	string f_sGetLine = "", f_sContent = "";
	int f_iLine = 0;
	if (m_llTilesInTopBar != nullptr) delete m_llTilesInTopBar;
	m_llTilesInTopBar = new LinkedList();

	LinkedList *l_llCurrent = m_llTilesInTopBar;
	LinkedList *l_llInner = nullptr;
	while (getline(infile, f_sGetLine))
	{
		l_llCurrent->set_data(Utility::SplitStringToLL(f_sGetLine, '|'));
		if (l_llCurrent->length() < 16)
		{
			int l_iLength = l_llCurrent->length();
			l_llInner = l_llCurrent->get_data(nullptr);
			if (l_llInner == nullptr)
			{
				l_llInner = new LinkedList();
				l_llCurrent->set_data(l_llInner);
				l_iLength--;
			}
			for (int i = 0; i < 16 - l_iLength; i++)
				l_llInner->AddNewToEnd("-1");
		}
		l_llCurrent = m_llTilesInTopBar->AddNewToEnd();
	}
	//We need as many rows/cols as are in the tileset even if they're all not in the file.
	//Otherwise the reset button would need to do this twice which I'm not huge on
	int l_iNewLength = m_tmTilemap->get_tilesetRows() - 1;
	int l_iNewCols = m_tmTilemap->get_tilesetCols() - 16;
	int l_iNewElements = l_iNewCols * m_tmTilemap->get_tilesetRows();
	float l_fLeftOverlength = l_iNewElements / 16.0f;
	int l_fWholeScreenNeeded = ((Utility::RoundFloat(l_fLeftOverlength, 1) / 7) * 7) + 7;
	l_iNewLength += l_fWholeScreenNeeded;//Round up
	if (l_iNewLength % 7 != 0)
	{
		int l_iLengthWorkOut = l_iNewLength / 7;
		l_iNewLength = (l_iLengthWorkOut * 7) + 7;
	}
	m_iMaxLengthTopBar = l_iNewLength;

	f_sGetLine = "Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined|Undefined";
	if(l_llCurrent->length() < l_iNewLength)
		for (int i = 0; i < l_iNewLength; i++)
		{
			l_llCurrent->set_data(Utility::SplitStringToLL(f_sGetLine, '|'));
			if(i + 1 < l_iNewLength)//Don't add an extra one
				l_llCurrent = m_llTilesInTopBar->AddNewToEnd();
		}
	infile.close();
	//debug_topBarLinkedList();//Debugging what this actually made

	load_specialBlockStatus();
	return true;
}

bool LevelEditor::keyboard_shortcuts()
{
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_uiInnerStage != leuisNoSubMenu) return false;
	if (p_controls == nullptr) return false;
	if (p_controls->isKeyDown(Controls::KeyboardKey::SPACE))
	{
		if (!m_bHitButton)
		{
			if (m_bShowUI)
				m_bShowUI = false;
			else
				m_bShowUI = true;
		}
		m_bHitButton = true;
	}
	else if (p_controls->isKeyDown(Controls::KeyboardKey::Key_V))
	{
		if (!m_bHitButton)
		{
			m_leTool = letMove;
			if (p_audio != nullptr && m_iActionClick > -1)
				p_audio->play_soundEffect(m_iActionClick, 0);
		}
		m_bHitButton = true;
	}
	else if (p_controls->isKeyDown(Controls::KeyboardKey::Key_B))
	{
		if (!m_bHitButton)
		{
			m_leTool = letPen;
			if (p_audio != nullptr && m_iActionClick > -1)
				p_audio->play_soundEffect(m_iActionClick, 0);
		}
		m_bHitButton = true;
	}
	else if (p_controls->isKeyDown(Controls::KeyboardKey::Key_C))
	{
		if (!m_bHitButton)
		{
			m_leTool = letEraser;
			if (p_audio != nullptr && m_iActionClick > -1)
				p_audio->play_soundEffect(m_iActionClick, 0);
		}
		m_bHitButton = true;
	}
	else if (p_controls->isKeyDown(Controls::KeyboardKey::Key_F))
	{
		if (!m_bHitButton)
		{
			m_leTool = letFlag;
			if (m_iTileselected != -1)
				m_iTileselected = -1;
			if (p_audio != nullptr && m_iActionClick > -1)
				p_audio->play_soundEffect(m_iActionClick, 0);
		}
		m_bHitButton = true;
	}
	else if (p_controls->isKeyDown(Controls::KeyboardKey::Key_I))
	{
		if (!m_bHitButton)
		{
			if(m_leSelection != lesEyedrop)
				m_leSelection = lesEyedrop;
			else
				m_leSelection = lesSingle;
			if (p_audio != nullptr && m_iActionClick > -1)
				p_audio->play_soundEffect(m_iActionClick, 0);
		}
		m_bHitButton = true;
	}
	else if (p_controls->isKeyDown(Controls::KeyboardKey::Key_O))
	{
		if (!m_bHitButton)
		{
			if (m_leSelection != lesArea)
				m_leSelection = lesArea;
			else
				m_leSelection = lesSingle;
			if (p_audio != nullptr && m_iActionClick > -1)
				p_audio->play_soundEffect(m_iActionClick, 0);
		}
		m_bHitButton = true;
	}
	else
		m_bHitButton = false;

	return true;
}

bool LevelEditor::main_drawFlagScreen()
{
	if (m_uiStage != leuisFlagScreen) return false;
	if (m_sprFlagBackground != nullptr)
		m_sprFlagBackground->RenderToScreen(p_sb->get_renderer());

	draw_flagScreen();
	return true;
}

bool LevelEditor::debug_topBarLinkedList()
{
	if (m_llTilesInTopBar == nullptr) return false;

	LinkedList *l_llLine;
	string l_sData = "";
	if (m_llTilesInTopBar == nullptr) return false;
	for (int i = 0; i < m_llTilesInTopBar->length(); i++)
	{
		cout << "i: " << i << " : ";
		l_llLine = m_llTilesInTopBar->read_data(i, nullptr);
		l_sData = m_llTilesInTopBar->read_data(i, l_sData);
		cout << l_sData << endl;
		if (l_llLine != nullptr)
		{
			for (int j = 0; j < l_llLine->length(); j++)
			{
				//cout << "l_llLine->length(): " << l_llLine->length() << endl;
				cout << "- " << l_llLine->read_data(j, l_sData) << endl;
			}
		}
		cout << endl;
	}

	return true;
}

bool LevelEditor::control_universalActions(float f_deltatime)
{
	if (p_controls == nullptr) return false;
	if (p_controls->get_menuInput() == Controls::miMouse)
	{
		Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
		if (l_mouseInfo->state == Controls::mouseInfo::msRight)
			m_bRightClick = true;
		else
		{
			if (m_bRightClick)
				if (m_iTileselected > -1)
				{
					if (m_leTool == letPen || m_leTool == letPointer) m_leTool = letMove;
					m_iTileselected = -1;

					if (p_audio != nullptr && m_iMessagePopupSound > -1)
						p_audio->play_soundEffect(m_iMessagePopupSound,0);
				}
					
			m_bRightClick = false;
		}
	}
	return true;
}

bool LevelEditor::control_movementOfWorld(float f_deltatime)
{
	if (m_iTileselected != -1) return false;
	if (m_leSelection != lesSingle) return false;
	if (m_leTool != letMove) return false;
	if (p_controls == nullptr) return false;
	if(m_uiStage != leuisNoSubMenu) return false;

	if (!m_bAreDraggingMainSelection)//I'm disabling Keyboard for now
	{
		//Keyboard works all the time:
		if (p_controls->isGameKeyDown(Controls::DPAD_DOWN))
			m_cameraWhole->MoveCamera(0, 1);
		else if (p_controls->isGameKeyDown(Controls::DPAD_UP))
			m_cameraWhole->MoveCamera(0, -1);
		if (p_controls->isGameKeyDown(Controls::DPAD_LEFT))
			m_cameraWhole->MoveCamera(-1, 0);
		else if (p_controls->isGameKeyDown(Controls::DPAD_RIGHT))
			m_cameraWhole->MoveCamera(1, 0);
	}
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (!m_bHasLeftClickMainSelection)
		{
			SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
			SDL_Rect l_rectMainSection = { 0, 65, 1206, 626 };
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			{
				m_bAreDraggingMainSelection = true;
				m_iMouseWhenClickedMainSelectionX = *l_mouseInfo->x;
				m_iMouseWhenClickedMainSelectionY = *l_mouseInfo->y;

				if (p_audio != nullptr && m_iUIClick > -1)
					p_audio->play_soundEffect(m_iUIClick,0);
			}
		}
		else if (m_bAreDraggingMainSelection)
		{
			m_cameraWhole->MoveCamera((int)(m_iMouseWhenClickedMainSelectionX - *l_mouseInfo->x), (int)(m_iMouseWhenClickedMainSelectionY - *l_mouseInfo->y));
			m_iMouseWhenClickedMainSelectionX = *l_mouseInfo->x;
			m_iMouseWhenClickedMainSelectionY = *l_mouseInfo->y;
				
		}
		m_bHasLeftClickMainSelection = true;
	}
	else
	{
		if(m_bAreDraggingMainSelection)
			p_audio->play_soundEffect(m_iUIClick, 0);
		m_bHasLeftClickMainSelection = false;
		m_bAreDraggingMainSelection = false;
	}
	return true;
}

bool LevelEditor::control_topBarInteraction(float f_deltatime)
{
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft && m_bShowUI)
	{
		SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
		SDL_Rect l_rectMainSection;
		if(m_uiStage != leuisCustomiseMenu && m_uiInnerStage != leuisCustomiseMenu)
			l_rectMainSection = { 0, 0, 1206, 65 };
		else
			l_rectMainSection = { 0, 0, 1206, 691 };
		if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
		{
			m_bTopClickedInRegion = true;
		}
		m_bTopLeftClick = true;
	}
	else if(m_bShowUI)
	{
		if (m_bTopLeftClick && m_bTopClickedInRegion)
		{
			SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
			SDL_Rect l_rectMainSection;
			if (m_uiStage != leuisCustomiseMenu && m_uiInnerStage != leuisCustomiseMenu)
				l_rectMainSection = { 0, 0, 1206, 65 };
			else
				l_rectMainSection = { 0, 0, 1206, 691 };
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			{
				Vector2D l_vector2D = { m_topBarBackground->get_locationX(),m_topBarBackground->get_locationY() };
				for (int line = 0; line < 8; line++)
				{
					l_vector2D.add_valueX(15 + m_topBarBackground->get_actualSizeWidth());
					int l_iPanelNumber = 0;
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							SDL_Rect l_rectBox = { l_vector2D.m_iX,l_vector2D.m_iY, m_topBarBackground->get_actualSizeWidth(),m_topBarBackground->get_actualSizeHeight() };
							if (Utility::ReturnCollision(l_rectMouse, l_rectBox))//They clicked on this one:
							{
								if (m_uiStage == leuisCustomiseMenu)
									if (m_iTileselected > -1)
										if (line > 0)
										{
											//Destination (second click) is the customise bar
											if (swap_elements(m_iTileSelectedRow - 1, m_iTileselected, m_iCustomiseBarsOffset + line - 1, l_iPanelNumber))
											{
												m_iTileselected = -1;
												l_vector2D.add_valueX(m_topBarBackground->get_actualSizeWidth());
												l_iPanelNumber++;
												if (p_audio != nullptr && m_iActionClick > -1)
													p_audio->play_soundEffect(m_iActionClick, 0);
												continue;
											}
										}
										else
										{
											//Destination (second click) is the top bar
											if (swap_elements(m_iTileSelectedRow - 1, m_iTileselected, m_iSelectedNumber - 1, l_iPanelNumber))
											{
												m_iTileselected = -1;
												l_vector2D.add_valueX(m_topBarBackground->get_actualSizeWidth());
												l_iPanelNumber++;
												if (p_audio != nullptr && m_iActionClick > -1)
													p_audio->play_soundEffect(m_iActionClick, 0);
												continue;
											}
										}
										
								m_iTileSelectedRow = line + m_iCustomiseBarsOffset;
								if (line == 0) m_iTileSelectedRow = m_iSelectedNumber;
								m_iTileselected = l_iPanelNumber;
								update_toolsTopBar();
								if (p_audio != nullptr && m_iActionClick > -1)
									p_audio->play_soundEffect(m_iActionClick, 0);
							}
							l_vector2D.add_valueX(m_topBarBackground->get_actualSizeWidth());
							l_iPanelNumber++;
						}
						l_vector2D.add_valueX(10);
					}
					SDL_Rect l_rectBox = { l_vector2D.m_iX,l_vector2D.m_iY, m_topBarBackground->get_actualSizeWidth(),m_topBarBackground->get_actualSizeHeight() };
					if (Utility::ReturnCollision(l_rectMouse, l_rectBox))//They clicked the Arrow
					{
						if (line == 0)
						{
							if (m_uiStage != leuisCustomiseMenu && m_uiInnerStage != leuisCustomiseMenu)
							{
								if(m_uiStage != leuisFlagScreen && m_uiInnerStage != leuisCustomiseMenu)
									m_uiStage = leuisCustomiseMenu;
								else
									m_uiInnerStage = leuisCustomiseMenu;

								if (p_audio != nullptr && m_iUIClick > -1)
									p_audio->play_soundEffect(m_iUIClick);
								//m_iTileselected = -1;
							}
							else
							{
								if(m_uiInnerStage != leuisCustomiseMenu)
									m_uiStage = leuisNoSubMenu;
								else
									m_uiInnerStage = leuisFlagScreen;

								if (p_audio != nullptr && m_iMessagePopupSound > -1)
									p_audio->play_soundEffect(m_iMessagePopupSound);
							}
									
						}
						else if(m_uiStage == leuisCustomiseMenu)
						{
							m_iSelectedNumber = line + m_iCustomiseBarsOffset;

							if (p_audio != nullptr && m_iActionClick > -1)
								p_audio->play_soundEffect(m_iActionClick);
						}
					}

					l_vector2D.m_iX = m_topBarBackground->get_locationX();
					l_vector2D.add_valueY(m_topBarBackground->get_actualSizeHeight() - 3);
					if (m_uiStage != leuisCustomiseMenu && m_uiInnerStage != leuisCustomiseMenu) break;
				}
				if (m_uiStage == leuisCustomiseMenu || m_uiInnerStage == leuisCustomiseMenu)
				{
					SDL_Rect l_rectBox = { 86,478,115, 35 };
					if (Utility::ReturnCollision(l_rectMouse, l_rectBox))//They clicked on up
					{
						m_iCustomiseBarsOffset -= 7;
						if (m_iCustomiseBarsOffset < 7) m_iCustomiseBarsOffset = 0;

						if (p_audio != nullptr && m_iUIClick > -1)
							p_audio->play_soundEffect(m_iUIClick);
					}

					l_rectBox = { 212,478,115, 35 };
					if (Utility::ReturnCollision(l_rectMouse, l_rectBox))//They clicked on down
					{
						m_iCustomiseBarsOffset += 7;
						//int l_iHeighestOffset = m_llTilesInTopBar->length() - 7;
						if (m_iCustomiseBarsOffset >= m_iMaxLengthTopBar) m_iCustomiseBarsOffset -= 7;
						if (p_audio != nullptr && m_iUIClick > -1)
							p_audio->play_soundEffect(m_iUIClick);
					}

					l_rectBox = { 352,478,482,35 };
					if (Utility::ReturnCollision(l_rectMouse, l_rectBox))//They clicked on Save
					{
						m_alertBox = new AlertBox(p_controls, p_sb);
						m_alertBox->set_title("Are you sure?");
						m_alertBox->set_content("Once you save you cannot go back, you may want to make a backup just incase this fails.");
						m_alertBox->set_leftButton("Go back");//Good One
						m_alertBox->set_rightButton("Save Tiles");//Bad One
						m_alertBox->set_responceRight(3);
						m_alertBox->set_audio(p_audio);
						if (p_audio != nullptr && m_iMessagePopupSound > -1)
							p_audio->play_soundEffect(m_iMessagePopupSound, 0);
					}

					l_rectBox = { 986,478,115, 35 };
					if (Utility::ReturnCollision(l_rectMouse, l_rectBox))//They clicked on Reset
					{
						m_alertBox = new AlertBox(p_controls, p_sb);
						m_alertBox->set_title("Are you sure?");
						m_alertBox->set_content("This will reset all the customise tiles and replace|them with the tileset in order.|If you press save after there is no going back.");
						m_alertBox->set_leftButton("Keep current");//Good One
						m_alertBox->set_rightButton("Reset tiles");//Bad One
						m_alertBox->set_audio(p_audio);
						if (p_audio != nullptr && m_iMessagePopupSound > -1)
							p_audio->play_soundEffect(m_iMessagePopupSound, 0);
					}
				}
			}
		}
		m_bTopLeftClick = false;
		m_bTopClickedInRegion = false;
	}

	if (m_sprTiles != nullptr)
	{
		if (m_iTileselected > -1)//Let's set the active tile sprite
		{
			m_v2dSelectedLocation.set_value(*l_mouseInfo->x - (m_sprTiles->get_actualSizeWidth() / 2), *l_mouseInfo->y - (m_sprTiles->get_actualSizeHeight() / 2));
		}
	}
	return true;
}

bool LevelEditor::main_topBarDraw()
{
	if (m_uiStage == leuisCustomiseMenu || m_uiInnerStage == leuisCustomiseMenu)
		m_customiseUI->RenderToScreen(p_sb->get_renderer());

	int l_iTopNumberRow = 0;
	int l_iTopNumberCol = m_iSelectedNumber - 1;
	if (m_iSelectedNumber > 7)
	{
		l_iTopNumberRow = (m_iSelectedNumber - 1) / 7;
		l_iTopNumberCol = (m_iSelectedNumber - 1) - (l_iTopNumberRow * 7);
		l_iTopNumberRow++;
	}

	m_topBarBackground->set_singleLocation(l_iTopNumberRow, l_iTopNumberCol);
	m_topBarBackground->RenderToScreen(p_sb->get_renderer());
	m_topBarBackground->set_singleLocation(0, 9);

	Vector2D l_vector2D = { m_topBarBackground->get_locationX(),m_topBarBackground->get_locationY() };
	l_vector2D.add_valueX(15 + m_topBarBackground->get_actualSizeWidth());
	int l_iColNumber = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_topBarBackground->RenderToScreen(p_sb->get_renderer(), &l_vector2D);

			draw_topBarElements(m_iSelectedNumber - 1, l_iColNumber++, l_vector2D.m_iX + 3, l_vector2D.m_iY + 3);

			l_vector2D.add_valueX(m_topBarBackground->get_actualSizeWidth());
		}
		l_vector2D.add_valueX(10);
	}
	m_topBarBackground->set_singleLocation(0, 10);
	m_topBarBackground->RenderToScreen(p_sb->get_renderer(), &l_vector2D);

	if (m_uiStage != leuisCustomiseMenu && m_uiInnerStage != leuisCustomiseMenu) return true;
	//Time to render the whole customise screen
	for (int line = 0; line < 7; line++)
	{
		l_vector2D.add_valueY(m_topBarBackground->get_actualSizeHeight() - 3);
		l_vector2D.set_value(m_topBarBackground->get_locationX(), l_vector2D.m_iY);
		
		m_topBarBackground->set_singleLocation(1 + (int)m_iCustomiseBarsOffset / 7, line);
		m_topBarBackground->RenderToScreen(p_sb->get_renderer(), &l_vector2D);
		m_topBarBackground->set_singleLocation(1 + (int)m_iCustomiseBarsOffset / 7, 9);
		l_vector2D.add_valueX(15 + m_topBarBackground->get_actualSizeWidth());
		l_iColNumber = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_topBarBackground->RenderToScreen(p_sb->get_renderer(), &l_vector2D);

				draw_topBarElements(m_iCustomiseBarsOffset + line, l_iColNumber++, l_vector2D.m_iX + 3, l_vector2D.m_iY + 3);

				l_vector2D.add_valueX(m_topBarBackground->get_actualSizeWidth());
			}
			l_vector2D.add_valueX(10);
		}
		m_topBarBackground->set_singleLocation(1 + (int)m_iCustomiseBarsOffset / 7, 10);
		m_topBarBackground->RenderToScreen(p_sb->get_renderer(), &l_vector2D);
	}
	return true;
}

bool LevelEditor::draw_tools()
{
	Vector2D l_vector2D = { m_sprTools->get_locationX(), m_sprTools->get_locationY() };
	for (int i = 1; i < m_letToolsCount - m_letToolsCountNotDisplayed; i++)
	{
		m_sprTools->set_singleLocation(0, i - 1);
		if ((int)m_leTool == i)
			m_sprTools->set_singleLocation(1, m_sprTools->get_sourceCol());
		else
			m_sprTools->set_singleLocation(0, m_sprTools->get_sourceCol());
		m_sprTools->RenderToScreen(p_sb->get_renderer(), &l_vector2D);
		l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
	}
	l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
	for (int i = 1; i < m_leSelectionsCount; i++)
	{
		m_sprTools->set_singleLocation(0, m_letToolsCount - 4 + i);
		if ((int)m_leSelection == i)
			m_sprTools->set_singleLocation(1, m_sprTools->get_sourceCol());
		else
			m_sprTools->set_singleLocation(0, m_sprTools->get_sourceCol());
		m_sprTools->RenderToScreen(p_sb->get_renderer() , &l_vector2D);
		l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
	}
	return true;
}

bool LevelEditor::draw_topBarElements(int f_iRowNumber, int f_iColNumber, int f_iLocationX, int f_iLocationY)
{
	LinkedList *l_llTile = get_topBarElement(f_iRowNumber, f_iColNumber);
	if (l_llTile == nullptr)return false;
	string l_sTile = ""; l_sTile  = l_llTile->get_data(l_sTile);
	if (l_sTile == "Unfound") return false;
	if (l_sTile == "Undefined") return false;
	int l_iTileNumber = Utility::convert_stringToInt(l_sTile);
	if((m_uiStage == leuisFlagScreen || m_uiInnerStage == leuisCustomiseMenu) && l_iTileNumber > -1)
	{//When on the flag screen only items with a flag are shown
		string l_sNewFlagLoader = m_tmTilemap->get_tilesetFlagLoader(l_iTileNumber);
		if (l_sNewFlagLoader == "Undefined" || l_sNewFlagLoader == "Unfound")
			return false;
	}
	int l_iRow = 0, l_iCol = 0;
	Utility::GetRowAndColFromNumber(m_tmTilemap->get_tilesetRows(), m_tmTilemap->get_tilesetCols(), l_iTileNumber, l_iRow, l_iCol);
	m_sprTiles->set_singleLocation(l_iRow, l_iCol);
	Vector2D l_vector2d = { f_iLocationX ,f_iLocationY };
	m_sprTiles->RenderToScreen(p_sb->get_renderer(), &l_vector2d);
	return true;
}

LinkedList* LevelEditor::get_topBarElement(int f_iRowNumber, int f_iColNumber)
{
	LinkedList *l_llRow = m_llTilesInTopBar->ReadFromList(f_iRowNumber);
	if (l_llRow == nullptr) return nullptr;
	LinkedList *l_llCol = l_llRow->get_data(nullptr);
	if (l_llCol == nullptr) return nullptr;
	LinkedList *l_llReturn = l_llCol->ReadFromList(f_iColNumber);
	return l_llReturn;
}

LinkedList* LevelEditor::add_barElement(int f_iRowNumber, int f_iColNumber)
{
	int l_iAddingRows = f_iRowNumber - m_llTilesInTopBar->length();
	LinkedList *l_llRow = m_llTilesInTopBar;
	if (l_iAddingRows > 0)
	{
		for (int i = 0; i < l_iAddingRows; i++)
		{
			l_llRow = l_llRow->AddNewToEnd(nullptr);
		}
	}
	int l_iAddingCol = f_iColNumber - l_llRow->length();
	LinkedList *l_llCol = l_llRow->get_data(nullptr);
	if (l_llCol == nullptr)
	{
		l_llCol = new LinkedList(nullptr);
		l_llCol = l_llRow->get_data(nullptr);
	}
		
	if (l_iAddingCol > 0)
	{
		for (int i = 0; i < l_iAddingCol; i++)
		{
			l_llCol = l_llCol->AddNewToEnd(nullptr);
		}
	}
	return l_llCol;
}

bool LevelEditor::swap_elements(int f_iItemOneR, int f_iItemOneC, int f_iItemTwoR, int f_iItemTwoC)
{
	string f_sFirstNumber = "";
	LinkedList *l_llFirstTile = get_topBarElement(f_iItemOneR, f_iItemOneC);
	if (l_llFirstTile == nullptr)l_llFirstTile = add_barElement(f_iItemTwoR, f_iItemTwoC);
	if (l_llFirstTile->get_data(f_sFirstNumber) == "Undefined") l_llFirstTile->set_data("0");
	LinkedList *l_llSecondTile = get_topBarElement(f_iItemTwoR, f_iItemTwoC);
	if (l_llSecondTile == nullptr) l_llSecondTile = add_barElement(f_iItemTwoR, f_iItemTwoC);
	if (l_llSecondTile->get_data(f_sFirstNumber) == "Undefined") l_llSecondTile->set_data("0");
	
	f_sFirstNumber = l_llFirstTile->get_data(f_sFirstNumber);
	l_llFirstTile->set_data(l_llSecondTile->get_data(f_sFirstNumber));
	l_llSecondTile->set_data(f_sFirstNumber);

	return true;
}

bool LevelEditor::update_alertbox(float f_deltaTime)
{
	if (m_alertBox == nullptr) return false;
	m_alertBox->Update();
	p_controls->toggle_mouseCursor(true);
	m_sprMouseCursor->set_visable(false);
	if (m_alertBox->get_enteredSelection() > 0)
	{
		/*
			The alertbox will deal with everything with selection.
			Then return a number on the buttons. 1 means nothing.
			The others I've set in various locations
		*/
		if (m_alertBox->get_enteredSelection() == 2) //Reset tiles
			reset_customiseTiles();
		else if (m_alertBox->get_enteredSelection() == 3) //Reset tiles
			save_customiseTiles();
		else if (m_alertBox->get_enteredSelection() == 4)
			save_tilemap();
		p_controls->toggle_mouseCursor(false);
		m_sprMouseCursor->set_visable(true);
		delete m_alertBox; m_alertBox = nullptr;
	}
	return true;
}
bool LevelEditor::reset_customiseTiles()
{
	/*
		THIS WILL SEEM MORE COMPLICATED THAN IT NEEDS TO BE HOWEVER
		MY TILESET IS 20 IN WIDTH, I'VE ONLY GOT 16 CUSTOMISE BLOCKS
		ON MY LEVEL EDITOR. THIS AT THE END ADDS ON ANY TILES IT MISSED
		BY ALTERING THE ROW AND COL ADDITIVE. THE COL ADDITIVE IS ZERO
		UNTIL YOU REACH THIS OVERFLOW.
	*/
	if (m_llTilesInTopBar == nullptr) return false;

	LinkedList *l_llLine, *l_llTile;
	string l_sData = "";
	if (m_llTilesInTopBar == nullptr) return false;
	int l_iRowLength = m_tmTilemap->get_tilesetCols();
	int l_iRowAdditive = 0;
	int l_iColLength = m_tmTilemap->get_tilesetRows();
	int l_iColAdditive = 0;
	int l_iMarginLeftOver = l_iColLength - 16;
	for (int i = 0; i < m_llTilesInTopBar->length(); i++)
	{
		//cout << "i: " << i << " : ";
		l_llLine = m_llTilesInTopBar->read_data(i, nullptr);
		l_sData = m_llTilesInTopBar->read_data(i, l_sData);
		//cout << l_sData << endl;
		if (l_llLine != nullptr)
		{
			for (int j = 0; j < l_llLine->length(); j++)
			{
				l_llTile = l_llLine->ReadFromList(j);
				l_llTile->set_data(to_string(l_iRowAdditive + j + l_iColAdditive));

				if (i + 1 > l_iColLength)
				{
					if ((j + 1) % l_iMarginLeftOver == 0)
					{
						l_iColAdditive -= l_iMarginLeftOver;
						l_iRowAdditive += l_iRowLength;
					}
				}
			}
		}
		if (i + 1 < l_iColLength)
			l_iRowAdditive += l_iRowLength;
		else
		{
			l_iColAdditive = l_iColLength - l_iMarginLeftOver;
			l_iRowAdditive -= l_iRowLength * l_iMarginLeftOver;
		}

		if (i + 1 == l_iColLength)
		{
			l_iColAdditive = l_iColLength - l_iMarginLeftOver;
			l_iRowAdditive = 0;
		}
	}

	return true;
}

bool LevelEditor::save_customiseTiles()
{
	string l_sFileLocation = m_fileSelectorCompletePackage->m_sTopFolderLocation + "/" + m_fileSelectorCompletePackage->m_sTopFolderSelection;
	string l_sLeft = l_sFileLocation, l_sRight = "";
	if (Utility::SplitStringAtPoint(&l_sLeft, &l_sRight, '.'))
		if (l_sLeft != "")
			l_sFileLocation = l_sLeft;
	l_sFileLocation += "_levelEditor.txt";

	ofstream outFile(l_sFileLocation);
	if (!outFile.is_open()){cout << "LevelEditor::save_customiseTiles(): Couldn't open " << l_sFileLocation << endl;return false;}

	LinkedList *l_llLine;
	string l_sData = "", l_sFileWrite = "";
	if (m_llTilesInTopBar == nullptr) return false;
	for (int i = 0; i < m_iMaxLengthTopBar; i++)
	{
		l_llLine = m_llTilesInTopBar->read_data(i, nullptr);
		l_sData = m_llTilesInTopBar->read_data(i, l_sData);
		//cout << l_sData << endl;
		if (l_llLine != nullptr)
		{
			for (int j = 0; j < l_llLine->length(); j++)
			{
				//cout << "l_llLine->length(): " << l_llLine->length() << endl;
				//cout << "- " << l_llLine->read_data(j, l_sData) << endl;
				if(j < 16)
					l_sFileWrite += l_llLine->read_data(j, l_sData) + '|';
			}
			outFile << l_sFileWrite << '\n';
			l_sFileWrite = "";
		}
		//cout << endl;

	}

	outFile.close();
	return true;
}

bool LevelEditor::load_specialBlockStatus()
{
	s_llTileInformation = m_tmTilemap->get_tileInformationLL();
	return true;
}

bool LevelEditor::control_drawToTilemap(float f_deltatime)
{
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_iTileselected == -1) return false;
	if (p_controls == nullptr) return false;
	if (m_leTool == letMove || m_leTool == letPointer) m_leTool = letPen;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (!m_bClickedInMainRegion)
		{
			SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
			SDL_Rect l_rectMainSection;
			if (m_bShowUI)
				l_rectMainSection = { 0, 65, 1206, 655 };
			else
				l_rectMainSection = { 0, 0, 1280, 720 };
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			{
				string l_sUse = "";
				Tilemap::Tilemap_Tile l_ttTileClicked = m_tmTilemap->get_tileFromLocation(*l_mouseInfo->x + m_cameraWhole->get_x() - (int)m_cameraWhole->get_screenX(), *l_mouseInfo->y + m_cameraWhole->get_y() - (int)m_cameraWhole->get_screenY());
				LinkedList *l_llTileSelected = get_topBarElement(m_iTileSelectedRow - 1, m_iTileselected);
				int l_iTileNumberTilesetForm = Utility::convert_stringToInt(l_llTileSelected->get_data(l_sUse));
				int l_iCurrentTileOnMap = m_tmTilemap->get_tileInfo(l_ttTileClicked.x, l_ttTileClicked.y);
				if (l_iCurrentTileOnMap != -1)
				{
					if (m_leTool == letPen)
						m_tmTilemap->set_tileInPlace(l_ttTileClicked, l_iTileNumberTilesetForm, false);
					else if (m_leTool == letFlag)
					{
						m_tmTilemap->set_tileInPlace(l_ttTileClicked, l_iTileNumberTilesetForm, true, true);
						m_bClickedInMainRegion = true;
					}
				}
							
				if (l_iCurrentTileOnMap == -1)
					m_tmTilemap->set_tileInPlace(l_ttTileClicked, l_iTileNumberTilesetForm, false, true);

				if (p_audio != nullptr && m_iActionClick > -1 && (m_ttPreviousDrawnTile.x != l_ttTileClicked.x || m_ttPreviousDrawnTile.y != l_ttTileClicked.y))
				{
					if (++m_iDrawSoundsCurrent > 2) m_iDrawSoundsCurrent = 0;
					p_audio->play_soundEffect(m_iDrawSounds[m_iDrawSoundsCurrent], 0);
				}
				m_ttPreviousDrawnTile.x = l_ttTileClicked.x;
				m_ttPreviousDrawnTile.y = l_ttTileClicked.y;
			}
		}
	}
	else
	{
		m_bClickedInMainRegion = false;
		m_ttPreviousDrawnTile = { -1,-1 };
	}
	return true;
}

bool LevelEditor::update_tools()
{
	//if (!m_bShowUI) return false;
	update_toolsMouseSelection();
	if (m_leTool == letEraser || m_leSelection == lesEyedrop)
		if (m_iTileselected != -1)
			m_iTileselected = -1;
	if (m_leTool == letPen)
		if (m_iTileselected == -1)
			m_leTool = letMove;
	return true;
}

bool LevelEditor::update_toolsMouseSelection()
{
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisNoSubMenu) return false;
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		SDL_Rect l_rectMainSection = { 1206, 63, 74, 626 };
		if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			m_bToolsLeftClickInRegion = true;
		m_bToolsLeftClick = true;
	}
	else
	{
		if (m_bToolsLeftClick && m_bToolsLeftClickInRegion)
		{
			Vector2D l_vector2D = { m_sprTools->get_locationX(), m_sprTools->get_locationY() };
				
			SDL_Rect l_rectCurrentTool = { 0,0, m_sprTools->get_actualSizeWidth(), m_sprTools->get_actualSizeHeight() };
			for (int i = 1; i < m_letToolsCount - m_letToolsCountNotDisplayed; i++)
			{
				l_rectCurrentTool.x = l_vector2D.m_iX;
				l_rectCurrentTool.y = l_vector2D.m_iY;

				if (Utility::ReturnCollision(l_rectMouse, l_rectCurrentTool))//They clicked on this one:
				{
					if ((int)m_leTool == i)
						m_leTool = letMove;
					else
						m_leTool = (LevelEditorTool)(i);

					if (p_audio != nullptr && m_iActionClick > -1)
						p_audio->play_soundEffect(m_iActionClick, 0);
				}
				l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
			}
			l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
			for (int i = 1; i < m_leSelectionsCount; i++)
			{
				l_rectCurrentTool.x = l_vector2D.m_iX;
				l_rectCurrentTool.y = l_vector2D.m_iY;

				if (Utility::ReturnCollision(l_rectMouse, l_rectCurrentTool))//They clicked on this one:
				{
					if ((int)m_leSelection == i)
						m_leSelection = lesSingle;
					else
						m_leSelection = (LevelEditorSelection)(i);

					if (p_audio != nullptr && m_iActionClick > -1)
						p_audio->play_soundEffect(m_iActionClick, 0);
				}
				l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
			}
			m_bToolsLeftClickInRegion = false;
		}
		m_bToolsLeftClick = false;
	}
	return true;
}

bool LevelEditor::update_toolsTopBar()
{
	if (!m_bShowUI) return false;
	//if (m_uiStage != leuisNoSubMenu) return false;
	string l_sUse = "";//Used just to send to the LinkedList
	LinkedList *l_llTileSelected = get_topBarElement(m_iTileSelectedRow - 1, m_iTileselected);
	int l_iTileNumberTilesetForm = Utility::convert_stringToInt(l_llTileSelected->get_data(l_sUse));

	string l_sNewFlagLoader = m_tmTilemap->get_tilesetFlagLoader(l_iTileNumberTilesetForm);
	if (l_sNewFlagLoader == "Undefined" || l_sNewFlagLoader == "Unfound")
	{
		if (m_leTool != letPen)
			m_leTool = letPen;
		if(m_leSelection == lesEyedrop)
			m_leSelection = lesSingle;
		
		if (m_uiStage == leuisFlagScreen)
		{
			m_iTileselected = -1;
		}
	}
	else
	{
		if (m_leTool != letFlag)
			m_leTool = letFlag;

		m_leSelection = lesSingle;
	}
	return true;
}

bool LevelEditor::control_flagEdit(float f_deltatime, Vector2D f_v2dOpenLocation)
{
	if ((f_v2dOpenLocation.m_iX == -1 && f_v2dOpenLocation.m_iY == -1))
	{
		if (m_uiStage != leuisNoSubMenu) return false;
		if (m_leTool != letFlag) return false;
		if (m_leSelection != lesSingle) return false;
		if (m_iTileselected != -1) return false;
	}
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (!m_bLeftClick)
		{
			SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
			SDL_Rect l_rectMainSection = { 0, 65, 1206, 655 };
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
				m_bClickedFlagRegion = true;
		}
		m_bLeftClick = true;
	}
	else
	{
		if (m_bClickedFlagRegion && m_bLeftClick || (f_v2dOpenLocation.m_iX != -1 || f_v2dOpenLocation.m_iY != -1))
		{
			//So they've clicked with the flag tool to look at the flags
			if(m_ttFETilemapLocation == nullptr)
				m_ttFETilemapLocation = new Tilemap::Tilemap_Tile();//Get the tilemap location
			if(f_v2dOpenLocation.m_iX == -1 || f_v2dOpenLocation.m_iY == -1)
				*m_ttFETilemapLocation = m_tmTilemap->get_tileFromLocation(*l_mouseInfo->x + m_cameraWhole->get_x() - (int)m_cameraWhole->get_screenX(), *l_mouseInfo->y + m_cameraWhole->get_y() - (int)m_cameraWhole->get_screenY());
			else
				*m_ttFETilemapLocation = m_tmTilemap->get_tileFromLocation(f_v2dOpenLocation.m_iX + m_cameraWhole->get_x() - (int)m_cameraWhole->get_screenX(), f_v2dOpenLocation.m_iY + m_cameraWhole->get_y() - (int)m_cameraWhole->get_screenY());

			bool l_bFoundTile = false;
			LinkedList *l_llTilemapTile = m_tmTilemap->get_tilemapTileNeverFail(*m_ttFETilemapLocation, &l_bFoundTile);//Get the tile
			m_llFlagTilemapEditing = l_llTilemapTile;

			string l_sFlagLoader = "Unfound"; l_sFlagLoader = l_llTilemapTile->get_data(l_sFlagLoader);
			if (l_sFlagLoader == "Unfound" || l_sFlagLoader == "Undefined" || l_sFlagLoader == "")//Are there flags on this tilemap tile?
			{
				m_bClickedFlagRegion = false;
				m_bLeftClick = false;
				return false;
			}

			if (m_llFEFlags != nullptr) delete m_llFEFlags;
			m_llFEFlags = Utility::SplitStringToLL(l_sFlagLoader, '_');//Get the flags and put them into this
			if (m_llFEFlags->read_data(0, l_sFlagLoader) == "S" || m_llFEFlags->read_data(0,l_sFlagLoader) == "E" || m_llFEFlags->read_data(0, l_sFlagLoader) == "I")
			{
				LinkedList *l_llOldFlags = m_llFEFlags;
				m_llFEFlags = m_llFEFlags->get_ahead();
				m_llFEFlags->update_identifier(0);
				l_llOldFlags->set_ahead(nullptr);
				delete l_llOldFlags;
			}

			LinkedList *l_llUniqueList = Utility::UniqueElements(m_llFEFlags, to_string(0));
			if(l_llUniqueList != nullptr)//Found some elements
				delete m_llFEFlags; m_llFEFlags = l_llUniqueList;
			m_iFETilenumber = m_tmTilemap->get_tileInfo(m_ttFETilemapLocation->x, m_ttFETilemapLocation->y);//This is the shown tile

			for (int i = 0; i < m_llFEFlags->length(); i++)
			{
				LinkedList *l_sCurrent = m_llFEFlags->ReadFromList(i);
				if (l_sCurrent->get_data(l_sFlagLoader) == "Own")
					l_sCurrent->set_data(m_iFETilenumber);
				else
				{
					l_sCurrent->set_data(Utility::convert_stringToInt(l_sCurrent->get_data(l_sFlagLoader)));
				}
			}
			for (int i = m_llFEFlags->length() - 1; i < 10; i++)
				m_llFEFlags->AddNewToEnd();

			m_uiStage = leuisFlagScreen;

			if (p_audio != nullptr && m_iDrawSounds[0] > -1)
				p_audio->play_soundEffect(m_iDrawSounds[0], 0);
		}
		m_bClickedFlagRegion = false;
		m_bLeftClick = false;
	}
	return true;
}

bool LevelEditor::control_eraser()
{
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_leTool != letEraser) return false;
	if (m_leSelection != lesSingle) return false;
	if (m_iTileselected != -1) return false;
	if (p_controls == nullptr) return false;
	if (m_leTool == letMove || m_leTool == letPointer) m_leTool = letPen;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (!m_bClickedInMainRegion)
		{
			SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
			SDL_Rect l_rectMainSection;
			l_rectMainSection = { 0, 65, 1206, 655 };
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			{
				string l_sUse = "";
				Tilemap::Tilemap_Tile l_ttTileClicked = m_tmTilemap->get_tileFromLocation(*l_mouseInfo->x + m_cameraWhole->get_x() - (int)m_cameraWhole->get_screenX(), *l_mouseInfo->y + m_cameraWhole->get_y() - (int)m_cameraWhole->get_screenY());
				bool l_bFoundTile = false;
				LinkedList *l_llTilemapTile = m_tmTilemap->get_tilemapTileNeverFail(l_ttTileClicked, &l_bFoundTile);//Get the tile
				l_llTilemapTile->set_data(-1);
				l_llTilemapTile->set_data(l_sUse);

				if (p_audio != nullptr && m_iActionClick > -1 && (m_ttPreviousErasedTile.x != l_ttTileClicked.x || m_ttPreviousErasedTile.y != l_ttTileClicked.y))
				{
					if (++m_iEraseSoundsCurrent > 2) m_iEraseSoundsCurrent = 0;
					p_audio->play_soundEffect(m_iEraseSounds[m_iEraseSoundsCurrent], 0);
				}
				m_ttPreviousErasedTile.x = l_ttTileClicked.x;
				m_ttPreviousErasedTile.y = l_ttTileClicked.y;
			}
		}
	}
	else
	{
		m_bClickedInMainRegion = false;
		m_ttPreviousErasedTile = { -1,-1 };
	}
	return true;
}

bool LevelEditor::control_blobber()
{
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_leSelection != lesEyedrop) return false;
	if (m_iTileselected != -1) return false;
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		m_bEyeDropClick = true;
	}
	else
	{
		if (m_bEyeDropClick)
		{
			SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
			SDL_Rect l_rectMainSection;
			l_rectMainSection = { 0, 65, 1206, 655 };
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			{
				string l_sUse = "";
				Tilemap::Tilemap_Tile l_ttTileClicked = m_tmTilemap->get_tileFromLocation(*l_mouseInfo->x + m_cameraWhole->get_x() - (int)m_cameraWhole->get_screenX(), *l_mouseInfo->y + m_cameraWhole->get_y() - (int)m_cameraWhole->get_screenY());
				bool l_bFoundTile = false;
				LinkedList *l_llTilemapTile = m_tmTilemap->get_tilemapTileNeverFail(l_ttTileClicked, &l_bFoundTile);//Get the tile
				int l_iTilesetNumber = l_llTilemapTile->get_data(0);
				Tilemap::Tilemap_Tile l_ttTopBarNumbers = convert_tilesetToTopbar(l_iTilesetNumber);
				m_iTileSelectedRow = l_ttTopBarNumbers.x;
				m_iTileselected = l_ttTopBarNumbers.y;
				m_leSelection = lesSingle;
				//Regarding: "~_". I've no idea why that is occasionally stored. If I blob with a pen it should fix this as flag aren't written.
				if (l_llTilemapTile->get_data(l_sUse) != "~_" && l_llTilemapTile->get_data(l_sUse) != "" && l_llTilemapTile->get_data(l_sUse) != "Unfound" && l_llTilemapTile->get_data(l_sUse) != "Undefined")
					m_leTool = letFlag;
				else
					m_leTool = letPen;

				if (p_audio != nullptr && m_iActionClick > -1 && (m_ttPreviousDrawnTile.x != l_ttTileClicked.x || m_ttPreviousDrawnTile.y != l_ttTileClicked.y))
				{
					if (++m_iDrawSoundsCurrent > 2) m_iDrawSoundsCurrent = 0;
					p_audio->play_soundEffect(m_iDrawSounds[m_iDrawSoundsCurrent], 0);
				}
			}
		}
		m_bEyeDropClick = false;
	}
	return true;
}

bool LevelEditor::update_flagScreen()
{
	control_flagScreen();

	return true;
}

bool LevelEditor::control_flagScreen()
{
	if (m_leSelection != lesSingle) return false;
	if (m_uiStage != leuisFlagScreen) return false;
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		SDL_Rect l_rectMainSection = { 0, 65, 1206, 655 };
		if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			m_bClickedInMainRegion = true;
	}
	else
	{
		if (m_bClickedInMainRegion)
		{
			control_flagMainTiles(l_rectMouse);
			control_flagActions(l_rectMouse);
		}
		m_bClickedInMainRegion = false;
	}
	return true;
}

bool LevelEditor::control_flagMainTiles(SDL_Rect f_rectMouseLocation)
{
	SDL_Rect l_rectMainSection = { 599, 272, 75, 75 };
	bool l_bDidChangeFlags = false;
	for (int i = 0; i < 10; i++)
	{
		if (i == 1)
			l_rectMainSection = { 342, 398, 54, 54 };

		if (Utility::ReturnCollision(f_rectMouseLocation, l_rectMainSection))//White Squares
		{
			l_bDidChangeFlags = true;
			if (m_iTileselected == -1)//Pickup
			{
				LinkedList *l_llCurrent = m_llFEFlags->ReadFromList(i);
				Tilemap::Tilemap_Tile l_ttTopBarTile = convert_tilesetToTopbar(l_llCurrent->get_data(0));
				m_iTileselected = l_ttTopBarTile.y;
				m_iTileSelectedRow = l_ttTopBarTile.x;
				l_llCurrent->set_data(-1);

				
				if (p_audio != nullptr && m_iActionClick > -1)
					p_audio->play_soundEffect(m_iActionClick, 0);
			}
			else // DROP
			{
				Tilemap::Tilemap_Tile l_ttTopBarTile = { m_iTileSelectedRow, m_iTileselected };
				int l_iTilesetTile = convert_topbarToTile(l_ttTopBarTile);

				LinkedList *l_llCurrent = m_llFEFlags->ReadFromList(i);
				l_ttTopBarTile = convert_tilesetToTopbar(l_llCurrent->get_data(0));
				m_iTileselected = l_ttTopBarTile.y;
				m_iTileSelectedRow = l_ttTopBarTile.x;

				l_llCurrent->set_data(l_iTilesetTile);

				if (p_audio != nullptr && m_iDrawSounds[1] > -1)
					p_audio->play_soundEffect(m_iDrawSounds[1], 0);
			}
		}
		l_rectMainSection.x += 64;
	}
	if (l_bDidChangeFlags)
		reorder_flagScreen();
	return true;
}

bool LevelEditor::control_flagActions(SDL_Rect f_rectMouseLocation, bool f_bAutoSave)
{
	SDL_Rect l_rectSave = { 346, 478, 244, 37 };
	if (Utility::ReturnCollision(f_rectMouseLocation, l_rectSave) || f_bAutoSave)//Save Button
	{
		
		bool l_bFoundTile = false;
		LinkedList *l_llTilemapTile = m_llFlagTilemapEditing;//Get the tile
		if (l_llTilemapTile == nullptr) return false;//Technically should be imposible
		string l_sFlags = "";
		int i = 0;
		bool l_bFoundData = false;
		for (i = 0; i < m_llFEFlags->length(); i++)
		{
			LinkedList *l_llCurrent = m_llFEFlags->ReadFromList(i);
			l_llCurrent->set_data(to_string(l_llCurrent->get_data(0)));
			if(l_llCurrent->get_data(0) > -1)
				l_bFoundData = true;
		}

		LinkedList *l_llUniqueFlags = nullptr;
		if(l_bFoundData)
			l_llUniqueFlags = Utility::UniqueElements(m_llFEFlags, l_sFlags);
		
		if (l_llUniqueFlags != nullptr) //There were at least an entry
		{
			for (int j = l_llUniqueFlags->length() - 1; j >= 0; j--)
				if (l_llUniqueFlags->read_data(j, 0) == -1)
					l_llUniqueFlags->delete_item(j);

			l_sFlags = Utility::LinkedListToString(l_llUniqueFlags, '_');

			char l_sPrefix = m_tmTilemap->get_tilesetFlagType(l_llUniqueFlags->get_data(0));//Get the type
			//Attach the type:
			if (l_sPrefix == 'S')
				l_sFlags = "S_" + l_sFlags;
			else if (l_sPrefix == 'E')
				l_sFlags = "E_" + l_sFlags;
			else if (l_sPrefix == 'I')
				l_sFlags = "I_" + l_sFlags;
			//Uncomment to test flag save
			//cout << "BEFORE: " << l_llTilemapTile->get_data(l_sFlags);
			//cout << "AFTER: " << l_sFlags;

			//Update the tilemap:
			l_llTilemapTile->set_data(l_llUniqueFlags->get_data(0));
			l_llTilemapTile->set_data(l_sFlags);

			if (l_llUniqueFlags != nullptr)
				delete l_llUniqueFlags;
		}
		else if (m_llFEFlags->get_data(0) != -1)//There's one entry but unqiue failed for whatever reason
		{
			l_llTilemapTile->set_data(m_llFEFlags->get_data(0));
			l_llTilemapTile->set_data(to_string(m_llFEFlags->get_data(0)));
		}
		else//This is now blank
		{
			l_llTilemapTile->set_data(-1);
			l_llTilemapTile->set_data(l_sFlags);
		}

		m_uiStage = leuisNoSubMenu;
		m_uiInnerStage = leuisNoSubMenu;

		if (p_audio != nullptr && m_iAcceptSound > -1)
			p_audio->play_soundEffect(m_iAcceptSound, 0);
	}

	l_rectSave = { 663, 478, 244, 37 };
	if (Utility::ReturnCollision(f_rectMouseLocation, l_rectSave))//Cancel Button
	{
		m_uiStage = leuisNoSubMenu;
		m_uiInnerStage = leuisNoSubMenu;

		if (p_audio != nullptr && m_iMessagePopupSound > -1)
			p_audio->play_soundEffect(m_iMessagePopupSound, 0);
	}
	return true;
}

Tilemap::Tilemap_Tile LevelEditor::convert_tilesetToTopbar(int f_iNumber)
{
	Tilemap::Tilemap_Tile l_ttReturn;
	LinkedList *l_llLine = nullptr, *l_llCurrent = nullptr;
	for (int i = 0; i < m_llTilesInTopBar->length(); i++)
	{
		l_llLine = m_llTilesInTopBar->ReadFromList(i);
		if (l_llLine == nullptr) continue;
		l_llLine = l_llLine->get_data(nullptr);
		if (l_llLine == nullptr) continue;
		for(int j = 0; j < l_llLine->length(); j++)
		{
			l_llCurrent = l_llLine->ReadFromList(j);
			if (l_llCurrent == nullptr) continue;
			if (l_llCurrent->get_data(to_string(0)) == to_string(f_iNumber))
			{
				l_ttReturn = { i + 1,j };
				return l_ttReturn;
			}
				

		}
	}
	l_ttReturn = { -1,-1 };
	return l_ttReturn;
}

int LevelEditor::convert_topbarToTile(Tilemap::Tilemap_Tile f_ttNumber)
{
	LinkedList *l_llTileSelected = get_topBarElement(m_iTileSelectedRow - 1, m_iTileselected);
	if (l_llTileSelected == nullptr) return -1;
	int l_iTileNumberTilesetForm = Utility::convert_stringToInt(l_llTileSelected->get_data(to_string(0)));
	return l_iTileNumberTilesetForm;
}

bool LevelEditor::draw_flagScreen()
{
	Vector2D l_v2dTile = { 601, 274};
	int l_iRow = -1, l_iCol = -1;
	int l_iTopBarVersion = m_llFEFlags->get_data(0);
	if (l_iTopBarVersion > 0)
	{
		Utility::GetRowAndColFromNumber(m_tmTilemap->get_tilesetRows(), m_tmTilemap->get_tilesetCols(), l_iTopBarVersion, l_iRow, l_iCol);
		m_sprTiles->set_singleLocation(l_iRow, l_iCol);
		m_sprTiles->RenderToScreen(p_sb->get_renderer(), &l_v2dTile);
	}

	l_v2dTile = { 345, 400 };
	for (int i = 1; i < 10; i++)
	{
		
		int l_iTopBarVersion = m_llFEFlags->read_data(i, 0);
		if (l_iTopBarVersion > 0)
		{
			Utility::GetRowAndColFromNumber(m_tmTilemap->get_tilesetRows(), m_tmTilemap->get_tilesetCols(), l_iTopBarVersion, l_iRow, l_iCol);
			m_sprTiles->set_singleLocation(l_iRow, l_iCol);
			m_sprTiles->RenderToScreen(p_sb->get_renderer(), &l_v2dTile);
		}
		l_v2dTile.add_valueX(64);
	}
	return true;
}

bool LevelEditor::reorder_flagScreen()
{
	LinkedList *l_llCurrent = nullptr;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			l_llCurrent = m_llFEFlags->ReadFromList(j);
			if (l_llCurrent->get_data(0) == -1)
			{
				l_llCurrent->set_data(l_llCurrent->get_ahead()->get_data(0));
				l_llCurrent->get_ahead()->set_data(-1);
			}
		}
	return true;
}

bool LevelEditor::draw_save()
{
	if (m_sprSaveButton != nullptr)
		m_sprSaveButton->RenderToScreen(p_sb->get_renderer());
	return true;
}
bool LevelEditor::update_save()
{
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_iTileselected != -1) return false;
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectMainSection = { m_sprSaveButton->get_locationX(), m_sprSaveButton->get_locationY(), m_sprSaveButton->get_actualSizeWidth(),  m_sprSaveButton->get_actualSizeHeight() - 10 };
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (!m_bSaveLeftClick)
		{
			m_bSaveButtonClick = false;
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			{
				m_bSaveButtonClick = true;
			}
		}
		m_bSaveLeftClick = true;
	}
	else
	{
		if (m_bSaveButtonClick)
		{
			if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They clicked on this one:
			{
				m_alertBox = new AlertBox(p_controls, p_sb);
				m_alertBox->set_title("Are you sure?");
				m_alertBox->set_content("Once you save you cannot go back, you may want to make a backup just incase this fails.");
				m_alertBox->set_leftButton("Go back");//Good One
				m_alertBox->set_rightButton("Save Map");//Bad One
				m_alertBox->set_responceRight(4);
				m_alertBox->set_audio(p_audio);

				if (p_audio != nullptr && m_iMessagePopupSound > -1)
					p_audio->play_soundEffect(m_iMessagePopupSound, 0);
			}
		}
		m_bSaveButtonClick = false;
		m_bSaveLeftClick = false;
	}
	return true;
}

bool LevelEditor::save_tilemap(string f_sNewSaveLocation)
{
	string l_sSaveLocation = m_fileSelectorCompletePackage->m_sSubFolderLocation;
	string l_sSaveSelection = m_fileSelectorCompletePackage->m_sSubFolderSelection;
	if (f_sNewSaveLocation != "")
		l_sSaveSelection = f_sNewSaveLocation;
	if (Utility::SearchStringForChar(l_sSaveSelection, '.') > -1)
	{
		string l_sRight;
		if (!Utility::SplitStringAtPoint(&l_sSaveSelection, &l_sRight, '.'))
			cout << "LevelEditor::save_tilemap: Couldn't split selection: " << l_sSaveSelection;
	}
	if (!m_tmTilemap->write_tilemapToFile(l_sSaveLocation + "/" + l_sSaveSelection))
	{
		m_alertBox = new AlertBox(p_controls, p_sb);
		m_alertBox->set_title("Something went wrong!");
		m_alertBox->set_content("Couldn't write to file| Name:|" + l_sSaveLocation + "/" + l_sSaveSelection);
		m_alertBox->set_leftButton("Continue");//Good One
		m_alertBox->set_rightButton("Continue");//Bad One
		m_alertBox->set_responceRight(0);
	}
	return true;
}

bool LevelEditor::draw_play()
{
	if (m_sprPlayButton != nullptr)
		m_sprPlayButton->RenderToScreen(p_sb->get_renderer());
		
	return true;
}

bool LevelEditor::update_play()
{
	if (m_level == nullptr)
		update_playBeforeLevel();
	else
		update_playDuringLevel();
	return true;
}

bool LevelEditor::update_playBeforeLevel()
{
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_iTileselected != -1) return false;
	if (m_leTool != letMove) return false;
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectMainSection = { m_sprPlayButton->get_locationX(), m_sprPlayButton->get_locationY(), m_sprPlayButton->get_actualSizeWidth(),  m_sprPlayButton->get_actualSizeHeight() };
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (!m_bPlayPress && Utility::ReturnCollision(l_rectMouse, l_rectMainSection))
		{
			if (!save_tilemap("LevelEditorPlay"))
				return false;

			if (p_audio != nullptr && m_iAcceptSound > -1)
				p_audio->play_soundEffect(m_iAcceptSound, 0);

			if(m_level == nullptr)
				m_level = new SMB_Level(p_sb, p_controls);
			m_level->set_worldNumber(1);
			m_level->set_levelNumber(1);
			m_level->set_coinsPointer(&m_iCoins);
			m_level->set_livesPointer(&m_iLives);
			m_level->set_pointsPointer(&m_iPoints);
			m_level->set_tilemap("LevelEditorPlay.txt");
			m_level->set_tileset(m_fileSelectorCompletePackage->m_sTopFolderSelection);
			m_level->set_numberOfPlayers(1);
			m_level->set_playerStartPosition(m_sprMario->get_locationX(), m_sprMario->get_locationY());
			m_iEndOfLevel = -1;
			m_level->set_endOfLevel(&m_iEndOfLevel);
			m_level->set_audio(p_audio);
			m_level->set_isLevelEditor(true);

			m_level->LoadContent();

			p_controls->toggle_mouseCursor(true);

			m_sprPlayButton->set_location(SCREEN_WIDTH - m_sprPlayButton->get_actualSizeWidth(), m_sprPlayButton->get_locationY());
			m_sprPlayButton->set_singleLocation(1, 0);
		}
		else
			m_bPlayPress = true;
	}
	else
	{
		m_bPlayPress = false;
	}
	return true;
}

bool LevelEditor::update_playDuringLevel()
{
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectMainSection = { m_sprPlayButton->get_locationX(), m_sprPlayButton->get_locationY(), m_sprPlayButton->get_actualSizeWidth(),  m_sprPlayButton->get_actualSizeHeight() };
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (!m_bPlayPress && Utility::ReturnCollision(l_rectMouse, l_rectMainSection))
		{
			m_bPlayPress = true;
		}
		
	}
	else
	{
		if ((m_bPlayPress && Utility::ReturnCollision(l_rectMouse, l_rectMainSection)) || m_iEndOfLevel >= 0)
		{
			delete m_level;
			m_level = nullptr;
			m_sprPlayButton->set_location(0, m_sprPlayButton->get_locationY());
			m_sprPlayButton->set_singleLocation(0, 0);

			if (p_audio != nullptr && m_iMessagePopupSound > -1)
				p_audio->play_soundEffect(m_iMessagePopupSound, 0);
		}
		m_bPlayPress = false;
	}
	return true;
}

bool LevelEditor::drag_playerPosition()
{
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_iTileselected != -1) return false;
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectMainSection = { m_sprMario->get_locationX() - m_cameraWhole->get_x(), m_sprMario->get_locationY() - m_cameraWhole->get_y(), m_sprMario->get_actualSizeWidth(),  m_sprMario->get_actualSizeHeight() };
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft && (m_leTool != letDragPlayer || m_leTool != letMove))
	{
		if ((!m_bPlayerDrag && Utility::ReturnCollision(l_rectMouse, l_rectMainSection)) || m_leTool == letDragPlayer)
		{
			if (m_leTool != letDragPlayer)
				m_leTool = letDragPlayer;

			if (m_v2dMousePosition.m_iX == -1 || m_v2dMousePosition.m_iY == -1)
			{
				m_v2dMousePosition.m_iX = *l_mouseInfo->x;
				m_v2dMousePosition.m_iY = *l_mouseInfo->y;
			}
			m_sprMario->move_location(*l_mouseInfo->x - m_v2dMousePosition.m_iX, *l_mouseInfo->y - m_v2dMousePosition.m_iY);
			m_v2dMousePosition.m_iX = *l_mouseInfo->x;
			m_v2dMousePosition.m_iY = *l_mouseInfo->y;
		}
		else
			m_bPlayerDrag = true;
	}
	else
	{
		m_bPlayerDrag = false;
		if (m_leTool == letDragPlayer)
		{
			m_leTool = letMove;
		}
		m_v2dMousePosition.m_iX = -1;
		m_v2dMousePosition.m_iY = -1;
	}
	return true;
}

bool LevelEditor::update_tooltips()
{
	if (p_controls == nullptr) return false;
	if (!m_bShowUI) return false;
	update_toolTipsTools();
	update_toolTipsFlagsOnSquares();
	update_toolTipsTopBarFlags();
	return true;
}

bool LevelEditor::update_toolTipsTools()
{
	if (p_controls == nullptr) return false;
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisNoSubMenu || m_uiInnerStage != leuisNoSubMenu) return false;

	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };

	Vector2D l_vector2D = { m_sprTools->get_locationX(), m_sprTools->get_locationY() };
	SDL_Rect l_rectCurrentTool = { 0,0, m_sprTools->get_actualSizeWidth(), m_sprTools->get_actualSizeHeight() };
	bool l_bAreOverIcon = false;
	for (int i = 1; i < m_letToolsCount - m_letToolsCountNotDisplayed; i++)
	{
		l_rectCurrentTool.x = l_vector2D.m_iX;
		l_rectCurrentTool.y = l_vector2D.m_iY;

		if (Utility::ReturnCollision(l_rectMouse, l_rectCurrentTool))//They clicked on this one:
		{
			m_tooltip->set_showToolTip(true);
			m_tooltip->set_id(1);
			switch (i)
			{
			case 1:
				m_tooltip->set_words("Move (v)");
				break;
			case 2:
				m_tooltip->set_words("Draw (b)");
				break;
			case 3:
				m_tooltip->set_words("Eraser (c)");
				break;
			case 4:
				m_tooltip->set_words("Flags (f)");
				break;
			}
			if (m_iPreviousToolTip != i)
				p_audio->play_soundEffect(m_iUIClick, 0);
			m_iPreviousToolTip = i;
			l_bAreOverIcon = true;
		}
		l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
	}
	l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
	for (int i = 1; i < m_leSelectionsCount; i++)
	{
		l_rectCurrentTool.x = l_vector2D.m_iX;
		l_rectCurrentTool.y = l_vector2D.m_iY;

		if (Utility::ReturnCollision(l_rectMouse, l_rectCurrentTool))//They clicked on this one:
		{
			m_tooltip->set_showToolTip(true);
			m_tooltip->set_id(1);
			switch (i)
			{
			case 1:
				m_tooltip->set_words("EyeDropper (i)");
				break;
			case 2:
				m_tooltip->set_words("Area (o)");
				break;
			}
			if (p_audio != nullptr && m_iPreviousToolTip != i + m_letToolsCount)
				p_audio->play_soundEffect(m_iUIClick,0);
			m_iPreviousToolTip = i + m_letToolsCount;
			l_bAreOverIcon = true;
		}
		l_vector2D.add_valueY(m_sprTools->get_actualSizeHeight() + 10);
	}
	if (!l_bAreOverIcon)
	{
		if (m_tooltip->get_id() == 1)
		{
			m_tooltip->set_id(-1);
			m_tooltip->set_showToolTip(false);
			m_iPreviousToolTip = -1;
		}
	}
	return true;
}
bool LevelEditor::update_toolTipsFlagsOnSquares()
{
	if (p_controls == nullptr) return false;
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisFlagScreen && m_uiInnerStage != leuisFlagScreen) return false;

	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };

	SDL_Rect l_rectMainSection = { 599, 272, 75, 75 };
	bool l_bDidChangeFlags = false;
	for (int i = 0; i < 10; i++)
	{
		if (i == 1)
			l_rectMainSection = { 342, 398, 54, 54 };

		if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//White Squares
		{
			l_bDidChangeFlags = true;
			if (m_tooltip->get_id() != 2)
			{
				LinkedList *l_llCurrent = m_llFEFlags->ReadFromList(i);
				string l_sFlag = m_tmTilemap->get_tilesetFlagLoader(l_llCurrent->get_data(0));
				if (l_sFlag == "Unfound") continue;
				if (l_sFlag == "Undefined") continue;
				m_tooltip->set_words(l_sFlag);
				m_tooltip->set_showToolTip(true);
				m_tooltip->set_id(2);
				if(p_audio != nullptr)
					p_audio->play_soundEffect(m_iUIClick, 0);
			}
		}
		l_rectMainSection.x += 64;
	}
	if (!l_bDidChangeFlags)
	{
		if (m_tooltip->get_id() == 2)
		{
			m_tooltip->set_id(-1);
			m_tooltip->set_showToolTip(false);
		}
	}
	return true;
}

bool LevelEditor::update_toolTipsTopBarFlags()
{
	if (p_controls == nullptr) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectMainSection;

	if (m_uiStage != leuisCustomiseMenu && m_uiInnerStage != leuisCustomiseMenu)
		l_rectMainSection = { 0, 0, 1206, 65 };
	else
		l_rectMainSection = { 0, 0, 1206, 691 };

	bool l_bFoundFlag = false;
	if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))//They're in this area
	{
		Vector2D l_vector2D = { m_topBarBackground->get_locationX(),m_topBarBackground->get_locationY() };
		for (int line = 0; line < 8; line++)
		{
			l_vector2D.add_valueX(15 + m_topBarBackground->get_actualSizeWidth());
			int l_iPanelNumber = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					SDL_Rect l_rectBox = { l_vector2D.m_iX,l_vector2D.m_iY, m_topBarBackground->get_actualSizeWidth(),m_topBarBackground->get_actualSizeHeight() };
					if (Utility::ReturnCollision(l_rectMouse, l_rectBox))//They clicked on this one:
					{

						int l_iTileSelectedRow = line + m_iCustomiseBarsOffset;
						if (line == 0) l_iTileSelectedRow = m_iSelectedNumber;
						int l_iTileselected = l_iPanelNumber;

						LinkedList *l_llTileSelected = get_topBarElement(l_iTileSelectedRow - 1, l_iTileselected);
						int l_iTileNumberTilesetForm = Utility::convert_stringToInt(l_llTileSelected->get_data(to_string(0)));

						string l_sNewFlagLoader = m_tmTilemap->get_tilesetFlagLoader(l_iTileNumberTilesetForm);
						if (l_sNewFlagLoader != "Undefined" && l_sNewFlagLoader != "Unfound")
						{
							m_tooltip->set_words(l_sNewFlagLoader);
							m_tooltip->set_showToolTip(true);
							m_tooltip->set_id(3);
							l_bFoundFlag = true;
							if(p_audio != nullptr && m_sPreviousFlag != l_sNewFlagLoader)
								p_audio->play_soundEffect(m_iUIClick, 0);
							m_sPreviousFlag = l_sNewFlagLoader;
						}
					}
					l_vector2D.add_valueX(m_topBarBackground->get_actualSizeWidth());
					l_iPanelNumber++;
				}
				l_vector2D.add_valueX(10);
			}
			l_vector2D.m_iX = m_topBarBackground->get_locationX();
			l_vector2D.add_valueY(m_topBarBackground->get_actualSizeHeight() - 3);
			if (m_uiStage != leuisCustomiseMenu && m_uiInnerStage != leuisCustomiseMenu) break;
		}
	}
	if(!l_bFoundFlag)
	{
		if (m_tooltip->get_id() == 3)
		{
			m_tooltip->set_id(-1);
			m_tooltip->set_showToolTip(false);
			m_sPreviousFlag = "";
		}
	}
	return true;
}

bool LevelEditor::draw_worldSize()
{
	if (m_sprWorldSizeBackground == nullptr) return false;

	m_sprWorldSizeBackground->RenderToScreen(p_sb->get_renderer());

	m_sprWorldIcons->set_singleLocation(1, 0);
	m_sprWorldIcons->set_location(m_sprWorldSizeBackground->get_locationX(), m_sprWorldSizeBackground->get_locationY());
	m_sprWorldIcons->RenderToScreen(p_sb->get_renderer());

	m_sprWorldIcons->RenderToScreen(p_sb->get_renderer());

	//float l_fUnits = m_sprWorldSizeBackground->get_actualSizeWidth() // m_cameraWhole->get_worldWidth();
	float l_fUnits = 0.0208333333333333;//This should be the above however with a 1920*10 world, 1920*8.6 gives a really nice result
	Vector2D l_v2dCurrentLocation;
	l_v2dCurrentLocation.m_iX = m_sprWorldSizeBackground->get_locationX() + (int)(l_fUnits * m_cameraWhole->get_x());
	l_v2dCurrentLocation.m_iY = m_sprWorldIcons->get_locationY();
	
	m_sprWorldIcons->set_singleLocation(1, 1);
	m_sprWorldIcons->RenderToScreen(p_sb->get_renderer(), &l_v2dCurrentLocation);

	m_sprWorldIcons->set_singleLocation(1, 2);
	l_v2dCurrentLocation.m_iX = m_sprWorldSizeBackground->get_locationX() + ((30 + m_iEndOfWorldLocation - (m_tmTilemap->get_actualWandH() * 17)) * l_fUnits);
	l_v2dCurrentLocation.m_iY -= 5;
	m_sprWorldIcons->RenderToScreen(p_sb->get_renderer(), &l_v2dCurrentLocation);
	return true;
}

bool LevelEditor::update_worldSize()
{
	if (!m_bShowUI) return false;
	if (p_controls == nullptr) return false;
	update_worldScroll();
	update_worldEndOfWorld();
	return true;
}

bool LevelEditor::update_worldScroll()
{
	if (p_controls == nullptr) return false;
	if (m_leTool != letMove) return false;
	if (m_leSelection != lesSingle) return false;
	if (!m_bShowUI) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectMainSection = { m_sprWorldSizeBackground->get_locationX(),m_sprWorldSizeBackground->get_locationY(), m_sprWorldSizeBackground->get_actualSizeWidth(),m_sprWorldSizeBackground->get_actualSizeHeight() };

	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))
		{
			float l_fUnitsReverse = 48;
			float l_fNewX = (l_rectMouse.x - m_sprWorldSizeBackground->get_locationX()) * 48;
			m_cameraWhole->set_x(l_fNewX);
		}
	}
	return true;
}

bool LevelEditor::update_worldEndOfWorld()
{
	if (p_controls == nullptr) return false;
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisNoSubMenu) return false;
	if (m_iTileselected != -1) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectFlagPole = { m_sprEndOfWorldPole->get_locationX() - m_cameraWhole->get_x(),m_sprEndOfWorldPole->get_locationY() - m_cameraWhole->get_y(), m_sprEndOfWorldPole->get_actualSizeWidth(),m_sprEndOfWorldPole->get_actualSizeHeight() };

	if (l_mouseInfo->state == Controls::mouseInfo::msLeft && (m_leTool != letDragEndPole || m_leTool != letMove))
	{
		if ((!m_bAreDragingPole && Utility::ReturnCollision(l_rectMouse, l_rectFlagPole)) || m_leTool == letDragEndPole)
		{
			if (m_leTool != letDragEndPole)
				m_leTool = letDragEndPole;

			if (m_v2dMousePosition.m_iX == -1 || m_v2dMousePosition.m_iY == -1)
			{
				m_v2dMousePosition.m_iX = *l_mouseInfo->x;
				m_v2dMousePosition.m_iY = *l_mouseInfo->y;
			}
			m_sprEndOfWorldPole->move_location(*l_mouseInfo->x - m_v2dMousePosition.m_iX, *l_mouseInfo->y - m_v2dMousePosition.m_iY);
			m_v2dMousePosition.m_iX = *l_mouseInfo->x;
			m_v2dMousePosition.m_iY = *l_mouseInfo->y;
		}
		else
			m_bAreDragingPole = true;
	}
	else
	{
		m_bAreDragingPole = false;
		if (m_leTool == letDragEndPole) //They were draging the pole
		{
			m_iEndOfWorldLocation = m_sprEndOfWorldPole->get_locationX();
			if (m_tmTilemap != nullptr)
				m_tmTilemap->set_worldSize(m_iEndOfWorldLocation);
			m_leTool = letMove;
		}
		m_v2dMousePosition.m_iX = -1;
		m_v2dMousePosition.m_iY = -1;
	}
	return true;
}

bool LevelEditor::update_menuButton()
{
	if (p_controls == nullptr) return false;
	if (m_leTool != letMove) return false;
	if (!m_bShowUI) return false;
	if (m_uiStage != leuisNoSubMenu) return false;
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
	SDL_Rect l_rectMainSection = { m_sprMenuButton->get_locationX(),m_sprMenuButton->get_locationY(), m_sprMenuButton->get_actualSizeWidth(),m_sprMenuButton->get_actualSizeHeight() };

	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))
			m_bHavePressedMenu = true;
		else
			m_bHavePressedMenu = false;
	}
	else
	{
		if (Utility::ReturnCollision(l_rectMouse, l_rectMainSection))
		{
			if (m_bHavePressedMenu)
			{
				*s_iGameSelectMenu = 2;
				p_controls->toggle_mouseCursor(true);
			}
		}
		m_bHavePressedMenu = false;
	}
	return true;
}