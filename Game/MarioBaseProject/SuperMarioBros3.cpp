#include "SuperMarioBros3.h"
#include <iostream>


SuperMarioBros3::SuperMarioBros3()
{
	
}


SuperMarioBros3::~SuperMarioBros3()
{
	delete m_level;
}

bool SuperMarioBros3::LoadContent(int f_iLevel)
{
	m_iTimeLeft = 300;

	m_level = new SMB_Level(m_sb, m_controls);
	m_level->set_worldNumber(1);
	m_level->set_levelNumber(f_iLevel);
	m_level->set_coinsPointer(m_iCoins);
	m_level->set_livesPointer(m_iLives);
	m_level->set_pointsPointer(&m_iPoints);
	m_level->set_tilemap("1_"+to_string(f_iLevel)+".txt");
	m_level->set_tileset("Basic");
	m_level->set_numberOfPlayers(2);
	m_level->set_endOfLevel(&m_iEndOfLevel);
	m_level->set_audio(m_audio);

	m_level->LoadContent();

	return true;
}

bool SuperMarioBros3::Update(float f_deltaTime)
{
	if (*s_iGameSelectMenu == 0)
	{
		m_level->Update(f_deltaTime);
		if (m_iEndOfLevel == 0)//They Died
		{
			for (int i = 0; i < 2; i++)
				m_iLives[i] -= 1;

			m_iEndOfLevel = -1;
			delete m_level;
			m_level = nullptr;
			LoadContent(m_iCurrentLevel);
		}
		else if (m_iEndOfLevel == 1)//They completed
		{
			m_iEndOfLevel = -1;
			delete m_level;
			m_level = nullptr;
			load_nextLevel();
		}
	}

	update_gameSelectMenu();
	return true;
}

void SuperMarioBros3::Draw()
{
	if(m_level != nullptr)
		m_level->Draw();
}
bool SuperMarioBros3::update_gameSelectMenu()
{
	if (*s_iGameSelectMenu > 0) return false;
	if (m_controls->get_menuInput() == Controls::miKeyboard)
	{
		m_iMouseStateRecorded = 0;

		if (m_controls->isGameKeyDown(Controls::BTN_OPTION, 1))
		{
			if (m_bReadyForGameSelectEsc)
				*s_iGameSelectMenu = 1;
			
			m_bReadyForGameSelectEsc = false;
		}
		else
			m_bReadyForGameSelectEsc = true;
	}
	else if (m_controls->get_menuInput() == Controls::miMouse)
	{
		update_gameSelectMouse();
	}

	return true;
}

bool SuperMarioBros3::update_gameSelectMouse()
{
	if (m_cMouseInfo == nullptr)
	{
		m_cMouseInfo = m_controls->get_mouseInfo();
		m_iMouseStateRecorded = 0;
	}

	if (m_cMouseInfo->state == Controls::mouseInfo::msLeft)
	{
		if (m_iMouseStateRecorded > 0)//Not the first click since switching
			m_iMouseStateRecorded = 1;
	}
	else if (m_cMouseInfo->state == Controls::mouseInfo::msNone)
	{
		if (m_iMouseStateRecorded == 1)
		{
			*s_iGameSelectMenu = 1;
		}
		m_iMouseStateRecorded = 2;
	}
	else
		m_iMouseStateRecorded = 2;
	return true;
}

bool SuperMarioBros3::load_nextLevel()
{
	m_iCurrentLevel++;
	if (m_iCurrentLevel > m_iMaxLevel) m_iCurrentLevel = 1;
	return LoadContent(m_iCurrentLevel);
}