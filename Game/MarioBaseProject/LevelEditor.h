#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>
#include <fstream>

#include "SPRITEBATCH.h"
#include "LinkedList.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Controls.h"
#include "GameInstance.h"
#include "AlertBox.h"
#include "SMB_Level.h"
#include "ToolTip.h"

#include "FileSelectorMenu.h"

using namespace::std;

class LevelEditor : public GameInstance
{
public:
	enum LevelEditorUIStage : int {leuisNoSubMenu = 0, leuisCustomiseMenu, leuisFlagScreen};
	enum LevelEditorTool : int {letPointer = 0, letMove, letPen, letEraser, letFlag, letDragPlayer, letDragEndPole};
	enum LevelEditorSelection : int {lesSingle = 0, lesEyedrop, lesArea};
	enum LevelEditorCursors : int { lecPointer = 0, lecMove, lecPen, lecEraser, lecFlag, lecDragPlayer, lecDragEndPole, lecEyedrop, lecArea };
private:
	bool m_bDebug = false;
	bool m_bFileDebug = false;

	//Choosing the file:
	string m_sFileStructure = "GameFiles/Mario3/LevelEditor/FileStructure.txt";
	FileSelectorMenu *m_fileSelectorMenu = nullptr;
	FileSelectorMenu::FileSelectorCompletePackage *m_fileSelectorCompletePackage = nullptr;

	//Editor:
	Tilemap *m_tmTilemap = nullptr;
	Camera *m_cameraWhole = nullptr;
	bool m_bHasLeftClick = false;
	LevelEditorUIStage m_uiStage = leuisNoSubMenu;
	Sprite *m_panelUI = nullptr;
	Sprite *m_customiseUI = nullptr;
	Sprite *m_topBarBackground = nullptr;
	int m_iSelectedNumber = 1;
	int m_iTileSelectedRow = 1;
	int m_iTileselected = -1;
	Vector2D m_v2dSelectedLocation;
	AlertBox *m_alertBox = nullptr;

	//Mouse Cursor
	Sprite *m_sprMouseCursor = nullptr;
	Sprite *m_sprTools = nullptr;

	//Universal Controls
	bool m_bRightClick = false;

	//Main Mouse Movement
	bool m_bHasLeftClickMainSelection = false;
	bool m_bAreDraggingMainSelection = false;
	int m_iMouseWhenClickedMainSelectionX = 0;
	int m_iMouseWhenClickedMainSelectionY = 0;

	//TopBar
	bool m_bTopLeftClick = false;
	bool m_bTopClickedInRegion = false;
	LinkedList *m_llTilesInTopBar = nullptr;
	Sprite *m_sprTiles = nullptr;
	int m_iCustomiseBarsOffset = 0;
	int m_iMaxLengthTopBar = 0;
	LinkedList *s_llTileInformation = nullptr; //Shared with Tilemap

	//Tools
	LevelEditorTool m_leTool = letMove;
	bool m_bClickedInMainRegion = false;
	int m_letToolsCount = 7;
	int m_letToolsCountNotDisplayed = 2;
	LevelEditorSelection m_leSelection = lesSingle;
	int m_leSelectionsCount = 3;
	bool m_bToolsLeftClick = false;
	bool m_bToolsLeftClickInRegion = false;

	//Flag Editor
	Tilemap::Tilemap_Tile *m_ttFETilemapLocation = nullptr;
	LinkedList *m_llFEFlags = nullptr;
	int m_iFETilenumber = -1;
	Sprite *m_sprFlagBackground = nullptr;
	LevelEditorUIStage m_uiInnerStage = leuisNoSubMenu;
	LinkedList *m_llFlagTilemapEditing = nullptr;
	bool m_bClickedFlagRegion = false;
	bool m_bLeftClick = false;

	//Eyedrop
	bool m_bEyeDropClick = false;

	//Save/Play Buttons
	Sprite *m_sprSaveButton = nullptr;
	Sprite *m_sprPlayButton = nullptr;
	bool m_bSaveButtonClick = false;
	bool m_bSaveLeftClick = false;
	bool m_bPlayPress = false;
	SMB_Level *m_level = nullptr;
	int m_iCoins = 0;
	int m_iLives = 0;
	int m_iPoints = 0;
	int m_iEndOfLevel = -1;

	//Additional Features
	bool m_bShowUI = true;
	bool m_bHitButton = false;

