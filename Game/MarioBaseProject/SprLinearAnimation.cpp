#include "SprLinearAnimation.h"



SprLinearAnimation::SprLinearAnimation()
{
}


SprLinearAnimation::~SprLinearAnimation()
{
	
}

bool SprLinearAnimation::Update(float f_deltaTime)
{
	if (m_iCurrentCol < m_iStartLocation || m_iCurrentCol > m_iEndLocation)
		m_iCurrentCol = m_iStartLocation;

	m_iCurrentTick += f_deltaTime;
	if (m_iCurrentTick > m_iSpeed)
	{
		m_iCurrentTick -= m_iSpeed;
		m_iCurrentCol++;
		if (m_iCurrentCol > m_iEndLocation)
			m_iCurrentCol = m_iStartLocation;
	}
	return true;
}

bool SprLinearAnimation::setup_animation(string f_sData)
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
		if (set_byString(f_sParameter, f_sValue)) m_bAnimation = true;
	}
	delete l_llLines;

	return true;
}

bool SprLinearAnimation::set_byString(string f_sPara, string f_sValue)
{
	string f_sParaUpper = Utility::StringToUpperCase(f_sPara);//Shouldn't be case sensative
	string f_sValueUpper = Utility::StringToUpperCase(f_sValue);//Shouldn't be case sensative

	//cout << "para: " << f_sParaUpper << endl;
	//cout << "value: " << f_sValueUpper << endl;

	if (f_sParaUpper == "" || f_sValueUpper == "")
		return false;
	else if (f_sParaUpper == "NUMBEROFFRAMES:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iNumberOfFrames = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "STARTLOCATION:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iStartLocation = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "ENDLOCATION:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iEndLocation = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "SPEED:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iSpeed = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "STARTROW:")
	{
		int l_iNumber = Utility::convert_stringToInt(f_sValueUpper);
		if (l_iNumber == -1) return false;
		m_iStartRow = l_iNumber;
		return true;
	}
	else if (f_sParaUpper == "TEXTURE:")
	{
		m_sTextureLocation = f_sValueUpper;
		return true;
	} 
	return false;
}

bool SprLinearAnimation::debug_animation()
{
	cout << "m_bAnimation: ";
	if (m_bAnimation) cout << "TRUE" << endl; else cout << "FALSE" << endl;
	cout << "m_iNumberOfFrames: " << m_iNumberOfFrames << endl;
	cout << "m_iStartLocation: " << m_iStartLocation << endl;
	cout << "m_iEndLocation: " << m_iEndLocation << endl;
	cout << "m_iSpeed: " << m_iSpeed << endl;
	cout << "m_iCurrentTick: " << m_iCurrentTick << endl;
	return true;
}