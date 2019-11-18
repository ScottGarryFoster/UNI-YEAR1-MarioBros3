#pragma once
#include "Platformer_Character.h"
#include "SMB_PlayerPlatformer.h"
#include "SprLinearAnimation.h"
#include "ItemAction.h"
#include "ScoreOnScreen.h"

#include <string>

using namespace std;

class Item : public Platformer_Character
{
private:
	bool m_bDebug = false;
	bool m_bDelete = false;
	SprLinearAnimation *m_sprLinearAnimation = nullptr;
	ItemAction *m_itemAction = nullptr;
	string m_sItemName = "";//Sent from our overlord the Item Manager

	SMB_PlayerPlatformer *m_players[2] = { nullptr };
	ScoreOnScreen *m_scoreOnScreen = nullptr;
	int m_playersLength = 2;

	bool m_bScoreOnDelete = false;
	bool m_bScoreAdditive = true;

	bool m_bHaveSetup = false;
	bool m_bCollectable = false;
	bool m_bDestoryNotOnScreen = true;//Will destory if not on screen
	bool m_bDrawBelowSpecialBlocks = false;
	bool m_bAlwaysCollectable = false;
public:
	Item(SMB_PlayerPlatformer *f_players[2], int f_iNumberOfPlayers);
	virtual ~Item();
	virtual bool Update(float f_deltaTime);

	bool load_fromFile(string f_sLocation);

	void set_intialLocation(float f_fX, float f_fY);
	void set_scoreOnScreen(ScoreOnScreen *f_scoreOnScreen) { m_scoreOnScreen = f_scoreOnScreen; }
	void set_itemName(string f_sItemName) { m_sItemName = f_sItemName; }

	//GET
	bool get_delete() { return m_bDelete; }
	bool get_collectable() { return m_bCollectable; }
	string get_itemName() { return m_sItemName; }
	bool get_drawBelowSpecialBlocks() { return m_bDrawBelowSpecialBlocks; }

	bool setup_meta(string f_sData);
	bool set_byString(string f_sParameter, string f_sValue);
private:
	bool update_action(float f_deltaTime);
	bool update_animation(float f_deltaTime);
};

