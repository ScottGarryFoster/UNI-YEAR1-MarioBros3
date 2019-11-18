#pragma once
#include "LinkedList.h"
#include "Utility.h"

#include <string>

using namespace::std;
class SprLinearAnimation
{
private:
	bool m_bAnimation = false;

	int m_iNumberOfFrames = 1;
	int m_iStartRow = 0;
	int m_iStartLocation = 0;
	int m_iCurrentCol = 0;
	int m_iEndLocation = 0;
	int m_iSpeed = 1000;

	int m_iCurrentTick = 0;

	string m_sTextureLocation = "";
public:
	SprLinearAnimation();
	virtual ~SprLinearAnimation();

	bool Update(float f_deltaTime);

	bool setup_animation(string f_sData);
	bool set_byString(string f_sPara, string f_sValue);

	string get_textureLocation(){ return m_sTextureLocation; }
	int get_startLocationRow() { return m_iStartRow; }
	int get_startLocation() { return m_iStartLocation; }

	int get_currentLocation() { return m_iCurrentCol; }

	bool debug_animation();
};

