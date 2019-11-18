#include "SMB_LevelEnd.h"
#include "SMB_PlayerPlatformer.h"


SMB_LevelEnd::SMB_LevelEnd()
{
}


SMB_LevelEnd::~SMB_LevelEnd()
{
}


bool SMB_LevelEnd::Update(float f_deltaTime)
{
	if (!m_bLoaded)
		load_content();

	update_textAndFont();

	bool l_bItsTheEndOfTheLevel = false;
	if(m_iPlayerOneFinishedLevel != nullptr)
		if(*m_iPlayerOneFinishedLevel > 0)
			if (m_playerOne != nullptr)
			{
				m_playerOne->set_simulateButtonPress(Controls::GameKeys::DPAD_RIGHT, true);
				l_bItsTheEndOfTheLevel = true;
				m_playerOne->set_deathstate(SMB_PlayerPlatformer::DeathState::dsComplete);
			}
	if (m_iPlayerTwoFinishedLevel != nullptr)
		if (*m_iPlayerTwoFinishedLevel > 0)
			if (m_playerTwo != nullptr)
			{
				m_playerTwo->set_simulateButtonPress(Controls::GameKeys::DPAD_RIGHT, true);
				l_bItsTheEndOfTheLevel = true;
				m_playerTwo->set_deathstate(SMB_PlayerPlatformer::DeathState::dsComplete);
			}

	if(l_bItsTheEndOfTheLevel)
		update_endOfLevel(f_deltaTime);
	return true;
}

bool SMB_LevelEnd::Draw()
{
	if (m_sprlTopLine != nullptr)
		m_sprlTopLine->RenderAsLetters(p_sb->get_renderer(), nullptr, "Course Clear !");

	if(m_sprlSecondLine != nullptr && m_sFirstPlayerText != "")
		m_sprlSecondLine->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sFirstPlayerText);

	if (m_sprlThirdLine != nullptr && m_sSecondPlayerText != "")
		m_sprlThirdLine->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sSecondPlayerText);

	if (m_sprTopPlayerCard != nullptr)
		m_sprTopPlayerCard->RenderToScreen(p_sb->get_renderer());

	if (m_sprBottomPlayerCard != nullptr)
		m_sprBottomPlayerCard->RenderToScreen(p_sb->get_renderer());
	return true;
}

bool SMB_LevelEnd::load_content()
{
	m_tsmLetters = new TextureSelfManaged();
	if (p_sb != nullptr)
		if (!m_tsmLetters->LoadTextureFromLocation(p_sb->get_renderer(), "Images/Mario3/Letters.png"))
			cout << "SMB_LevelEnd::load_content: Couldn't load: " << "Images/Mario3/Letters.png" << endl;

	m_sprlTopLine = new SprLetters();
	m_sprlTopLine->set_singleSize(16, 16);
	m_sprlTopLine->set_scaleSize(2);
	m_sprlTopLine->set_textureSelfManageFromPointer(m_tsmLetters);
	m_sprlTopLine->set_location(450, 100);
	m_sprlTopLine->set_lettersRow(0);
	m_sprlTopLine->set_lettersCol(0);
	m_sprlTopLine->set_numbersRow(1);
	m_sprlTopLine->set_numbersCol(0);

	m_sprlSecondLine = new SprLetters();
	m_sprlSecondLine->set_singleSize(16, 16);
	m_sprlSecondLine->set_scaleSize(2);
	m_sprlSecondLine->set_textureSelfManageFromPointer(m_tsmLetters);
	m_sprlSecondLine->set_location(350, 200);
	m_sprlSecondLine->set_lettersRow(0);
	m_sprlSecondLine->set_lettersCol(0);
	m_sprlSecondLine->set_numbersRow(1);
	m_sprlSecondLine->set_numbersCol(0);

	m_sprlThirdLine = new SprLetters();
	m_sprlThirdLine->set_singleSize(16, 16);
	m_sprlThirdLine->set_textureSelfManageFromPointer(m_tsmLetters);
	m_sprlThirdLine->set_location(350, 300);
	m_sprlThirdLine->set_scaleSize(2);
	m_sprlThirdLine->set_lettersRow(0);
	m_sprlThirdLine->set_lettersCol(0);
	m_sprlThirdLine->set_numbersRow(1);
	m_sprlThirdLine->set_numbersCol(0);

	m_sprTopPlayerCard = new Sprite();
	if (p_sb != nullptr)
		if (!m_sprTopPlayerCard->LoadTextureFromFile(p_sb->get_renderer(), "Images/Mario3/LowerUI.png"))
			cout << "SMB_LevelEnd::load_content: Couldn't load: " << "Images/Mario3/LowerUI.png" << endl;
	m_sprTopPlayerCard->set_singleSize(48, 56);
	m_sprTopPlayerCard->set_singleLocation(2, 7);
	m_sprTopPlayerCard->set_location(875, 170);
	m_sprTopPlayerCard->set_scaleSize(1.5f);
	m_sprTopPlayerCard->set_visable(false);

	m_sprBottomPlayerCard = new Sprite();
	m_sprBottomPlayerCard->set_textureSelfManageFromPointer(m_sprTopPlayerCard->get_textureSelfManageFromPointer());
	m_sprBottomPlayerCard->set_singleSize(48, 56);
	m_sprBottomPlayerCard->set_singleLocation(2, 7);
	m_sprBottomPlayerCard->set_location(875, 270);
	m_sprBottomPlayerCard->set_scaleSize(1.5f);
	m_sprBottomPlayerCard->set_visable(false);

	//Audio
	if (p_audio != nullptr)
	{
		m_iMusicTrack = p_audio->find_soundEffectNeverFail("Audio/Music/smb3_level_clear.wav");
	}

	m_playerOne->set_didDrawThisFrame(true);
	m_playerTwo->set_didDrawThisFrame(true);

	m_bLoaded = true;
	return true;
}

