#include "Particle.h"



Particle::Particle()
{
}


Particle::~Particle()
{
}

bool Particle::Update(float f_deltaTime)
{
	if (m_bFirstRun)
		set_didDrawThisFrame(true);
	m_bFirstRun = false;

	update_hMovement(f_deltaTime);
	Platformer_Character::Update(f_deltaTime);
	update_deletion();
	return true;
}

bool Particle::update_hMovement(float f_deltaTime)
{
	switch (m_pDirection)
	{
	case pdLeft:
		move_location(m_fMovementIncrese * f_deltaTime, 0);
		break;
	case pdRight:
		move_location((m_fMovementIncrese * f_deltaTime) * -1, 0);
		break;
	}
	update_sprite(f_deltaTime);
	return true;
}

bool Particle::update_deletion()
{
	if (!get_didDrawThisFrame())
		m_bDeleteMe = true;
	return true;
}

bool Particle::update_sprite(float f_deltaTime)
{
	m_fFlipSpriteCurrent += f_deltaTime;
	if (m_fFlipSpriteCurrent < m_fFlipSpriteOn * 1)
		set_singleLocation(0, 0);
	else if (m_fFlipSpriteCurrent < m_fFlipSpriteOn * 2)
		set_singleLocation(0, 1);
	else
		m_fFlipSpriteCurrent -= m_fFlipSpriteOn * 2;
	return true;
}