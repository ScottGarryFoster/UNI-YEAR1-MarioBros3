#include "ScoreOnScreen.h"



ScoreOnScreen::ScoreOnScreen()
{
}


ScoreOnScreen::~ScoreOnScreen()
{

}

bool ScoreOnScreen::Update(float f_deltaTime)
{
	if (m_iScoreCombo > 0)
	{
		m_fScoreTick += f_deltaTime;
		if (m_fScoreTick > 700)
		{
			m_fScoreTick = 0;
			m_iScoreCombo = 0;
		}
	}
	if (m_iScoreCombo > 0)
	{
		if (!m_bReachedPeak)
		{
			if(get_locationY() > m_fGoalY)
				move_location(0, -(f_deltaTime *  0.2f));

			if (get_locationY() <= m_fGoalY)
			{
				set_location(get_locationX(), m_fGoalY);
				m_bReachedPeak = true;
			}
		}
		else
		{
			set_disableGravity(false);
			Platformer_Character::Update(f_deltaTime);
		}
		
		if (m_iScoreCombo != m_iLastCombo)
		{
			set_singleLocation(19, 10 + m_iScoreCombo);
			set_visable(true);
			m_iLastCombo = m_iScoreCombo;

			set_disableGravity(true);
			set_gravityPull(-0.4f);
			//set_collision(0, 0, 1, 1);
		}
	}
	if (m_iScoreCombo <= 0)
	{
		m_iLastCombo = -1;
		set_disableGravity(true);
		set_visable(false);
	}
		
	return true;
}

bool ScoreOnScreen::new_score(float f_fX, float f_fY, bool f_bAdditive)
{
	if (!f_bAdditive)
	{
		m_iLastCombo = -1;
		set_disableGravity(true);
		set_visable(false);
		m_iScoreCombo = 1;
	}
	else
		m_iScoreCombo++;

	return new_scoreKnown(f_fX, f_fY, m_iScoreCombo);
}

bool ScoreOnScreen::new_scoreKnown(float f_fX, float f_fY, int f_iScore)
{
	m_fScoreTick = 0;

	m_fOriginalX = f_fX;
	m_fOriginalY = f_fY;

	m_fGoalY = m_fOriginalY - (get_actualSizeHeight() * 2.0f);
	m_bReachedPeak = false;

	set_location(f_fX, f_fY);

	if (m_iPoints != nullptr)
	{
		m_iScoreCombo = f_iScore;
		switch (m_iScoreCombo)
		{
		case 1:
			*m_iPoints += 100;
			break;
		case 2:
			*m_iPoints += 200;
			break;
		case 3:
			*m_iPoints += 400;
			break;
		case 4:
			*m_iPoints += 800;
			break;
		case 5:
			*m_iPoints += 1000;
			break;
		case 6:
			*m_iPoints += 2000;
			break;
		case 7:
			*m_iPoints += 4000;
			break;
		case 8:
			*m_iPoints += 8000;
			break;
		default:
			*m_iPoints += 10000;
			break;
		}
	}
	return true;
}