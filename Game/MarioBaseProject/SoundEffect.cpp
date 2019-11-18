#include "SoundEffect.h"



SoundEffect::SoundEffect()
{
}


SoundEffect::~SoundEffect()
{
	if (m_bShouldDeleteMusic)
		if (m_mchunk != nullptr)
		{
			Mix_FreeChunk(m_mchunk);
			delete m_mchunk;
		}
}

bool SoundEffect::load_track(string f_location)
{
	if (m_mchunk != nullptr)
		if (m_bShouldDeleteMusic)
		{
			Mix_FreeChunk(m_mchunk);
			delete m_mchunk;
		}

	m_mchunk = Mix_LoadWAV(f_location.c_str());
	if (m_mchunk == NULL)
	{
		cout << "SoundEffect::load_track: Could not load sound. " << Mix_GetError() << endl;
		return false;
	}
	return true;
}

bool SoundEffect::set_track(void *f_sound)
{
	if (m_mchunk != nullptr)
		if (m_bShouldDeleteMusic)
		{
			Mix_FreeChunk(m_mchunk);
			delete m_mchunk;
		}
	Mix_Chunk *l_msound = (Mix_Chunk*)f_sound;
	m_mchunk = l_msound;
	m_bShouldDeleteMusic = false;
	return true;
}

void* SoundEffect::get_track()
{
	return (void*)m_mchunk;
}

bool SoundEffect::play_track(bool f_bInterupt, int f_iChannel)
{
	if(f_bInterupt || Mix_Playing(f_iChannel) == 0)
		Mix_PlayChannel(f_iChannel, m_mchunk, 0);
	return true;
}