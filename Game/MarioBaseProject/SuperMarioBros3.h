#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>

#include "SPRITEBATCH.h"
#include "LinkedList.h"
#include "Tilemap.h"
#include "Background.h"
#include "Camera.h"
#include "Controls.h"
#include "Audio.h"

#include "SMB_PlayerPlatformer.h"
#include "SMB_ScreenUI.h"
#include "SMB_Level.h"

using namespace::std;

class SuperMarioBros3
{
private:
	SpriteBatch *m_sb;
	Controls *m_controls = nullptr;
	SMB_Level *m_level = nullptr;
	Audio *m_audio = nullptr;

	int m_iCoins[2] = { 0,0 };
	int m_iLives[2] = { 3,3 };
	int m_iPoints = 0;
	int m_iTimeLeft = 300;

	int m_iFrame = 0;
	int *s_iGameSelectMenu = nullptr;//S means Shared which isa  var only used to single from a belonging to something which has the belonging.
	bool m_bReadyForGameSelectEsc = false;

	int m_iEndOfLevel = -1;
	int m_iCurrentLevel = 1;
	int m_iMaxLevel = 3;

	//Mouse:
	int m_iMouseStateRecorded = 0;
	Controls::mouseInfo* m_cMouseInfo = nullptr;
public:
	SuperMarioBros3();
	SuperMarioBros3(SpriteBatch* f_sb) { set_spriteBatch(f_sb); };
	~SuperMarioBros3();

	void set_spriteBatch(SpriteBatch *f_sb){ m_sb = f_sb;}
	void set_controls(Controls *f_controls){ m_controls = f_controls;}
	void set_gameSelectMenu(int *f_i) { s_iGameSelectMenu = f_i; }
	void set_audio(Audio *f_audio) { m_audio = f_audio; }

	void Draw();
	bool Update(float f_deltaTime);
	bool LoadContent(int f_iLevel = 1);

private:
	bool update_gameSelectMenu();
	bool update_gameSelectMouse();
	bool load_nextLevel();
};