bool SMB_LevelEnd::update_textAndFont()
{
	if (m_sFirstPlayerText == "")
	{
		if (*m_iPlayerOneFinishedLevel > 0)
		{
			//Red text:
			m_sprlSecondLine->set_lettersRow(4);
			m_sprlSecondLine->set_lettersCol(0);
			m_sprlSecondLine->set_numbersRow(5);
			m_sprlSecondLine->set_numbersCol(0);
			m_sFirstPlayerText = "Mario got a card";

			m_sprTopPlayerCard->set_visable(true);
			m_sprTopPlayerCard->set_singleLocation(2, 7 + *m_iPlayerOneFinishedLevel);
		}
		else
		{
			//Green text:
			m_sprlSecondLine->set_lettersRow(8);
			m_sprlSecondLine->set_lettersCol(0);
			m_sprlSecondLine->set_numbersRow(9);
			m_sprlSecondLine->set_numbersCol(0);
			m_sFirstPlayerText = "Luigi got a card";

			m_sprTopPlayerCard->set_visable(true);
			m_sprTopPlayerCard->set_singleLocation(3, 7 + *m_iPlayerTwoFinishedLevel);
		}
	}

	if (m_sSecondPlayerText == "")
	{
		if (*m_iPlayerOneFinishedLevel > 0 && *m_iPlayerTwoFinishedLevel > 0)
		{
			//Green text:
			m_sprlThirdLine->set_lettersRow(8);
			m_sprlThirdLine->set_lettersCol(0);
			m_sprlThirdLine->set_numbersRow(9);
			m_sprlThirdLine->set_numbersCol(0);
			m_sSecondPlayerText = "Luigi got a card";

			m_sprBottomPlayerCard->set_visable(true);
			m_sprBottomPlayerCard->set_singleLocation(3, 7 + *m_iPlayerTwoFinishedLevel);
		}
	}
	return true;
}

bool SMB_LevelEnd::update_endOfLevel(float f_deltaTime)
{
	if (p_audio != nullptr && m_iMusicTrack > -1 && !m_bStartPlay)
	{
		p_audio->pause_music();
		p_audio->stop_allSoundEffects();
		p_audio->play_soundEffect(m_iMusicTrack);
	}
	m_bStartPlay = true;


	m_fCurrentTimePassed += f_deltaTime;
	if (m_fMusicLength > m_fCurrentTimePassed)
		return true;

	bool l_bPlayerOneComplete = false;

	if (m_playerOne != nullptr)
	{
		if(!m_playerOne->get_didDrawThisFrame())
			l_bPlayerOneComplete = true;
	}
	else
		l_bPlayerOneComplete = true;

	bool l_bPlayerTwoComplete = false;

	if (m_playerTwo != nullptr)
	{
		if (!m_playerTwo->get_didDrawThisFrame())
			l_bPlayerTwoComplete = true;
	}
	else
		l_bPlayerTwoComplete = true;

	if (l_bPlayerOneComplete && l_bPlayerTwoComplete)
		if (s_iEndOfLevel != nullptr)
			*s_iEndOfLevel = 1;

	return true;
}