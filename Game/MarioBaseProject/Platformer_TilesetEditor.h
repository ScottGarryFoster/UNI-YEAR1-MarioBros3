#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>
#include <fstream>
#include <iostream>

#include "GameInstance.h"
#include "LinkedList.h"
#include "Utility.h"
#include "SprLetters.h"

using namespace::std;

class Platformer_TilesetEditor : public GameInstance
{
public:
	enum PTE_MenuStage : int {pteMainMenu = 0, pteNewTileset, pteEditMeta, pteEditCollision};
private:
	bool m_bLoaded = false;

	LinkedList *m_llNamesOfTilesets = nullptr;

	PTE_MenuStage m_menuStage = pteMainMenu;

	//All Menus
	Sprite *m_sprBackgroundColor = nullptr;

	//Main Menu:
	SprLetters *m_ltrsTitle = nullptr;
	SprLetters *m_ltrsNewTileset = nullptr;
	SprLetters *m_ltrsEditMeta = nullptr;
	SprLetters *m_ltrsEditCollisions = nullptr;
public:
	Platformer_TilesetEditor(SpriteBatch *f_sb, Audio *f_audio, int *f_iSelectMenu);
	virtual ~Platformer_TilesetEditor();

	virtual bool Draw();
	virtual bool Update(float f_deltatime);
private:
	bool setup_tilesetEditor();
	bool load_tilesetList();

	//MainMenuFunctions
	bool load_mainMenu();
	bool update_mainMenu();
	bool draw_mainMenu();
};

