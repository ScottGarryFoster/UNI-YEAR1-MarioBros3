#include "TileInformation.h"



TileInformation::TileInformation()
{
}


TileInformation::~TileInformation()
{
}

void TileInformation::setup_animation(int f_iFrames, int f_iTickLimit, int f_iStartTileNumber, string f_sSignature)
{
	m_iNumberOfFrames = f_iFrames;
	m_iTickLimit = f_iTickLimit;
	m_sSignature = f_sSignature;
	m_iStartTileNumber = f_iStartTileNumber;
	m_iNewTileNumber = f_iStartTileNumber;
	m_bAnimation = true;
}

bool TileInformation::TickOver_begin()
{
	if (m_iNewTileNumber == -1)
		m_iNewTileNumber = m_iStartTileNumber;

	m_iCurrentTileNumber = m_iNewTileNumber;
	m_iNewTileNumber++;
	if (m_iNewTileNumber > m_iStartTileNumber + m_iNumberOfFrames - 1)
		m_iNewTileNumber = m_iStartTileNumber;
	m_bAreTicking = true;
	return true;
}

bool TileInformation::TickOver_end()
{
	if (!m_bAreTicking) return false;
	m_iCurrentTileNumber = -1;
	//m_iNewTileNumber -= -1;
	m_bAreTicking = false;
	return true;
}

void TileInformation::debug_animation()
{
	cout << "m_bAnimation: ";
	if (m_bAnimation) cout << "true" << endl; else cout << "false" << endl;
	cout << "m_sSignature: " << m_sSignature << endl;
	cout << "m_iNumberOfFrames: " << m_iNumberOfFrames << endl;
	cout << "m_iTickLimit: " << m_iTickLimit << endl;
	cout << "m_iCurrentTick: " << m_iCurrentTick << endl;
	cout << "m_iStartTileNumber: " << m_iStartTileNumber << endl;

	cout << "m_iCurrentTileNumber: " << m_iCurrentTileNumber << endl;
	cout << "m_iNewTileNumber: " << m_iNewTileNumber << endl;
	cout << "m_bAreTicking: ";
	if (m_bAreTicking) cout << "true" << endl; else cout << "false" << endl;
}