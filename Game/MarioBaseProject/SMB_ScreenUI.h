#pragma once
#include "ScreenUI.h"
#include "Vector2D.h"
#include "SprLetters.h"

class SMB_ScreenUI : public ScreenUI
{
private:
	TextureSelfManaged *m_tsmTexture = nullptr;
	Sprite *m_sprLowerBackground = nullptr;
	Sprite *m_sprSmallerElements = nullptr;
	SprLetters *m_sprCharacters = nullptr;
	Vector2D *m_v2dNewLocation = nullptr;
	float m_fOffsetX = 0;
	float m_fOffsetY = 0;
	float m_fOriginalLocationX = 25;
	float m_fOriginalLocationY = SCREEN_HEIGHT - 55;

	char m_cPlayerCounter = 'm';
	char *m_cPtrPlayerCounter = nullptr;
	int m_iPSpeed = 3;
	int *m_iPtrSpeed = nullptr;
	int m_iWorldNumber = 4;
	int *m_iPtrWorldNumber = nullptr;
	int m_iPoints = 9581;
	int *m_iPtrPoints = nullptr;
	int m_iCoins = 34;
	int *m_iPtrCoins = nullptr;
	int m_iLives = 42;
	int *m_iPtrLives = nullptr;
	int m_iTime = 321;
	int *m_iPtrTime = nullptr;
public:
	SMB_ScreenUI(SpriteBatch *f_sb);
	virtual ~SMB_ScreenUI();

	bool Update();
	bool Draw();

	void set_offset(float f_fX, float f_fY) { m_fOffsetX = f_fX; m_fOffsetY = f_fY;}
	void set_offsetX(float f_fInput) { m_fOffsetX = f_fInput; }
	void set_offsetY(float f_fInput) { m_fOffsetY = f_fInput; }

	void set_playerCounter(char f_cInput){m_cPlayerCounter = f_cInput;}
	void set_pSpeed(int f_iInput) { m_iPSpeed = f_iInput; }
	void set_worldNumber(int f_iInput) { m_iWorldNumber = f_iInput; }
	void set_points(int f_iInput) { m_iPoints = f_iInput; }
	void set_coins(int f_iInput) { m_iCoins = f_iInput; }
	void set_lives(int f_iInput) { m_iLives = f_iInput; }
	void set_time(int f_iInput) { m_iTime = f_iInput; }

	void set_playerCounter(char *f_cInput) { m_cPtrPlayerCounter = f_cInput; }
	void set_pSpeed(int *f_iInput) { m_iPtrSpeed = f_iInput; }
	void set_worldNumber(int *f_iInput) { m_iPtrWorldNumber = f_iInput; }
	void set_points(int *f_iInput) { m_iPtrPoints = f_iInput; }
	void set_coins(int *f_iInput) { m_iPtrCoins = f_iInput; }
	void set_lives(int *f_iInput) { m_iPtrLives = f_iInput; }
	void set_time(int *f_iInput) { m_iPtrTime = f_iInput; }
private:
	void update_pointersToMembers();
};

