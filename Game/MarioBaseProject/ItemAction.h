#pragma once
#include "LinkedList.h"
#include "Utility.h"

#include <string>

using namespace std;
class ItemAction
{
public:
	enum ItemActions : int {iaNone = 0, iaCoinInstant, iaMushroom, iaFeather};
private:
	bool m_bAction = false;

	//Setup:
	bool m_bGravity = true;
	ItemActions m_itemActionType = iaNone;
	int m_iVariableDifforence = 0;
	int m_iActualHeight = -1;

	//Use:
	float m_fCurrentX = -1;
	float m_fCurrentY = -1;
	float m_fOriginalX = -1;
	float m_fOriginalY = -1;

	bool b_bDireciton = false;
	float m_fGoalX = -1;
	float m_fGoalY = -1;
	float m_fGravityAdjustment = -9001;
	int m_iMovementStage = -1;//Used in some actions like feather
	int m_iFlipItemImage = 0;
	bool *s_bCanCollect = nullptr;

	//Items that use Gravity and move_character
	bool m_bMoveCharacterReaction = true;
	float m_fMoveX = 0;
	float m_fMoveY = 0;
	bool m_bDirectionIsRight = true;
	bool m_bHaveUsedMoveCharacter = false;

	bool m_bDelete = false;//If true you're sending the delete
public:
	ItemAction();
	virtual ~ItemAction();

	virtual bool Update(float f_deltaTime);

	//Sets
	void set_currentLocation(float f_fX, float f_fY) { m_fCurrentX = f_fX; m_fCurrentY = f_fY; }
	void set_actualHeight(int f_iNew) { m_iActualHeight = f_iNew; }
	void set_gravityAdjustment(float f_fNew) { m_fGravityAdjustment = f_fNew; }
	void set_moveCharacterReaction(bool f_bData) { m_bMoveCharacterReaction = f_bData; }
	void set_canCollect(bool *f_b) { s_bCanCollect = f_b; }

	//Gets
	float get_locationX() { return m_fCurrentX; }
	float get_locationY() { return m_fCurrentY; }
	int get_actualHeight(){return m_iActualHeight;}
	bool get_delete() { return m_bDelete; }
	bool get_gravity() { return m_bGravity; }
	float get_gravityAdjustment(){return m_fGravityAdjustment;}
	float get_originalX() { return m_fOriginalX; }
	float get_originalY() { return m_fOriginalY; }
	float get_moveX() { return m_fMoveX; }
	float get_moveY() { return m_fMoveY; }
	int get_flipImage() { return m_iFlipItemImage; }
	
	//Setup:
	bool setup_action(string f_sData);
	bool set_byString(string f_sPara, string f_sValue);
	bool set_intValue(string f_sValue, int *f_iDestination);
	bool set_intValue(int f_iValue, int *f_iDestination);

private:
	bool update_coinInstant(float f_deltaTime);
	bool update_mushroom(float f_deltaTime);
	bool update_feather(float f_deltaTime);
};

