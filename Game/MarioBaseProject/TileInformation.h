#pragma once
#include <string>
#include <iostream>

using namespace::std;

class TileInformation
{
private:
	bool m_bAnimation = false;
	string m_sSignature = "";
	int m_iNumberOfFrames = 0;
	int m_iTickLimit = 0;
	int m_iCurrentTick = 0;
	int m_iStartTileNumber = 0;

	int m_iCurrentTileNumber = -1;
	int m_iNewTileNumber = -1;
	bool m_bAreTicking = false;

	bool m_bMovement = false;
	float m_fMovementX = 0;
	float m_fMovementY = 0;
	int m_iTileNumber = -1;//The number once the movement is over
public:
	TileInformation();
	virtual ~TileInformation();

	//Animations:

	void set_newTileNumber(int f_iData) { m_iNewTileNumber = f_iData; }

	string get_signature() { return m_sSignature; }
	int get_numberOfFrames() { return m_iNumberOfFrames; }
	int get_tickLimit() { return m_iTickLimit; }
	int get_currentTick() { return m_iCurrentTick; }
	int get_currentTileNumber() { return m_iCurrentTileNumber; }
	int get_newTileNumber() { return m_iNewTileNumber; }

	bool TickOver_begin();
	bool TickOver_end();

	void add_tick(float f_dt) { m_iCurrentTick += (int)f_dt; }

	void setup_animation(int f_iFrames, int f_iTickLimit, int f_iStartTileNumber, string f_sSignature);

	void debug_animation();

	//Modifications like movement:
	bool get_movement() { return m_bMovement; }
	int get_modTileNumber() { return m_iTileNumber; }
	float get_modMovementX() { return m_fMovementX; }
	float get_modMovementY() { return m_fMovementY; }
};

