#pragma once
#include "SpecialBlock.h"

#include <iostream>
#include <string>

using namespace std;

class SMB_PlayerPlatformer;//Needs to know of this without implimentation

class EndOfLevelBlock : public SpecialBlock
{
private:
	bool m_bLoaded = false;

	float m_fAnimationTick = 0;
	float m_fAnimationTicks = 100;
	int m_iNumberOfAnimations = 3;
	
	bool m_bTriggered = false;
	int m_iTriggeredCard = 0;
	bool m_bFinshedMovingUp = false;
	int m_iPlayerTriggered = -1;

	int *m_iPlayerOneFinishedLevel = nullptr;
	int *m_iPlayerTwoFinishedLevel = nullptr;
public:
	EndOfLevelBlock();
	virtual ~EndOfLevelBlock();

	virtual bool Update(float f_deltaTime);

	virtual void set_playerOneEndCard(int *f_iCard) { m_iPlayerOneFinishedLevel = f_iCard; }//Used only by end of level
	virtual void set_playerTwoEndCard(int *f_iCard) { m_iPlayerTwoFinishedLevel = f_iCard; }//Used only by end of level

	virtual int get_drawThisTileUnderMe() { return 20; }

	virtual bool update_actionActivationPolymorphVersion(SDL_Rect f_rectActor, int f_iDirection, int f_iActor, int f_iAdditionalNumber, SMB_PlayerPlatformer *f_player);
private:
	bool load_endOfLevel();

	bool update_graphic(float f_deltaTime);
	bool update_graphicFlicker(float f_deltaTime);
	bool update_floatUp(float f_deltaTime);
	bool update_whiteRotate(float f_deltaTime);
};

