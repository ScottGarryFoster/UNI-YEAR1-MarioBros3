#pragma once

class TileModifications
{
public:
	struct TileMod_Tile
	{
		int x = 0, y = 0;
		void set_position(int f_iX, int f_iY) { x = f_iX; y = f_iY; }
	};
private:
	bool m_bModification = false;

	TileMod_Tile *m_ttLocation = nullptr;
	


	float m_fXModifyGoal = 0;
	float m_fYModifyGoal = 0;

	int m_iCurrentTick = 0;
	int m_iTickLimit = 0;
	bool m_bAreTicking = false;

	float m_fXModify = 0;
	float m_fYModify = 0;
public:
	TileModifications();
	virtual ~TileModifications();

	bool TickOver_begin();
	bool TickOver_end();

	TileMod_Tile *get_tilemapLocation() { return m_ttLocation; }

	bool setup_movementModification(float f_fXMod, float f_fYMod, int f_iTickLimit, TileMod_Tile* f_ffLocation);
};