	//Mario's Position
	Sprite *m_sprMario = nullptr;
	bool m_bPlayerDrag = false;
	Vector2D m_v2dMousePosition = { -1,-1 };

	//WorldSize editor:
	Sprite *m_sprWorldSizeBackground = nullptr;
	Sprite *m_sprWorldIcons = nullptr;
	Sprite *m_sprEndOfWorldPole = nullptr;
	int m_iEndOfWorldLocation = 0;
	//UI Portion
	bool m_bWSClickedInArea = false;
	bool m_bAreDragingPole = false;

	//Menu Button
	Sprite *m_sprMenuButton = nullptr;
	bool m_bHavePressedMenu = false;

	//ToolTip
	ToolTip *m_tooltip = nullptr;

	//Audio
	int m_iDesignMusic = -1;
	int m_iUIClick = -1;
	int m_iAcceptSound = -1;
	int m_iMessagePopupSound = -1;
	int m_iActionClick = -1;
	int m_iDrawSounds[3] = { -1,-1,-1 };
	int m_iDrawSoundsCurrent = 0;
	int m_iEraseSounds[3] = { -1,-1,-1 };
	int m_iEraseSoundsCurrent = 0;

	//Audio Triggers
	int m_iPreviousToolTip = -1;
	string m_sPreviousFlag = "";
	Tilemap::Tilemap_Tile m_ttPreviousDrawnTile = { -1,-1 };
	Tilemap::Tilemap_Tile m_ttPreviousErasedTile = { -1,-1 };
public:
	LevelEditor(SpriteBatch *f_sb, Audio *f_audio, int *f_iSelectMenu);
	virtual ~LevelEditor();
	virtual bool Update(float f_deltatime);
	bool Draw();

	bool debug_topBarLinkedList();

private:
	bool SelectFile(float f_deltatime);
	bool SelectFileDraw();

	Tilemap::Tilemap_Tile convert_tilesetToTopbar(int f_iNumber);
	int convert_topbarToTile(Tilemap::Tilemap_Tile f_ttNumber);

	bool main_update(float f_deltatime);
	bool main_draw();
	bool main_topBarDraw();
	bool main_loadContent(float f_deltatime);
	bool main_loadTopBar();
	bool mouse_updateCursor();
	bool main_drawFlagScreen();

	bool control_universalActions(float f_deltatime);
	bool control_movementOfWorld(float f_deltatime);
	bool control_topBarInteraction(float f_deltatime);
	bool control_flagEdit(float f_deltatime, Vector2D f_v2dOpenLocation = { -1,-1 });

	LinkedList* get_topBarElement(int f_iRowNumber, int f_iColNumber);
	LinkedList* add_barElement(int f_iRowNumber, int f_iColNumber);

	bool draw_topBarElements(int f_iRowNumber, int f_iColNumber, int f_iLocationX, int f_iLocationY);
	bool swap_elements(int f_iItemOneR, int f_iItemOneC, int f_iItemTwoR, int f_iItemTwoC);
	bool reset_customiseTiles();
	bool save_customiseTiles();

	bool update_alertbox(float f_deltaTime);
	
	bool load_specialBlockStatus();

	//Draw to tilemap:
	bool control_drawToTilemap(float f_deltatime);

	//Tools
	bool update_tools();
	bool update_toolsMouseSelection();
	bool draw_tools();
	bool update_toolsTopBar();
	bool keyboard_shortcuts();

	//Flag Edit
	bool update_flagScreen();
	bool draw_flagScreen();
	bool control_flagScreen();
	bool control_flagMainTiles(SDL_Rect f_rectMouseLocation);
	bool control_flagActions(SDL_Rect f_rectMouseLocation, bool f_bAutoSave = false);
	bool reorder_flagScreen();

	//Smaller Tools
	bool control_eraser();
	bool control_blobber();

	//Save
	bool draw_save();
	bool update_save();
	bool save_tilemap(string f_sNewSaveLocation = "");

	//Play
	bool draw_play();
	bool update_play();
	bool update_playBeforeLevel();
	bool update_playDuringLevel();
	bool drag_playerPosition();

	//WorldSize
	bool draw_worldSize();
	bool update_worldSize();
	bool update_worldScroll();
	bool update_worldEndOfWorld();

	//Menu
	bool update_menuButton();

	//Tooltips
	bool update_tooltips();
	bool update_toolTipsTools();
	bool update_toolTipsFlagsOnSquares();
	bool update_toolTipsTopBarFlags();

	//Audio
	bool load_audio();
};

