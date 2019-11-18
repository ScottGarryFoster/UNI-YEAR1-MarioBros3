#include "ItemAction.h"



ItemAction::ItemAction()
{
}


ItemAction::~ItemAction()
{
}

bool ItemAction::Update(float f_deltaTime)
{
	if (m_fOriginalX == -1) m_fOriginalX = m_fCurrentX;
	if (m_fOriginalY == -1) m_fOriginalY = m_fCurrentY;
	switch (m_itemActionType)
	{
	case iaCoinInstant:
		update_coinInstant(f_deltaTime);
		return true;
	case iaMushroom:
		update_mushroom(f_deltaTime);
		return true;
	case iaFeather:
		update_feather(f_deltaTime);
		return true;
	}
	return true;
}

bool ItemAction::update_coinInstant(float f_deltaTime)
{
	if (m_fGoalX == -1)
	{
		m_bGravity = true;
		m_fCurrentY -= m_iActualHeight;
		m_fGoalX = m_fOriginalX;
		m_fGoalY = m_fOriginalY - (m_iActualHeight * 4.5f);
	}
	if (!b_bDireciton)
	{
		if (m_fCurrentY > m_fGoalY)
			m_fCurrentY -= f_deltaTime * 0.75f;
		
		if (m_fCurrentY <= m_fGoalY)
		{
			m_fCurrentY = m_fGoalY;
			b_bDireciton = true;
			m_fGoalY = m_fOriginalY - (m_iActualHeight * 2);
			m_bGravity = false;
			m_fGravityAdjustment = -0.1f;
		}
	}
	else
	{
		/*if (m_fCurrentY < m_fGoalY)
			m_fCurrentY += f_deltaTime * 0.75f;
		*/
		if (m_fCurrentY >= m_fGoalY)
		{
			m_bDelete = true;
		}
	}
	return true;
}

bool ItemAction::update_mushroom(float f_deltaTime)
{
	if (m_fGoalX == -1)
	{
		m_bGravity = true;
		m_fGoalX = m_fOriginalX;
		m_fGoalY = m_fOriginalY - (m_iActualHeight * 1);
	}

	if (m_bGravity)
	{
		if (m_fCurrentY > m_fGoalY)
			m_fCurrentY -= f_deltaTime * 0.2f;

		if(m_fCurrentY == m_fGoalY)
			m_bGravity = false;

		if (m_fCurrentY < m_fGoalY)
		{
			m_fCurrentY = m_fGoalY;
		}
	}
	else
	{
		//cout << "YH" << endl;
		if(m_fCurrentX == 0 && m_bHaveUsedMoveCharacter)
			m_bDirectionIsRight = true;
		m_bHaveUsedMoveCharacter = true;

		if (!m_bMoveCharacterReaction)
		{
			if (m_bDirectionIsRight)
				m_bDirectionIsRight = false;
			else
				m_bDirectionIsRight = true;
		}
		if (m_bDirectionIsRight)
			m_fMoveX = f_deltaTime * 0.1f;
		else
			m_fMoveX = -(f_deltaTime * 0.1f);
	}
	return true;
}

