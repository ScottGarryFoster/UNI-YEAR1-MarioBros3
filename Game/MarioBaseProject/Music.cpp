#include "Music.h"



Music::Music()
{
}


Music::~Music()
{
	if (m_bShouldDeleteMusic)
		if (m_mmusic != nullptr)
		{
			Mix_FreeMusic(m_mmusic);
			delete m_mmusic;
		}
}

bool Music::load_track(string f_sLocation)
{
	if (m_mmusic != nullptr)
		if (m_bShouldDeleteMusic)
		{
			Mix_FreeMusic(m_mmusic);
			delete m_mmusic;
		}

	m_mmusic = Mix_LoadMUS(f_sLocation.c_str());
	if (m_mmusic == NULL)
	{
		cout << "Music::load_track: Could not load sound." << Mix_GetError() << endl;
		return false;
	}
	m_bShouldDeleteMusic = true;
	return true;
}

bool Music::set_track(void *f_sound)
{
	if (m_mmusic != nullptr)
		if (m_bShouldDeleteMusic)
		{
			Mix_FreeMusic(m_mmusic);
			delete m_mmusic;
		}
	Mix_Music *l_mmusic = (Mix_Music*)f_sound;
	m_mmusic = l_mmusic;
	m_bShouldDeleteMusic = false;
	return true;
}

void* Music::get_track()
{
	return (void*)m_mmusic;
}

bool Music::play_track(bool f_bInterupt, int f_iChannel)
{
	if (f_bInterupt || Mix_PlayingMusic() == 0)
		Mix_PlayMusic(m_mmusic, -1);
	return true;
}