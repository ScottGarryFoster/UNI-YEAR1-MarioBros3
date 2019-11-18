#include "EnemyEffect.h"



EnemyEffect::EnemyEffect()
{
}


EnemyEffect::~EnemyEffect()
{
}


bool EnemyEffect::setup_effect(string f_sData)
{
	m_sActor = "";
	m_bDirectionLeft = false;
	m_bDirectionRight = false;
	m_bDirectionTop = false;
	m_bDirectionBottom = false;
	m_effect = eeeNone;
	m_iEffectState = -1;
	m_iEffectConditionState = -1;
	m_sEffectAddition = "";


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
		if (set_byString(f_sParameter, f_sValue)) m_bSetup = true;
	}
	delete l_llLines;

	return true;
}

bool EnemyEffect::set_byString(string f_sPara, string f_sValue)
{
	string f_sParaUpper = Utility::StringToUpperCase(f_sPara);//Shouldn't be case sensative
	string f_sValueUpper = Utility::StringToUpperCase(f_sValue);//Shouldn't be case sensative

	if (f_sParaUpper == "")
		return false;
	else if (f_sParaUpper == "ACTOR:")
	{
		m_sActor = f_sValueUpper;
		return true;
	}
	else if (f_sParaUpper == "DIRECTION:")
	{
		if (f_sValueUpper == "TOP" || f_sValueUpper == "ANY") m_bDirectionTop = true;
		if (f_sValueUpper == "LEFT" || f_sValueUpper == "ANY") m_bDirectionLeft = true;
		if (f_sValueUpper == "RIGHT" || f_sValueUpper == "ANY") m_bDirectionRight = true;
		if (f_sValueUpper == "BOTTOM" || f_sValueUpper == "ANY") m_bDirectionBottom = true;
		return true;
	}
	else if (f_sParaUpper == "EFFECT:")
	{
		if (f_sValueUpper == "IDIE")
			m_effect = eeeIDie;
		else if (f_sValueUpper == "THEYTAKEDAMAGE")
			m_effect = eeeTheyTakeDamage;
		else if (f_sValueUpper == "STOMPED")
			m_effect = eeeStomped;
		else if (f_sValueUpper == "CHANGEDIRECTION")
			m_effect = eeeChangeDirection;
		else if (f_sValueUpper == "UPDATESTATE")
			m_effect = eeeUpdateState;
		else if (f_sValueUpper == "KILLNONESHELL")
			m_effect = eeeKillNoneShell;
		else if (f_sValueUpper == "KILLBOTHSHELLS")
			m_effect = eeeKillBothShells;
		else///
			m_effect = eeeNone;
		return true;
	}
	else if (f_sParaUpper == "EFFECTSTATE:")
	{
		m_iEffectState = 0;
		if (f_sValueUpper == "SHELL") m_iEffectState = 1;
		if (f_sValueUpper == "MOVINGSHELL") m_iEffectState = 2;
		return true;
	}
	else if (f_sParaUpper == "CONDITIONALSTATE:")
	{
		m_iEffectConditionState = -1;
		if (f_sValueUpper == "PRONE")	m_iEffectConditionState = 0;
		if (f_sValueUpper == "SHELL") m_iEffectConditionState = 1;
		if (f_sValueUpper == "MOVINGSHELL") m_iEffectConditionState = 2;

		//cout << "m_iEffectConditionState: " << m_iEffectConditionState << endl;
		return true;
	}
	else if (f_sParaUpper == "EFFECTADDITION:")
	{
		m_sEffectAddition = f_sValueUpper;
		return true;
	}
	return false;
}