bool ItemAction::update_feather(float f_deltaTime)
{
	if (m_fGoalY == -1)
	{
		m_bGravity = true;
		m_fGoalX = m_fOriginalX;
		m_fGoalY = m_fOriginalY - (m_iActualHeight * 3);
		m_iMovementStage = -1;
		if (s_bCanCollect != nullptr)
			*s_bCanCollect = true;
	}
	if (m_iMovementStage == -1)
	{
		if (m_fCurrentY > m_fGoalY)
			m_fCurrentY -= f_deltaTime * 0.5f;

		if (m_fCurrentY == m_fGoalY)
		{
			m_iMovementStage++;
			m_fGoalX = -1;
		}
			
		if (m_fCurrentY < m_fGoalY)
		{
			m_fCurrentY = m_fGoalY;
		}
	}
	else
	{
		if (m_fGoalX == -1)
		{
			if (m_iMovementStage % 2 == 0)
			{
				m_fGoalX = m_fOriginalX + (m_iActualHeight * 2);
				m_iFlipItemImage = 1;
			}
			else
			{
				m_fGoalX = m_fOriginalX;
				m_iFlipItemImage = 0;
			}

			m_fGoalY = m_fCurrentY + (m_iActualHeight * 0.75f);
		}

		if (m_fCurrentY < m_fGoalY)
			m_fCurrentY += f_deltaTime * 0.18f;

		if (m_fCurrentX > m_fGoalX)
		{
			m_fCurrentX -= f_deltaTime * 0.18f;
			if (m_fCurrentX < m_fGoalX)
				m_fCurrentX = m_fGoalX;
		}
		else if (m_fCurrentX < m_fGoalX)
		{
			m_fCurrentX += f_deltaTime * 0.18f;
			if(m_fCurrentX > m_fGoalX)
				m_fCurrentX = m_fGoalX;
		}

		if (m_fCurrentY == m_fGoalY && m_fCurrentX == m_fGoalX)
		{
			m_iMovementStage++;
			m_fGoalX = -1;
		}

		if (m_fCurrentY > m_fGoalY)
			m_fCurrentY = m_fGoalY;
	}
	return true;
}

bool ItemAction::setup_action(string f_sData)
{
	LinkedList *l_llLines = Utility::SplitStringToLL(f_sData, '~');
	if (l_llLines->length() <= 1) return false;
	string l_sWholeLine = "";
	for (int line = 0; line < l_llLines->length(); line++)
	{
		l_sWholeLine = l_llLines->read_data(line, l_sWholeLine);
		//cout << "l_sWholeLine: " << l_sWholeLine << endl;
		if (l_sWholeLine == "") continue;
		if (!Utility::SearchStringForChar(l_sWholeLine, ' ')) continue;
		string f_sParameter = l_sWholeLine, f_sValue = "";
		if (!Utility::SplitStringAtPoint(&f_sParameter, &f_sValue, ' '))continue;
		if (set_byString(f_sParameter, f_sValue)) m_bAction = true;
	}
	delete l_llLines;

	//cout << "m_bGravity: " << m_bGravity << " m_itemActionType: " << m_itemActionType << " m_iVariableDifforence: " << m_iVariableDifforence << endl;

	return true;
}

bool ItemAction::set_byString(string f_sPara, string f_sValue)
{
	string f_sParaUpper = Utility::StringToUpperCase(f_sPara);//Shouldn't be case sensative
	string f_sValueUpper = Utility::StringToUpperCase(f_sValue);//Shouldn't be case sensative

	//cout << "f_sParaUpper: " << f_sParaUpper << ": " << f_sValueUpper << endl;

	if (f_sParaUpper == "" || f_sValueUpper == "")
		return false;
	else if (f_sParaUpper == "VARIABLEDIFFORENCE:")
	{
		set_intValue(f_sValue, &m_iVariableDifforence);
		return true;
	}
	else if (f_sParaUpper == "GRAVITY:")
	{
		if (f_sValueUpper == "FALSE")
			m_bGravity = false;
		return true;
	}
	else if (f_sParaUpper == "MOTION:")
	{
		if (f_sValueUpper == "COININSTANT") m_itemActionType = iaCoinInstant;
		if (f_sValueUpper == "MUSHROOM")m_itemActionType = iaMushroom;
		if (f_sValueUpper == "FEATHER")m_itemActionType = iaFeather;
		return true;
	}
	return false;
}

bool ItemAction::set_intValue(string f_sValue, int *f_iDestination)
{
	int l_iNumber = Utility::convert_stringToInt(f_sValue);
	if (l_iNumber == -1) return false;
	return set_intValue(l_iNumber, f_iDestination);
}
bool ItemAction::set_intValue(int f_iValue, int *f_iDestination)
{
	*f_iDestination = f_iValue;
	return true;
}