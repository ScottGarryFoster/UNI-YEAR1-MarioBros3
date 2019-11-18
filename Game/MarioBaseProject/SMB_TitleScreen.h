#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>

#include "SPRITEBATCH.h"
#include "LinkedList.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Controls.h"

#include "SMB_PlayerPlatformer.h"
#include "SMB_ScreenUI.h"
#include "ItemManager.h"
#include "ItemSpawnInformation.h"
#include "ScoreOnScreen.h"

class SMB_TitleScreen
{
private:
	SpriteBatch *p_sb = nullptr;
	Controls *p_control = nullptr;

	enum titleScreenStage : int {tssCurtain = 0, tssLogoAppearing, tssPlayerSelectionScreen};
public:
	SMB_TitleScreen();
	virtual ~SMB_TitleScreen();

	//Defaults
	bool Update(float f_deltaTime);
	bool Draw();
};

