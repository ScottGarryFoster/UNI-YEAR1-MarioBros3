#include "EndOfLevelBlock.h"
#include "SMB_PlayerPlatformer.h"
#include "ParticleManager.h"


EndOfLevelBlock::EndOfLevelBlock()
{
}


EndOfLevelBlock::~EndOfLevelBlock()
{
}

bool EndOfLevelBlock::Update(float f_deltaTime)
{
	if (p_sb == nullptr) return false;
	if(!m_bLoaded)
		load_endOfLevel();
	
	update_graphic(f_deltaTime);
	return true;
}

bool EndOfLevelBlock::load_endOfLevel()
{
	//Setup Image
	if (!LoadTextureFromFile(p_sb->get_renderer(), "Images/Mario3/endCards.png"))
		cout << "EndOfLevelBlock::load_endOfLevel: Could not load " << "Images/Mario3/endCards.png" << endl;
	setup_locationInner();//Ensure we've moved the block to the correct location
	set_screenSize(26 * get_scaleSize(), 26 * get_scaleSize());
	set_singleSize(26, 26);
	move_location(-5 * get_scaleSize(), -5 * get_scaleSize());//Because this block is a tad bigger we need to do this
	set_renderAbovePlayer(false);


	m_bLoaded = true;
	return true;
}

bool EndOfLevelBlock::update_actionActivationPolymorphVersion(SDL_Rect f_rectActor, int f_iDirection, int f_iActor, int f_iAdditionalNumber, SMB_PlayerPlatformer *f_player)
{
	if(m_bTriggered) return false;
	if(f_iActor != 0) return false;
	SDL_Rect l_rectMine = { get_locationX(), get_locationY(), get_actualSizeWidth(), get_actualSizeHeight() };
	if (Utility::ReturnCollision(f_rectActor, l_rectMine))
	{
		if (!f_player->get_triggeredEndCard())
		{
			m_bTriggered = true;
			m_iPlayerTriggered = f_iAdditionalNumber + 1;
			f_player->set_triggeredEndCard(true);
		}
	}
	return true;
}

bool EndOfLevelBlock::update_graphic(float f_deltaTime)
{
	if (!m_bTriggered)
		update_graphicFlicker(f_deltaTime);
	else if (!m_bFinshedMovingUp)
		update_floatUp(f_deltaTime);
	//else
	//	cout << "out" << endl;
	return true;
}

bool EndOfLevelBlock::update_graphicFlicker(float f_deltaTime)
{
	m_fAnimationTick += f_deltaTime;

	for (int i = 1; i <= 3; i++)
		if (m_fAnimationTick < m_fAnimationTicks * i)
		{
			set_singleLocation(0, i);
			m_iTriggeredCard = i;
			break;
		}


	if (m_fAnimationTick > m_fAnimationTicks * m_iNumberOfAnimations)
		m_fAnimationTick -= m_fAnimationTicks * m_iNumberOfAnimations;
	return true;
}

bool EndOfLevelBlock::update_floatUp(float f_deltaTime)
{
	if (!get_didDrawThisFrame())
	{
		set_visable(false);
		m_bFinshedMovingUp = true;
		if (m_iPlayerTriggered == 1 && m_iPlayerOneFinishedLevel != nullptr)
		{
			*m_iPlayerOneFinishedLevel = m_iTriggeredCard;
		}
		else if(m_iPlayerTriggered == 2 && m_iPlayerTwoFinishedLevel != nullptr)
			*m_iPlayerTwoFinishedLevel = m_iTriggeredCard;
	}
	update_whiteRotate(f_deltaTime);
	move_location(0, (0.4f * f_deltaTime) * -1);
	return true;
}

bool EndOfLevelBlock::update_whiteRotate(float f_deltaTime)
{
	m_fAnimationTick += f_deltaTime;

	if (m_fAnimationTick < m_fAnimationTicks * 1)
		set_singleLocation(1, m_iTriggeredCard);
	else if (m_fAnimationTick < m_fAnimationTicks * 2)
		set_singleLocation(2, m_iTriggeredCard);
	else if (m_fAnimationTick < m_fAnimationTicks * 3)
		set_singleLocation(1, 0);
	else if (m_fAnimationTick < m_fAnimationTicks * 4)
		set_singleLocation(2, m_iTriggeredCard);

	if (m_fAnimationTick > m_fAnimationTicks * (m_iNumberOfAnimations + 1))
		m_fAnimationTick -= m_fAnimationTicks * (m_iNumberOfAnimations + 1);

	return true;
}