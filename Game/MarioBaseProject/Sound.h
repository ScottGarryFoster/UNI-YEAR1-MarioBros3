#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

#include "Utility.h"

#include <string>

using namespace std;
class Sound
{
public:
	Sound();
	virtual ~Sound();

	//Load
	virtual bool load_track(string f_sLocation) = 0;
	virtual bool set_track(void *f_sound) = 0;
	virtual void* get_track() = 0;

	//Play
	virtual bool play_track(bool f_bInterupt = false, int f_iChannel = -1) = 0;
};