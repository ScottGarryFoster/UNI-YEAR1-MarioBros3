#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

#include "Sound.h"

#include <string>
using namespace std;

class SoundEffect :public Sound
{
private:
	Mix_Chunk *m_mchunk = nullptr;
	int m_channel = 0;
	bool m_bShouldDeleteMusic = false;
public:
	SoundEffect();
	virtual ~SoundEffect();

	//Load
	virtual bool load_track(string f_location);
	virtual bool set_track(void *f_sound);

	virtual void* get_track();

	//Play
	virtual bool play_track(bool f_bInterupt = false, int f_iChannel = -1);
};