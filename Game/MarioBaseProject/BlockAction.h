#pragma once
#include "LinkedList.h"
#include "Utility.h"
#include "Audio.h"

#include <string>

using namespace::std;

class BlockAction
{
public:
	enum actorType : int {baatPlayer = 0, baatEnemy, baatShell};
	enum graphicalEffect : int {bageNone = 0, bageNudge, bageNudgeWithSound, bageDestory};
protected:
	Audio *p_audio = nullptr;
private:
	bool m_bBlockAction = false;
	actorType m_character = baatPlayer;
	bool m_bDeleteMe = false;

	bool m_bDirectionTop = false;
	bool m_bDirectionLeft = false;
	bool m_bDirectionRight = false;
	bool m_bDirectionBottom = false;

	graphicalEffect m_graphicalEffect = bageNone;

	int m_iFrequency = 1;
	string m_sItem = "";
	int m_iCompletePointRow = -1;
	int m_iCompletePointCol = -1;
	int m_iResetCol = -1;//Set this when any movement animation is completed.
	int m_iResetRow = -1;//Set this when any movement animation is completed.
	
	float m_fXOriginal = 0;//Set by value on activate
	float m_fYOriginal = 0;//Set by value on activate
	//Generated and not set:
	bool m_bActivated = false;
	int m_iDirectionHit = 0;
	float m_fXModificationTarget = -1;
	float m_fYModificationTarget = -1;
	float m_fXModification = 0;
	float m_fYModification = 0;
	int m_iHeightWidth = 0;//Set by value on activate
	bool m_bReturnToOriginal = false;
	float m_fStrength = 0.5f;
	
	int m_iNeedToBeBig = 0;//0 - No requirement, 1 - Need to be big, 2 - Need to be small

	//Audio
	int m_iBumpSound = -1;
public:
	BlockAction();
	virtual ~BlockAction();

	bool Update(float f_deltaTime);
	
	bool setup_action(string f_sData);

	void set_audio(Audio *f_audio) { p_audio = f_audio; }
	bool set_byString(string f_sPara, string f_sValue);
	bool set_spriteCurrentLocation(float f_fX, float f_fY);
	bool set_widthHeight(int f_iWidthHeight) { m_iHeightWidth = f_iWidthHeight; return true; }
	void set_activated(bool f_b) { m_bActivated = f_b; }
	void add_frequency(int f_i) { m_iFrequency += f_i; }

	float get_locationXMod() { return m_fXModification; }
	float get_locationYMod() { return m_fYModification; }
	bool get_activated() { return m_bActivated; }
	int get_completedCol() { return m_iResetCol; }
	int get_completedRow() { return m_iResetRow; }
	int get_frequency() { return m_iFrequency; }
	string get_itemName() { return m_sItem; }

	bool get_deleteMe() { return m_bDeleteMe; }

	bool update_checkActivation(int f_iDirection, int f_iActor, bool f_bPlayerSize = false);

	bool update_action(float f_deltaTime);
	bool update_nudge(float f_deltaTime);
	bool update_destroy(float f_deltaTime);

	bool debug_action();
private:
	bool load_audio();

	bool play_bump();
};

