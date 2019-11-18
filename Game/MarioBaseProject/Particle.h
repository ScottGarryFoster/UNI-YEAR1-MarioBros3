#pragma once
#include "Platformer_Character.h"

class Particle : public Platformer_Character
{
public:
	enum partcleDirection : int {pdNone = 0, pdLeft, pdRight};
private:
	float m_fActualSpeed = 0;
	float m_fMovementSpeed = 0;
	float m_fMovementIncrese = 0.2f;

	partcleDirection m_pDirection = pdNone;
	bool m_bDeleteMe = false;

	float m_fFlipSpriteOn = 50;
	float m_fFlipSpriteCurrent = 0;

	bool m_bFirstRun = true;
public:
	Particle();
	virtual ~Particle();

	//Sets
	void set_pDirection(partcleDirection f_pd) { m_pDirection = f_pd; }

	bool get_deleteMe() { return m_bDeleteMe; }

	virtual bool Update(float f_deltaTime);
private:
	bool update_hMovement(float f_deltaTime);
	bool update_deletion();
	bool update_sprite(float f_deltaTime);
};