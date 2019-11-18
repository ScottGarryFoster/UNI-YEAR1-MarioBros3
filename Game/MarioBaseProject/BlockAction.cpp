#include "BlockAction.h"



BlockAction::BlockAction()
{
}

BlockAction::~BlockAction()
{
}

bool BlockAction::Update(float f_deltaTime)
{
	if (m_bActivated)
		update_action(f_deltaTime);
	return true;
}

bool BlockAction::update_action(float f_deltaTime)
{
	switch (m_graphicalEffect)
	{
	case bageNudge:
	case bageNudgeWithSound:
		update_nudge(f_deltaTime);
		break;
	case bageDestory:
		update_destroy(f_deltaTime);
		break;
	}
	if (m_iFrequency == 0)
	{
		if(m_iCompletePointRow != -1)
			m_iResetRow = m_iCompletePointRow;
		if (m_iCompletePointCol != -1)
			m_iResetCol = m_iCompletePointCol;
	}
	return true;
}

bool BlockAction::update_nudge(float f_deltaTime)
{
	//Setup:
	if ((m_iDirectionHit == 4 || m_iDirectionHit == 6) && m_fXModificationTarget == -1)
	{
		if (m_graphicalEffect == bageNudgeWithSound) play_bump();//Handle bump audio
		m_fXModification = m_fXOriginal; m_fYModification = m_fYOriginal;
		if (m_iDirectionHit == 4) m_fXModificationTarget = m_fXModification - (m_iHeightWidth * 0.5f);
		if (m_iDirectionHit == 6) m_fXModificationTarget = m_fXModification + (m_iHeightWidth * 0.5f);
	}
	else if ((m_iDirectionHit == 8 || m_iDirectionHit == 2) && m_fYModificationTarget == -1)
	{
		if (m_graphicalEffect == bageNudgeWithSound) play_bump();//Handle bump audio
		m_fXModification = m_fXOriginal; m_fYModification = m_fYOriginal;
		if (m_iDirectionHit == 8) m_fYModificationTarget = m_fYModification + (m_iHeightWidth * 0.5f);
		if (m_iDirectionHit == 2) m_fYModificationTarget = m_fYModification - (m_iHeightWidth * 0.5f);
	}
	//Climb:
	if (m_iDirectionHit == 2)
	{
		//FROM THE BOTTOM
		if (!m_bReturnToOriginal)
		{
			if (m_fYModification > m_fYModificationTarget)
				m_fYModification -= m_fStrength * f_deltaTime;
			else
				m_bReturnToOriginal = true;
		}
		else
			if (m_fYModification < m_fYOriginal)
				m_fYModification += m_fStrength * f_deltaTime;

		if (m_bReturnToOriginal)
			if (m_fYModification >= m_fYOriginal)
			{
				m_fYModification = m_fYOriginal;
				m_fXModificationTarget = -1;
				m_fYModificationTarget = -1;
				m_bActivated = false;
				m_bReturnToOriginal = false;
			}
				
	}
	return true;
}

bool BlockAction::update_destroy(float f_deltaTime)
{
	m_bDeleteMe = true;
	return true;
}

bool BlockAction::setup_action(string f_sData)
{
	LinkedList *l_llLines = Utility::SplitStringToLL(f_sData, '~');
	if (l_llLines->length() <= 1) return false;
	string l_sWholeLine = "";
	for (int line = 0; line < l_llLines->length(); line++)
	{
		l_sWholeLine = l_llLines->read_data(line, l_sWholeLine);
		if (l_sWholeLine == "") continue;
		if (!Utility::SearchStringForChar(l_sWholeLine, ' ')) continue;
		string f_sParameter = l_sWholeLine, f_sValue = "";
		if (!Utility::SplitStringAtPoint(&f_sParameter, &f_sValue, ' '))continue;
		if (set_byString(f_sParameter, f_sValue)) m_bBlockAction = true;
	}
	delete l_llLines;

	load_audio();

	return true;
}

