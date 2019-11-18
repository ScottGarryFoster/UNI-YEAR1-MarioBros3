#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include "Sound.h"
#include "Music.h"
#include "SoundEffect.h"
#include "LinkedList.h"

#include <string>
using namespace std;

class Audio
{
private:
	int m_iCurrentMusicPlaying = -1;
	LinkedList *m_llMusicPreload = nullptr;

	LinkedList *m_llsoundeffectPreload = nullptr;

	int m_iSoundEffectChannelsLength = 1;
public:
	Audio();
	virtual ~Audio();

	bool awake() { return true; }//Used to ensure this exists in some construction cases

	//Add
	int add_music(string f_sLocation);
	int add_soundEffect(string f_sLocation);

	//Find Track
	int find_sound(string f_sLocation, LinkedList *f_llTop, bool f_bShowErrors = false);
	LinkedList* find_sound(int f_iTrackNumber, LinkedList *f_llTop, bool f_bShowErrors = false);
	int find_music(string f_sLocation);
	LinkedList* find_music(int f_iTrackNumber);
	int find_musicNeverFail(string f_sLocation);
	int find_soundEffect(string f_sLocation);
	LinkedList* find_soundEffect(int f_iTrackNumber);
	int find_soundEffectNeverFail(string f_sLocation);

	//Find Channel
	int find_unusedChannel();

	//Play
	bool play_music(int f_iTrackNumber, bool f_bForce = false);
	bool play_soundEffect(int f_iTrackNumber, int f_iChannel = -1, bool f_bForce = false);
	bool is_channelUsed(int f_iChannel = -1);

	//Pause/Resume
	bool pause_music();
	bool resume_music();
	bool reset_currentPlaying();
	bool stop_allSoundEffects();
private:
	int add_track(string f_sLocation, LinkedList **l_llDestination, Sound *l_sound);

	Sound* cast_voidToSound(LinkedList *f_llData);
};

