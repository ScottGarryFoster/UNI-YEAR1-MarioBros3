#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include "TextureSelfManaged.h"
#include <iostream>
#include <string>

#include "Controls.h"
#include "Camera.h"

#include "Platformer_Character.h"
#include "SMB_PlayerPlatformer.h"
#include "SMB_ScreenUI.h"
#include "ScoreOnScreen.h"
#include "EnemyEffect.h"
#include "SPRITEBATCH.h"
#include "SpecialBlock.h"
#include "Audio.h"

using namespace std;

class Enemy : public Platformer_Character
{
public:
	enum EnemyHealth : int { ehAlive = 0, ehDead, ehShell };
	enum EnemyState : int {esProne = 0, esShell, esMovingShell};
	enum EnemySizeBox : int {esbSmall = 0, esbBig};
	enum EnemyAdditionalBehaviours : int {eabNone = 0, eabKoopa};
	enum EnemyMovement : int { emNone = 0, emWalk, emWalkDontFall };
	enum EnemyDeathSprite : int { edsNone = 0, edsOneCol, edsTwoCol, edsFlip};
protected:
	SMB_PlayerPlatformer *p_players[2] = { nullptr };//Players:
	SpriteBatch *p_sb = nullptr;
	Audio *p_audio = nullptr;
private:
	//Importantr Meta
	int m_iEnemyID = -1;
	bool m_bHasBeenOnScreen = false;
	//General
	EnemyHealth m_enemyHealth = ehAlive;
	EnemyState m_enemyState = esProne;
	int m_iEnemyState = (int)m_enemyState;
	int m_iActivationBuffer = 0;
	int m_iActivationBufferLength = 350;
	int m_iOtherEnemyActivationBuffer = 0;
	int m_iOtherEnemyActivationBufferLength = 50;
	int m_iLastTurnedBecauseOfThisEnemy = -1;

	//SetupViaFile
	string m_sMetaDataFile = "";
	string m_sCharacterSheet = "";
	int m_iSingleSizeWidth = 0;
	int m_iSingleSizeHeight = 0;
	int m_iSingleSizeRow = 0;
	int m_iSingleSizeCol = 0;
	EnemySizeBox m_esbSize = esbSmall;
	EnemySizeBox m_esbSizeOriginal = esbSmall;
	EnemyAdditionalBehaviours m_additionalBehaviours = eabNone;
	EnemyDeathSprite m_deathSprite = edsNone;
	//Effects
	EnemyEffect *m_effects[20];
	int m_effectsLength = 20;

	//Movement
	EnemyMovement m_enemyMovement = emNone;
	enum enemyDirection : int {edLeft = 0, edRight};
	enemyDirection m_enemyDirection = edLeft;
	int m_iWalkingAnimationTime = 0;
	bool m_bHaveStoodOnAPlatform = false;
	float m_fTimePassed = 0;
	float l_fMovementPreviousYLocation = 0;
	Vector2D m_v2dPreviousLocationWhenMovingWithoutFalling = { -1,-1 };

	int m_iMovingShellAnimationFrame = 50;
	int m_iMovingShellFrames = 5;

	//Death
	int m_iDeathAnimationTime = 0;
	bool m_bDead = false;
	bool m_bAnimatedDeath = false;
	
	//Aditional Behaviours
	//Koopa
	int m_iABKoopa_shellTick = 0;
	int m_iABKoopa_timesToShake = 20;
	int m_iABKoopa_shellTickShake = 5000;
	int m_iABKoopa_shellTickTransform = 10000;
	int m_iABKoopa_shakeModification = 0;
	float m_fABKoopa_movementModifier = 0;//Used to transition mario's movement to the moving shel

	//Shell and Being Held as a shell
	bool m_bSBeingHeld = false;
	int m_iSBPlayerIndex = -1;
	int m_iSBLastPlayerIndex = -1;

	//Audio
	int m_iAudioBump = -1;
	int m_iAudioKick = -1;
	int m_iAudioStomp = -1;
public:
	Enemy();
	virtual ~Enemy();

	//Defaults
	virtual bool Update(float f_deltaTime);
	virtual bool Draw();

	//Sets
	void set_spriteBatch(SpriteBatch *f_sb) { p_sb = f_sb; }
	void set_audio(Audio *f_audio) { p_audio = f_audio; }
	void set_playersReference(SMB_PlayerPlatformer *f_ppRef, int f_iNumber) { p_players[f_iNumber] = f_ppRef; }
	void set_id(int f_iData) { m_iEnemyID = f_iData; }
	void set_enemyHealth(EnemyHealth f_ehData) { m_enemyHealth = f_ehData; }

	//GET
	bool get_amDead() { return m_bDead; }
	Tilemap::Tilemap_Tile get_tileStoodOn(Vector2D f_v2dOffsetPixels = { -9001,-9001 });
	bool get_amStoodOnTile(Vector2D f_v2dOffsetPixels, bool f_bDebug = false);
	int get_enemyState() { return (int)m_enemyState; }
	EnemyState get_enemyState(bool f_b) { return m_enemyState; }
	EnemyHealth get_enemyHealth() { return m_enemyHealth; }
	int get_id() { return m_iEnemyID; }
	bool get_isBeingHeld() { return m_bSBeingHeld; }
	
	virtual Direction get_direction() { if (m_enemyDirection == edLeft) return Direction::Left; else return Direction::Right; }

	//Load
	bool load_fromFile(string f_sLocation);
	bool setup_meta(string f_sData);
	bool set_byString(string f_sParameter, string f_sValue);
	bool load_audio();

	//Actions performed on the enemy
	bool update_actionActivation(SDL_Rect f_rectCollision, int f_iDirection, int f_iActor, bool f_bPlayerSize, int *f_iFeedback, int f_iAdditionalNumber = -1);
	bool update_collision(bool f_bDebug = false);

	virtual int get_playerNumberFromBases() { return get_id(); }//Made to be overridden by the player
private:
	int update_actionActivationDirectionUpdate(SDL_Rect f_rectMyRect, SDL_Rect f_rectTheirRect, bool f_bOnlyH = false, bool f_bDebug = false);
	bool update_playerHoldCheck(int f_iPlayerNumber);

	bool update_movement(float f_deltaTime);
	bool update_movementWalk(float f_deltaTime);
	bool update_movementWalkDontFall(float f_deltaTime, bool f_bStoodOnTile = false, bool f_bRecur = false);
	bool update_walkingAnimation(float f_deltaTime);
	bool update_walkingDontFallTurn(float f_deltaTime);
	bool update_shell(float f_deltaTime);
	bool update_movingShell(float f_deltaTime);

	bool update_death(float f_deltaTime);
	bool update_enemyStateChange(int f_iNewState, string f_sAddition = "", int f_iDirection = 0, int *f_iFeedback = nullptr);

	bool update_additionalBehaviour(float f_deltaTime);
	bool update_abKoopa(float f_deltaTime);

	bool update_beingHeldOnUpdate(float f_deltaTime);
	
	bool change_state(SDL_Rect f_rectCollision, int f_iDirection, string f_sStateAddition, int f_iNewState, int *f_iFeedback);

	bool check_movingShellCanMove();
};