bool BlockAction::set_byString(string f_sPara, string f_sValue)
{
	string f_sParaUpper = Utility::StringToUpperCase(f_sPara);//Shouldn't be case sensative
	string f_sValueUpper = Utility::StringToUpperCase(f_sValue);//Shouldn't be case sensative

	if (f_sParaUpper == "")
		return false;
	else if (f_sParaUpper == "CHARACTER:")
	{
		if (f_sValueUpper == "PLAYER") m_character = baatPlayer;
		else if (f_sValueUpper == "SHELL") m_character = baatShell;
		return true;
	}
	else if (f_sParaUpper == "DIRECTION:")
	{
		if (f_sValueUpper == "TOP") m_bDirectionTop = true;
		else if (f_sValueUpper == "LEFT") m_bDirectionLeft = true;
		else if (f_sValueUpper == "RIGHT") m_bDirectionRight = true;
		else if (f_sValueUpper == "BOTTOM") m_bDirectionBottom = true;
		return true;
	}
	else if (f_sParaUpper == "GRAPHICAL:")
	{
		if (f_sValueUpper == "NONE") m_graphicalEffect = bageNone;
		else if (f_sValueUpper == "NUDGE") m_graphicalEffect = bageNudge;
		else if (f_sValueUpper == "NUDGEWITHSOUND") m_graphicalEffect = bageNudgeWithSound;
		else if (f_sValueUpper == "DESTROY") m_graphicalEffect = bageDestory;
		return true;
	}
	else if (f_sParaUpper == "FREQUENCY:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iFrequency = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "COMPLETEPOINTROW:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iCompletePointRow = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "COMPLETEPOINTCOL:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iCompletePointCol = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "ITEM:")
	{
		m_sItem = f_sValueUpper;
		return true;
	}
	else if (f_sParaUpper == "REQUIREMENT:")
	{
		if (f_sValueUpper == "BIG") { m_iNeedToBeBig = 1; return true; }
		if (f_sValueUpper == "SMALL") { m_iNeedToBeBig = 2; return true; }
		return false;
	}
	return false;
}

bool BlockAction::load_audio()
{
	if (p_audio == nullptr) return false;

	if (m_iBumpSound == -1)
		m_iBumpSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/Mario3/smb3_bump.wav");
	
	return true;
}

bool BlockAction::play_bump()
{
	if (m_iBumpSound > -1)
		p_audio->play_soundEffect(m_iBumpSound);
	return true;
}

bool BlockAction::update_checkActivation(int f_iDirection, int f_iActor, bool f_bPlayerSize)
{
	if (m_bActivated) return false;
	if (m_iFrequency < 1) return false;
	if (f_iActor != (int)m_character) return false;
	if (f_iActor == (int)baatPlayer)
	{
		if (m_iNeedToBeBig == 1 && !f_bPlayerSize)
			return false;
		else if(m_iNeedToBeBig == 2 && f_bPlayerSize)
			return false;
	}

	switch (f_iDirection)
	{
	case 8:
		if (m_bDirectionTop == false) return false;
		break;
	case 2:
		if (m_bDirectionBottom == false) return false;
		break;
	case 4:
		if (m_bDirectionLeft == false) return false;
		break;
	case 6:
		if (m_bDirectionRight == false) return false;
		break;
	}
	//Supposing all the above didn't stop us getting here, we are now active:
	m_bActivated = true;
	m_iDirectionHit = f_iDirection;
	if (m_iFrequency == 9001) m_iFrequency++;
	return true;
}

bool BlockAction::set_spriteCurrentLocation(float f_fX, float f_fY)
{
	m_fXOriginal = f_fX;
	m_fYOriginal = f_fY;
	return true;
}

bool BlockAction::debug_action()
{
	cout << "m_bBlockAction: "; if(m_bBlockAction) cout << "TRUE" << endl; else cout << "FALSE" << endl;
	cout << "m_character: " << (int)m_character << endl;
	cout << "m_bDirectionTop: "; if (m_bDirectionTop) cout << "TRUE" << endl; else cout << "FALSE" << endl;
	cout << "m_bDirectionLeft: "; if (m_bDirectionLeft) cout << "TRUE" << endl; else cout << "FALSE" << endl;
	cout << "m_bDirectionRight: "; if (m_bDirectionRight) cout << "TRUE" << endl; else cout << "FALSE" << endl;
	cout << "m_bDirectionBottom: "; if (m_bDirectionBottom) cout << "TRUE" << endl; else cout << "FALSE" << endl;
	cout << "m_graphicalEffect: " << m_graphicalEffect << endl;
	cout << "m_iFrequency: " << m_iFrequency << endl;
	cout << "m_sItem: " << m_sItem << endl;
	cout << "m_iCompletePointRow: " << m_iCompletePointRow << endl;
	cout << "m_iCompletePointCol: " << m_iCompletePointCol << endl;
	return true;
}