#include "Audio.h"



Audio::Audio()
{
}


Audio::~Audio()
{
	//
	if (m_llMusicPreload != nullptr)
	{
		LinkedList *l_llCurrentList = m_llMusicPreload;
		Sound *l_llSound = nullptr;
		for (int i = 0; i < m_llMusicPreload->length(); i++)
		{
			l_llSound = (Sound*)l_llCurrentList->get_data(nullptr, false);
			delete l_llSound;
			if (l_llCurrentList->get_ahead() == nullptr)
				break;
			l_llCurrentList = l_llCurrentList->get_ahead();
		}
		delete m_llMusicPreload;
	}
	if (m_llsoundeffectPreload != nullptr)
	{
		LinkedList *l_llCurrentList = m_llsoundeffectPreload;
		Sound *l_llSound = nullptr;
		for (int i = 0; i < m_llsoundeffectPreload->length(); i++)
		{
			l_llSound = (Sound*)l_llCurrentList->get_data(nullptr, false);
			delete l_llSound;
			if (l_llCurrentList->get_ahead() == nullptr)
				break;
			l_llCurrentList = l_llCurrentList->get_ahead();
		}
		delete m_llsoundeffectPreload;
	}
}

bool Audio::reset_currentPlaying()
{
	m_iCurrentMusicPlaying = -1;
	return true;
}

bool Audio::stop_allSoundEffects()
{
	for(int i = 0; i < m_iSoundEffectChannelsLength; i++)
		Mix_Pause(i);
	return true;
}

int Audio::add_music(string f_sLocation)
{
	Sound *l_sound = new Music();
	return add_track(f_sLocation, &m_llMusicPreload, l_sound);
}

int Audio::add_soundEffect(string f_sLocation)
{
	Sound *l_sound = new SoundEffect();
	return add_track(f_sLocation, &m_llsoundeffectPreload, l_sound);
}

int Audio::add_track(string f_sLocation, LinkedList **l_llDestination, Sound *l_sound)
{
	if (f_sLocation == "")
	{
		cout << "Audio::add_track: Location blank." << endl;
		delete l_sound;
		return -1;
	}
	if (!l_sound->load_track(f_sLocation))
	{
		cout << "int Audio::add_track: Could not add track: " << f_sLocation << endl;
		delete l_sound;
		return -1;
	}
	LinkedList *l_llTopOfList = nullptr;
	LinkedList *l_llCurrentElement = nullptr;
	if (*l_llDestination == nullptr)
	{
		*l_llDestination = new LinkedList();
		l_llTopOfList = *l_llDestination;
	}
	else
	{
		l_llTopOfList = *l_llDestination;
		l_llTopOfList->AddNewToEnd();
	}
	
	l_llCurrentElement = l_llTopOfList->ReadFromList(l_llTopOfList->length()-1);
	l_llCurrentElement->set_data((void*)l_sound, false);
	l_llCurrentElement->set_data(f_sLocation);

	return l_llTopOfList->length() - 1;
}

int Audio::find_music(string f_sLocation)
{
	return find_sound(f_sLocation, m_llMusicPreload);
}

LinkedList* Audio::find_music(int f_iTrackNumber)
{
	return find_sound(f_iTrackNumber, m_llMusicPreload);
}

int Audio::find_musicNeverFail(string f_sLocation)
{
	int l_iMusic = find_sound(f_sLocation, m_llMusicPreload, false);
	if (l_iMusic == -1)
		return add_music(f_sLocation);
	else
		return l_iMusic;
}

int Audio::find_soundEffect(string f_sLocation)
{
	return find_sound(f_sLocation, m_llsoundeffectPreload);
}

LinkedList* Audio::find_soundEffect(int f_iTrackNumber)
{
	return find_sound(f_iTrackNumber, m_llsoundeffectPreload);
}

int Audio::find_soundEffectNeverFail(string f_sLocation)
{
	int l_iSound = find_sound(f_sLocation, m_llsoundeffectPreload, false);
	if (l_iSound == -1)
		return add_soundEffect(f_sLocation);
	else
		return l_iSound;
}

int Audio::find_sound(string f_sLocation, LinkedList *f_llTop, bool f_bShowErrors)
{
	if (f_llTop == nullptr)
	{
		if(f_bShowErrors) cout << "Audio::find_sound: No sound loaded. " << endl;
		return -1;
	}
	LinkedList *l_llAnswer = f_llTop->search_list(f_sLocation);
	if (l_llAnswer == nullptr)
	{
		if (f_bShowErrors) cout << "Audio::find_sound: Could not find track." << endl;
		return -1;
	}
	return l_llAnswer->get_identifier();
}

LinkedList* Audio::find_sound(int f_iTrackNumber, LinkedList *f_llTop, bool f_bShowErrors)
{
	if (f_llTop == nullptr)
	{
		if (f_bShowErrors) cout << "Audio::find_sound: No sound loaded. " << endl;
		return nullptr;
	}
	LinkedList *l_llAnswer = f_llTop->ReadFromList(f_iTrackNumber);
	if (l_llAnswer == nullptr)
	{
		if (f_bShowErrors) cout << "Audio::find_sound: Could not find music." << endl;
		return nullptr;
	}
	return l_llAnswer;
}

bool Audio::play_music(int f_iTrackNumber, bool f_bForce)
{
	if (m_iCurrentMusicPlaying == f_iTrackNumber && f_bForce == false)
		return true;
	LinkedList *l_llTrack = find_music(f_iTrackNumber);
	if (l_llTrack == nullptr) return false;
	Sound* l_sound = cast_voidToSound(l_llTrack);
	if (l_sound == nullptr) return false;
	m_iCurrentMusicPlaying = f_iTrackNumber;
	return l_sound->play_track(true);
}

bool Audio::play_soundEffect(int f_iTrackNumber, int f_iChannel, bool f_bForce)
{
	LinkedList *l_llTrack = find_soundEffect(f_iTrackNumber);
	if (l_llTrack == nullptr) return false;
	Sound* l_sound = cast_voidToSound(l_llTrack);
	if (l_sound == nullptr) return false;
	if (f_iChannel > m_iSoundEffectChannelsLength) m_iSoundEffectChannelsLength = f_iChannel;
	if (f_iChannel == -1) f_iChannel = find_unusedChannel();
	if (f_iChannel == -1)
	{
		m_iSoundEffectChannelsLength++;
		f_iChannel = m_iSoundEffectChannelsLength - 1;
	}
	return l_sound->play_track(f_bForce, f_iChannel);
}

bool Audio::is_channelUsed(int f_iChannel)
{
	if (f_iChannel <= -1) return false;
	if (Mix_Playing(f_iChannel) == 0)
		return true;
	return false;
}

int Audio::find_unusedChannel()
{
	for (int c = 0; c < m_iSoundEffectChannelsLength; c++)
		if (Mix_Playing(c) == 0)
			return c;
	return -1;
}

Sound* Audio::cast_voidToSound(LinkedList *f_llData)
{
	if (f_llData == nullptr)
	{
		cout << "Audio::cast_voidToSound: Could not find linkedlist item." << endl;
		return nullptr;
	}
	void *l_void = f_llData->get_data(nullptr, false);
	if (l_void == nullptr)
	{
		cout << "Audio::cast_voidToSound: Could not find music." << endl;
		return nullptr;
	}
	Sound* l_sound = (Sound*)l_void;
	return l_sound;
}

bool Audio::pause_music()
{
	Mix_PauseMusic();
	return true;
}

bool Audio::resume_music()
{
	Mix_ResumeMusic();
	return true;
}