#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

#include "Sound.h"

#include <string>
using namespace std;

class Music : public Sound
{
private:
	Mix_Music *m_mmusic = nullptr;
	bool m_bShouldDeleteMusic = false;
public:
	Music();
	virtual ~Music();

	//Load
	virtual bool load_track(string f_sLocation);
	virtual bool set_track(void *f_sound);

	virtual void* get_track();

	//Play
	virtual bool play_track(bool f_bInterupt = false, int f_iChannel = -1);
};

