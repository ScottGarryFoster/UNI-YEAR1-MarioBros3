#pragma once
#include "GameEngine.h"
#include "Controls.h"
#include "SuperMarioBros3.h"
#include "TextureSelfManaged.h"
#include "Platformer_TilesetEditor.h"
#include "GameInstance.h"
#include "AlertBox.h"
#include "LevelEditor.h"
#include "Sound.h"
#include "Music.h"
#include "SoundEffect.h"

class GameSelect : public GameEngine
{
private:
	//Debug
	bool m_bDebug = false;
	bool m_bDebug2 = false;
	int m_iNumber = 0;

	//System
	float m_fDeltaTime = 0;

	//GameSelectMenu
	int m_iGameLoaded = 0;//The game which is loaded currently
	int m_iGameSelected = 0;//Game selected within the menu
	int m_iMMOptionSelected = 0; //(MM is main menu and only applies to the outter most menu)
	bool m_bMMOptionReady = false;
	Sprite *m_sprGameWallpaper = nullptr;
	Sprite *m_sprMenuBackground = nullptr;
	Sprite *m_sprMenuButtons = nullptr;
	bool m_bBackgroundYSway = false;

	//Control Input Change
	Sprite *m_sprCicIcons = nullptr;
	Controls::menuInput m_miCicLastSeen = Controls::miKeyboard;
	Controls::menuInput m_miCicDisplaying = Controls::miKeyboard;
	int m_iCicLastSeenFrame = -1;
	

	//Mouse Control Stuff
	Controls::mouseInfo* m_cMouseInfo = nullptr;
	int m_iNumberOfButtons = 5;//How many buttons you have
	int m_iOptionNumbers[5] = { 0,1,2,3,4 };//The selection options
	int *m_iSelectionLocation = nullptr;
	int m_iPreviousLocation = -1;
	int m_iMouseStateRecorded = false;
	//You then handle the left click your self, this just handles selection
	Sprite *m_sprControllerManagementIcon = nullptr;
	bool m_bControllerLeftClick = false;
	bool m_bDisableControlMethodIcon = false;//If you're using a combo of keyboard and mouse you may want to turn off the indicator for controller change.

	//Options Menu
	int s_iGameSelectMenu = 0;//S means Shared which isa  var only used to single from a belonging to something which has the belonging.
	int m_iOptionsOptionSelected = 0;
	AlertBox *m_alertBox = nullptr;

	//Level Editor Menu
	bool m_bLEMonMenu = false;
	int m_iLEMOptionSelected = 0;
	bool m_iLEMCanPress = false;

	GameInstance *m_editor = nullptr;

	//Games
	SuperMarioBros3 *m_gameMario = nullptr;

	//Audio
	int m_iTitleScreenMusic = -1;
	int m_iUIClick = -1;
	int m_iAcceptSound = -1;
	int m_iMessagePopupSound = -1;
public:
	GameSelect(int argc, char* args[]);
	~GameSelect();
	void LoadContent();
	void AfterContent();
	bool GameLoop(float f_deltaTime);
	bool Draw();

	//Game Selection Menu:
	bool UpdateLoadContent();
	bool UpdateGameSelect(float f_deltaTime);
	bool UpdateDraw();

	//GameManagement:
	bool LoadNewGame(int f_iGame = -1);
	bool FreeResources();
private:
	//Control Input Change:
	bool draw_controlInputChange();
	bool update_controlInputChange(float f_deltaTime);

	//Main Menu
	bool update_inputSelection();
	bool update_inputMouseSelection();
	bool update_sucessfulSelection();
	bool update_backgroundLocation(float f_deltaTime);
	bool update_inputKeyboard();
	bool update_inputMouse();

	bool load_mainMenuMouseOptions();

	//Options Menu (in game options)
	bool update_optionsMenu();
	bool update_optionsKeyboard();
	bool update_optionsMouse();
	bool update_optionsSelection();
	bool draw_optionsMenu();

	//Level Editor Selection
	bool update_levelEditorMenu();
	bool update_levelEditorKeyboard();
	bool update_levelEditorMouse();
	bool update_levelEditorSelection();
	bool draw_levelEditorMenu();

	bool setup_editor(int f_iType);

	//Controller Management
	bool update_controllerManagement();
};