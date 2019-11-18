#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>

#include "Sprite.h"
#include "SPRITEBATCH.h"
#include "Vector2D.h"

using namespace::std;

class FireBall : public Sprite
{
protected:
	SpriteBatch *p_sb = nullptr;
private:
	double m_dAngle = -9001;
	double m_dGradient = -9001;
	double m_dIntercept = 0;
	Vector2D m_v2dMovement = { -1,-1 };
public:
	FireBall();
	virtual ~FireBall();

	virtual bool Update(float f_deltaTime);
	virtual bool Draw();

	//Sets:
	void set_spritebatch(SpriteBatch *f_sb) { p_sb = f_sb; }
	void set_angle(double f_dData) { m_dAngle = f_dData; }
	void set_gradient(double f_dData) { m_dGradient = f_dData; }
	void set_intercept(double f_dData) { m_dIntercept = f_dData; }
	void set_movementVector(Vector2D f_v2d) { m_v2dMovement = f_v2d; }
};