bool EnemyEffect::update_actionActivation(int f_iDirection, int f_iActor, bool f_bPlayerSize, int *f_iFeedback)
{

	switch (f_iDirection)
	{
	case 2:
		if (!m_bDirectionBottom) return false;
		break;
	case 8:
		if (!m_bDirectionTop) return false;
		break;
	case 4:
		if (!m_bDirectionLeft) return false;
		break;
	case 6:
		if (!m_bDirectionRight) return false;
		break;
	default:
		return false;
	}
	switch (f_iActor)
	{
	case 0:
		if (m_sActor != "PLAYER") return false;
		break;
	case 1:
		//if (m_sActor == "ENEMY") cout << "1" << endl;
		if (m_sActor != "ENEMY") return false;
		break;
	case 2:
		//if (m_sActor == "ENEMY") cout << "1" << endl;
		if (m_sActor != "MOVINGSHELL") return false;
		break;
	case 3:
		//if (m_sActor == "ENEMY") cout << "1" << endl;
		if (m_sActor != "SHELLINHAND") return false;
		break;
	default:
		return false;
	}

	//cout << "ACTIVATE:" << endl;
	//debug_enemyEffect();
	//cout << "DATA: f_iDirection: " << f_iDirection << ", f_iActor: " << f_iActor << endl;

	switch (m_effect)
	{
	case eeeTheyTakeDamage:
		//cout << "THEY DIE" << endl;
		if (f_iFeedback != nullptr) *f_iFeedback = 1;
		break;
	case eeeStomped:
		if (f_iFeedback != nullptr) *f_iFeedback = 0;
		//cout << "I Stomped" << endl; 
		break;
	case eeeIDie:
		if (f_iFeedback != nullptr) *f_iFeedback = 2;
		//cout << "I Died" << endl;
		break;
	case eeeUpdateState:
		if (f_iFeedback != nullptr) *f_iFeedback = 3;
		//cout << "Changed State" << endl;
		break;
	case eeeChangeDirection:
		if (f_iFeedback != nullptr) *f_iFeedback = 4;
		//cout << "Change Direction" << endl;
		break;
	case eeeKillNoneShell:
		//cout << "YH" << endl;
		if (f_iFeedback != nullptr) *f_iFeedback = 5;
		//cout << "eeeKillNoneShell" << endl;
		break;
	case eeeKillBothShells:
		//cout << "YH" << endl;
		if (f_iFeedback != nullptr) *f_iFeedback = 6;
		//cout << "eeeKillNoneShell" << endl;
		break;
	default:
		return false;
	}

	//debug_enemyEffect();

	return true;
}

bool EnemyEffect::debug_enemyEffect()
{
	cout << "m_sActor: " << m_sActor << endl;
	cout << "m_bDirectionLeft: "; if (m_bDirectionLeft) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bDirectionRight: "; if (m_bDirectionRight) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bDirectionTop: "; if (m_bDirectionTop) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_bDirectionBottom: "; if (m_bDirectionBottom) cout << "TRUE"; else cout << "FALSE"; cout << endl;
	cout << "m_effect: ";
	switch (m_effect)
	{
	case eeeNone:cout << "NONE";break;
	case eeeChangeDirection:cout << "ChangeDirection";break;
	case eeeIDie:cout << "IDie";break;
	case eeeStomped:cout << "Stomped";break;
	case eeeTheyTakeDamage:cout << "TheyTakeDamage";break;
	case eeeUpdateState:cout << "UpdateState";break;
	case eeeKillNoneShell:cout << "eeeKillNoneShell";break;
	case eeeKillBothShells:cout << "eeeKillBothShells";break;
	}
	cout << endl;
	cout << "m_iEffectState: ";
	switch (m_iEffectState)
	{
	case 0: cout << "0 (prone)" << endl; break;
	case 1: cout << "1 (Shell)" << endl; break;
	case 2: cout << "2 (MovingShell)" << endl; break;
	}
	cout << "m_iEffectConditionState: ";
	switch (m_iEffectConditionState)
	{
	case 0: cout << "0 (prone)" << endl; break;
	case 1: cout << "1 (Shell)" << endl; break;
	case 2: cout << "0 (MovingShell)" << endl; break;
	}
	cout << "m_sEffectAddition: " << m_sEffectAddition << endl;

	cout << "==============================================" << endl;
	return true;
}