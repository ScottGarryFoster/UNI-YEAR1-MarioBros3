#include "GameInstance.h"



GameInstance::GameInstance(SpriteBatch *f_sb, Audio *f_audio, int *f_iSelectMenu)
{
	setup_gameInstance(f_sb, f_audio);
	s_iGameSelectMenu = f_iSelectMenu;
}


GameInstance::~GameInstance()
{
}

bool GameInstance::setup_gameInstance(SpriteBatch *f_sb, Audio *f_audio)
{
	p_sb = f_sb;
	if (p_sb == nullptr)
		cout << "GameInstance Constructor: Spritebatch Nullptr." << endl;

	//Let's test to see if the sprite batch exists:
	try
	{
		p_sb->awake();
	}
	catch (...)
	{
		cout << "GameInstance Constructor: Spritebatch doesn't exist." << endl;
		throw;
	}
	p_audio = f_audio;
	if (p_audio == nullptr)
		cout << "GameInstance Constructor: Spritebatch Nullptr." << endl;

	//Let's test to see if the sprite batch exists:
	try
	{
		p_audio->awake();
	}
	catch (...)
	{
		cout << "GameInstance Constructor: Audio doesn't exist." << endl;
		throw;
	}
	return true;
}