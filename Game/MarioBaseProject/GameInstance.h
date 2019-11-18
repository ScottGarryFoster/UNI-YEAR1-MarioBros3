#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "SPRITEBATCH.h"
#include "LinkedList.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Controls.h"
#include "Audio.h"

class GameInstance
{
private:
	bool m_bKillMe = false; //If true the object which owns this one will destory this object
protected:
	SpriteBatch *p_sb = nullptr;
	Controls *p_controls = nullptr;
	Audio *p_audio = nullptr;
	int *s_iGameSelectMenu = nullptr;
public:
	GameInstance(SpriteBatch *f_sb, Audio *f_audio, int *f_iSelectMenu);
	virtual ~GameInstance();

	virtual bool Draw() = 0;
	virtual bool Update(float f_deltatime) = 0;

	void set_controls(Controls *f_controls) { p_controls = f_controls; }

	bool KillMe(){ return m_bKillMe; }
protected:
	void KillMeNow() { m_bKillMe = true; }
	bool setup_gameInstance(SpriteBatch *f_sb, Audio *f_audio);
};

