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
#include "ItemManager.h"
#include "ItemSpawnInformation.h"
#include "ScoreOnScreen.h"
#include "FireBall.h"
#include "SMB_LevelEnd.h"
#include "ParticleManager.h"

using namespace::std;

class SMB_Level
{
private:
	int *s_iEndOfLevel = nullptr;
	bool m_bIsLevelEditor = false;

	int m_iWorldNumber = 1;
	int m_iLevelNumber = 1;
	
	int m_iPauseEverythingButPlayer = -1;//If above -1 will pause the update bar the player who matches the number 
	SpriteBatch *m_sb;
	Tilemap *m_tmLevelMap = nullptr;
	Background *m_background = nullptr;
	Camera *m_camera = nullptr;
	Camera *m_camera2 = nullptr;
	Camera *m_cameraWhole = nullptr;
	Controls *m_controls = nullptr;
	SMB_PlayerPlatformer *m_player1 = nullptr;
	SMB_PlayerPlatformer *m_player2 = nullptr;
	SMB_PlayerPlatformer *m_playersAsArray[2] = { nullptr };
	Audio *m_audio = nullptr;
	int m_iCameraPlayers = 1;
	int m_iWholeCameraPerson = 1;
	int m_iCollapseSplitScreenToWhole = -1;

	SMB_ScreenUI *m_suiPlayerOne = nullptr;
	SMB_ScreenUI *m_suiPlayerTwo = nullptr;
	Sprite *m_UIBlack = nullptr;

	Sprite *m_sprSplitScreenBar = nullptr;
	Sprite *m_spriteRandom = nullptr;

	ItemManager *m_itemManager = nullptr;
	ItemSpawnInformation *m_itemSpawnInfo = nullptr;
	int m_itemSpawnInfoLength = 50;

	int *m_iCoins = nullptr;
	int *m_iLives = nullptr;
	int *m_iPoints = nullptr;
	int m_iTimeLeft = 300;

	bool m_bPlayerOneIsLeft = true;
	bool m_bChosenSplitLocation = false;

	ScoreOnScreen *m_scoreOnScreen = nullptr;

	ParticleManager *m_partcleManager = nullptr;

	//Setup:
	int m_iNumberOfPlayers = 1;
	string m_sTilemap = "1_1.txt";
	string m_sTileset = "Basic";
	Vector2D m_v2dPlayerSpawnLocation = {-1, -1 };
	
	//End of the Level:
	SMB_LevelEnd *m_endOfLevel = nullptr;
	int m_iPlayerOneEndOfLevel = 0;
	int m_iPlayerTwoEndOfLevel = 0;
	bool m_bFinishTheLevel = false;
	bool m_bStartEndAnimation = false;

	FireBall *m_fireball = nullptr;

	//Debuging
	SprLetters *m_sprlettersFPS = nullptr;
	float m_fLastTimeDelta = 0;
	int m_iFPSLast = 0;
	int m_iUpdateFPS = 0;
	int m_iUpdateFPSLimit = 100;

	//Audio
	int m_iMusicTrack = -1;
public:
	SMB_Level(SpriteBatch *f_sb, Controls *f_controls);
	virtual ~SMB_Level();

	bool LoadContent();
	bool Draw();
	bool Update(float f_deltaTime);

	void set_worldNumber(int f_iInput) { m_iWorldNumber = f_iInput; }
	void set_levelNumber(int f_iInput) { m_iLevelNumber = f_iInput; }
	void set_coinsPointer(int *f_iInput) { m_iCoins = f_iInput; }
	void set_livesPointer(int *f_iInput) { m_iLives = f_iInput; }
	void set_pointsPointer(int *f_iInput) { m_iPoints = f_iInput; }
	void set_tilemap(string f_sData) { m_sTilemap = f_sData; }
	void set_tileset(string f_sData) { m_sTileset = f_sData; }
	void set_numberOfPlayers(int f_iData) { m_iNumberOfPlayers = f_iData; }
	void set_playerStartPosition(int f_iX, int f_iY) { m_v2dPlayerSpawnLocation.m_iX = f_iX; m_v2dPlayerSpawnLocation.m_iY = f_iY; }
	void set_endOfLevel(int *f_i) { s_iEndOfLevel = f_i; }
	void set_audio(Audio *f_audio) { m_audio = f_audio; }
	void set_isLevelEditor(bool f_b) { m_bIsLevelEditor = f_b; }
private:
	bool update_multiplayerCamera();
	bool update_screenUILocation();
	bool update_endOfLevel(float f_deltaTime);
	bool switch_cameraFocus(int f_iPlayer);

	bool update_smoothCameraTransition(float f_deltaTime);

	bool update_runFinishedSequence(float f_deltaTime);
};

