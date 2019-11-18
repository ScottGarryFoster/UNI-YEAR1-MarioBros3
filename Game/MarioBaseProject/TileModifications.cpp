#include "TileModifications.h"



TileModifications::TileModifications()
{
}


TileModifications::~TileModifications()
{
	if (m_ttLocation != nullptr)
		delete m_ttLocation;
}

bool TileModifications::TickOver_begin()
{
	if (m_ttLocation == nullptr)
		m_ttLocation = new TileMod_Tile();
	m_fXModify = 0;
	m_fYModify = 0;

	return true;
}

bool TileModifications::TickOver_end()
{
	if (m_ttLocation == nullptr) return false;
	m_ttLocation->set_position(-1, -1);
	m_fXModify = 0;
	m_fYModify = 0;
	return false;
}

bool TileModifications::setup_movementModification(float f_fXMod, float f_fYMod, int f_iTickLimit, TileMod_Tile* f_ffLocation)
{
	m_ttLocation = f_ffLocation;
	m_fXModifyGoal = f_fXMod;
	m_fYModifyGoal = f_fYMod;
	m_iTickLimit = f_iTickLimit;
	return false;
}
