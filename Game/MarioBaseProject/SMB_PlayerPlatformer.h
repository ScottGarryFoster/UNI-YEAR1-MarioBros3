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
#include "SMB_ScreenUI.h"
#include "ScoreOnScreen.h"
#include "Audio.h"

using namespace std;

class SMB_PlayerPlatformer : public Platformer_Character
{
public:
	enum AnimationModification : int {animNone = 0, animSkid, animSlowDown};
	enum PlayerControl : int {pcPlayerOne = 1, pcPlayerTwo, pcPlayerThree, pcPlayerFour};
	enum AnimationGenerics : int {agNone = 0, agRegularJump, agPJump, agMovement1, agMovement2, agMovement3, agMovement4, agPMovement1, agPMovement2, agPMovement3, agPMovement4, agSkid};
	enum PlayerPowerup : int {ppNone = 0, ppFeather, ppFire};
	enum DeathState : int {dsAlive = 0, dsBubble, dsDead, dsComplete};
	enum BubbleModes : int {bmDefault = 0, bmHover};
protected:
	Audio *p_audio = nullptr;
private:
	int m_iDebugNumber = 0;

	SMB_PlayerPlatformer *p_playerOthers[1] = { nullptr };
	int p_playerOthersLength = 1;
	int *s_iPauseControl = nullptr;//Means is sent from the class who owns this. Set this to the playercontrol to int to pause all but this class.
	int *s_iEndOfLevel = nullptr;
	int *s_iFinishedLevel = nullptr;
	bool m_bHasTriggeredEndCard = false;
	Controls *m_controls = nullptr;
	PlayerControl m_playerControl = pcPlayerOne;
	char m_cPlayerLetter = 'm';
	SMB_ScreenUI *m_screenUI = nullptr;
	bool m_bLinkedScreenUI = false;
	int m_iRow = 0;
	ScoreOnScreen *m_scoreOnScreen = nullptr;
	Platformer_Character::Direction m_directionSprite = Platformer_Character::Direction::Left;

	//UI and Game Stats
	int *s_iStatLives = nullptr;
	int *s_iCoinCount = nullptr;

	int m_iAnimationFrame = 0;
	AnimationModification m_animationModification = animNone;
	float m_fJumpPeakMomentum = 0;
	float m_fJumpPeakMomentumY = 0;
	float m_fJumpMomentum = 0;
	bool m_bCanJump = false;
	bool m_bPressJump = false;
	bool m_bFirstTimeJump = false;
	float m_fPeakJumpHeight = 0;
	int m_fPeakJumpOffset = 8;
	int m_fPeakJumpOffsetDefault = 16;
	bool m_bJumping = false;
	bool m_bJumpBoost = false;
	Momentum_V m_momentumLast = mvNone;

	float m_fSpeedMultipler = 0.6f;

	long long int m_iTicks = 0;

	//Jumping/Movement Stats:
	int m_iMovingAnimationTickDefaultSingle = 105;//Time on a frame
	int m_iMovingAnimationTickDefault = m_iMovingAnimationTickDefaultSingle * 4;//Increase this to slow the frames between certain animations
	int m_iMovingAnimationTick = m_iMovingAnimationTickDefault;//Increase this to slow the frames between certain animations
	float m_bJumpStrength = 0.6f;
	float m_fMovementSpeedDefault = 0.37f;
	float m_fMovementSpeed = 0.37f;
	float m_fActualSpeed = 0;
	float m_fIncrease = 0.0003f;
	int m_iTimesJump = 0;
	bool m_bArePressingMoveButtons = false;//Used for PSpeed
	float m_fSpeedWhenAtFullSpeed = -9001;

	int m_iPSpeed = 0;
	int m_iPSpeedUI = 0;
	float m_fPSpeedFull = 0;
	bool m_bPSpeedFlash = false;
	float m_iPSpeedFlashTicker = 0;

	float m_iPSpeedTicker = 0;

	//Eating Things:
	bool m_bEatingMushroom = false;
	bool m_bEatingPowerup = false;
	PlayerPowerup m_ppAfterState = ppNone;
	int m_iAnimationMushroom = 0;
	int m_iAnimationMushroomGoal = 0;
	int m_iAnimationSet = 0;

	bool m_bIsBig = false;
	PlayerPowerup m_playerPowerup = ppNone;

	//Jumping on Enemies
	int m_iJumpAutoTime = 0;
	bool m_bTakeDamage = false;
	int m_iInvincibilityFramesTick = 2000;
	int m_iInvincibilityFlicker = 20;
	bool m_bReduceSize = false;

	//Holding Shells:
	bool m_bProneToHoldShell = false;
	bool m_bIsHoldingShell = false;

