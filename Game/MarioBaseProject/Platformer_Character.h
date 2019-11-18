#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include "Vector2D.h"
#include "TextureSelfManaged.h"
#include "CircleCollision.h"
#include <iostream>
#include <string>

#include "SPRITEBATCH.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Controls.h"

using namespace::std;

class Platformer_Character : public Sprite
{
public:
	enum Direction : int { Left = 0, Right, Down, Up };
	enum Animation : int { None = 0, Moving, Jumping, Attack, Falling };
	enum Momentum_V : int { mvNone = 0, mvUp, mvDown };
protected:
	Tilemap *p_tilemap;
	int p_iFeedback = -1;
	int p_iInvincibilityFrames = 0;
	bool p_disableCollisions = false;
private:
	Controls *m_controls = nullptr;

	float m_fGravityStrength = 0.003f;
	float m_fGravityPull = 0;
	SDL_Rect *m_rectCollision = nullptr;
	CircleCollision *m_circleCollision = nullptr;
	float m_fMomentumDirectionCheckY = -9001;
	bool m_bDisableGravity = false;
	bool m_bResetUpward = false;

	float m_fVActualSpeed = 0;
	float m_fVMovementInverter = 0;
	float m_fVMovementInverterMax = 0.1f;
	float m_fVMovementSpeed = 0.7f;
	float m_fVIncrease = 0.003f;
	bool m_bSuccessfulFall = false;
	float m_fTileUnder = -1;

	bool m_bScrollUp = false;
	bool m_bHaveEverLanded = false;

	int m_iActor = -1;

	int m_iDebugNumber = 0;
protected:
	Direction p_direction = Direction::Right;
	Animation p_animation = Animation::None;
	Momentum_V p_momentumV = Momentum_V::mvNone;

	int p_iCameraLockId = -1;
public:
	Platformer_Character();
	virtual ~Platformer_Character();

	virtual bool Update(float f_deltaTime);
	//SETS

	void set_tilemap(Tilemap *f_tilemap) { p_tilemap = f_tilemap; }
	void set_collision();
	void set_collision(SDL_Rect *f_rect) { m_rectCollision = f_rect; }
	void set_collision(int f_iX, int f_iY, int f_iWidth, int f_iHeight);

	void set_debugControls(Controls *f_c) { m_controls = f_c; }

	void set_gravityPull(float f_fNew) { m_fVActualSpeed = f_fNew; };
	void set_disableGravity(float f_bGravity) { m_bDisableGravity = f_bGravity; };

	void set_lockId(int f_i) { p_iCameraLockId = f_i; }

	void set_actorNumber(int f_i) { m_iActor = f_i; }
	void set_disableCollisions(bool f_b) { p_disableCollisions = f_b; }

	void set_gravityInversionToMax() { if(m_fVMovementInverter <= 0 && m_fVActualSpeed >= -0.1f) m_fVMovementInverter = m_fVMovementInverterMax; }
	void set_gravityInversion(float f_fData) { m_fVMovementInverter = f_fData; }

	void shrink_collision(int f_iValue);//Shrinks Collision Rect evenly

	//GETS

	int get_tileNumberAtFeet();
	int get_tileNumberAtFeet(Tilemap::Tilemap_Tile f_ttRef);
	Tilemap::Tilemap_Tile get_tileReferenceAtFeet();
	Tilemap::Tilemap_Tile get_tileReferenceBottomLeft();
	Tilemap::Tilemap_Tile get_tileReferenceBottomRight();
	SDL_Rect get_collisionRect();
	CircleCollision* get_circleCollision();
	int get_actorNumber(){return m_iActor;}
	virtual Direction get_direction() { return p_direction; }

	float get_singleMovementModifier(float f_delta);

	float get_gravityPull() { return m_fVActualSpeed; }
	float get_gravityStrength() { return m_fGravityStrength; }
	bool get_successfulFall() { return m_bSuccessfulFall; }
	bool get_disableCollisions() { return p_disableCollisions; }

	int get_lockId() { return p_iCameraLockId; }
	float get_vMovementSpeed() { return m_fVMovementSpeed; }

	int* get_feedbackInt() { return &p_iFeedback; }
	//Adjustments
	virtual bool adjust_ifStuckInTopLip(SDL_Rect f_rectTileUnder);
	bool additionalBlockCheck(SDL_Rect f_rectPlayerRect);
	//bool additionalBlockCheck(SDL_Rect f_rectPlayerRect, Tilemap::Tilemap_Tile f_ttYourTile);
	

	Tilemap::Tilemap_Tile get_tileReferenceFromNumberPad(int f_iLocation);
	Tilemap::Tilemap_Tile get_tileReferenceFromNumberPad(int f_iLocation, int f_iX, int f_iY);

	//FUNCTIONS
	bool move_character(float f_fX, float f_fY, Direction *f_directionReturn = nullptr);
	virtual bool update_entiesReactions(int f_iDirectionY, int f_iDirectionX);

	bool can_snapCamera();

	bool update_momentumDirectionCheck();

	virtual bool reset_upwardMomentum();

	virtual int get_playerNumberFromBases() { return -1; }//Made to be overridden by the player
private:
	bool PullCharacterToGravity(float f_deltaTime);
	bool update_gravityPull(float f_deltaTime);
protected:
	bool update_cameraLock();
private:
	bool move_character_innerCheck(Tilemap::tmCollisionSide f_tmcDirection, int f_iNumberpadDirection, float &f_fX, float &f_fY, bool f_bRecurrsion = false);
};

