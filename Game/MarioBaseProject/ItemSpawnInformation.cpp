#include "ItemSpawnInformation.h"



ItemSpawnInformation::ItemSpawnInformation()
{
}


ItemSpawnInformation::~ItemSpawnInformation()
{
}

bool ItemSpawnInformation::setup_all(string f_sItemName, int f_iX, int f_iY, ItemSpawnEvent f_ise)
{
	if (m_bIsUsed == true) return false;
	m_sItemName = f_sItemName;
	m_iLocationX = f_iX;
	m_iLocationY = f_iY;
	m_ise = f_ise;
	m_bIsUsed = true;
	return true;
}

bool ItemSpawnInformation::reset_all()
{
	m_sItemName = "";
	m_iLocationX = 0;
	m_iLocationY = 0;
	m_ise = iseAsIs;
	m_bIsUsed = false;
	return true;
}