	//Death
	bool m_bDeathAnimation = false;
	DeathState m_deathState = dsAlive;
	BubbleModes m_bubbleMode = bmDefault;
	bool m_bBubbleHoverDirection = false;//False means up, true down
	int m_iBubbleModeHoverTick = 0;
	float m_fBubbleVSpeed = 0;

	//Audio
	int m_iAudioJump = -1;
	int m_iAudioPowerup = -1;
	int m_iAudio1up = -1;
	int m_iAudioLoseLife = -1;
	int m_iAudioLosePower = -1;
	int m_iAudioCoin = -1;

	//Simulation of Button Presses
	int m_iSimulateKeys[10] = { -1 };
public:
	SMB_PlayerPlatformer();
	virtual ~SMB_PlayerPlatformer();

	//Set
	void set_controls(Controls *f_controls) { m_controls = f_controls; }
	void set_audio(Audio *f_audio) { p_audio = f_audio; }
	void set_player(PlayerControl f_pc);
	void set_screenUI(SMB_ScreenUI *f_screenUI);
	void set_pauseControl(int *f_iPtr) { s_iPauseControl = f_iPtr; }
	void set_scoreOnScreen(ScoreOnScreen *f_sos) { m_scoreOnScreen = f_sos; }
	void set_otherPlayers(SMB_PlayerPlatformer *f_player, int f_iNumber) { p_playerOthers[f_iNumber] = f_player; }

	void set_statLivesPtr(int *f_1up) {s_iStatLives = f_1up;}
	void set_coinPtr(int *f_coins) { s_iCoinCount = f_coins;}
	void set_endOfLevel(int *f_i) { s_iEndOfLevel = f_i; }
	void set_isHoldingShell(bool f_bData) { m_bIsHoldingShell = f_bData; }

	void set_finishedLevel(int *f_iPtr) { s_iFinishedLevel = f_iPtr; }
	void set_triggeredEndCard(bool f_bData) { m_bHasTriggeredEndCard = f_bData; }
	void set_deathstate(SMB_PlayerPlatformer::DeathState f_deathState) { m_deathState = f_deathState; }

	//GETS
	bool *get_isBigPtr() { return &m_bIsBig; };
	bool get_isInvinsible() { if (p_iInvincibilityFrames > 0) return true; else return false; }
	DeathState get_deathState() { if (s_iFinishedLevel == nullptr) return m_deathState; else { if (*s_iFinishedLevel > 0) return dsComplete; else return m_deathState;} }
	bool get_isProneToHoldShells() { return m_bProneToHoldShell; }
	bool get_isHoldingShell() { return m_bIsHoldingShell; }
	float get_actualSpeed() { return m_fActualSpeed; }
	bool get_triggeredEndCard() { return m_bHasTriggeredEndCard; }

	virtual Direction get_direction();
	//Defaults
	bool Update(float f_deltaTime);

	//Overide Parent
	virtual bool reset_upwardMomentum();

	//More Specfic:
	bool player_eatsItem(string f_sItem);

	virtual bool adjust_ifStuckInTopLip(SDL_Rect f_rectTileUnder);

	virtual int get_playerNumberFromBases() { return (int)m_playerControl; }//Made to be overridden by the player

	virtual bool update_entiesReactions(int f_iDirectionY, int f_iDirectionX);

	virtual bool load_audio();

	//Simulation of Mechnics:
	bool set_simulateButtonPress(Controls::GameKeys f_gk, bool f_bNewState = true);
	bool get_isSimulatingKey(Controls::GameKeys f_gk);

	//EndOfLevelControl
	bool update_endOfLevel(float f_deltaTime);
private:
	bool update_sprite(float f_deltaTime);
	bool update_input(float f_deltaTime);
	int update_animation(float f_deltaTime);
	int get_animation(AnimationGenerics f_animation);
	int get_animationRow();

	bool update_moving(float f_deltaTime);
	bool update_jumping(float f_deltaTime, bool f_bSetup = false);
	bool update_pSpeed(float f_deltaTime);

	bool update_mushroomAnimation(float f_deltaTime);
	bool update_powerupAnimation(float f_deltaTime);
	bool switch_marioSizeToggle();
	bool switch_marioSizeKnown(bool f_bNewSize);
	bool convert_animationPosition(bool f_bSize, bool f_bDoNotMove = false);

	bool update_handleFeedback();

	bool update_takeDamage(float f_deltaTime);
	bool update_invincibility(float f_deltaTime);
	bool update_reduceSize(float f_deltaTime);
	bool update_animateDeath(float f_deltaTime);

	bool update_bubble(float f_deltaTime);

	bool update_isProneToHoldShell();
};

