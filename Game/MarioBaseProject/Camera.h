#pragma once
#include <iostream>
#include "Constants.h"

using namespace std;
class Camera
{
private:
	int m_iId = 0;

	float m_iLocationX = 0, m_iLocationY = 0; //This is where the camera is in the world

	int m_iWorldWidth = 0, m_iWorldHeight = 0;//The whole world width and height

	float m_fScreenX = 0, m_fScreenY = 0; //The position rendered on the screen

	float m_fScreenWidth = 1280, m_fScreenHeight = 720; //The size rendered on the screen

	bool m_bLockToWorld = false; //Lock where the camera can be to the world
	float m_iYLowest = 0;
	float m_iYHighest = 55*3;
	bool m_bScrolledUp = false;

	int m_iLockId = 0;

	bool m_bVisable = true;
	bool m_bLevelEditor = false;//If you're the level editor this will show you as much. Used to difforentiate a character you're following and a mouse cursor.
public:
	Camera();
	~Camera();

	void const set_x(float f_iNewX) { m_iLocationX = f_iNewX; update_lockToWorld();}
	void const set_y(float f_iNewY) { m_iLocationY = f_iNewY; update_lockToWorld();}

	void set_worldWidth(int f_iNewWidth) { m_iWorldWidth = f_iNewWidth; }
	void set_worldHeight(int f_iNewHeight) { m_iWorldHeight = f_iNewHeight; }

	void set_screenX(float f_fNew) { m_fScreenX = f_fNew; }
	void set_screenY(float f_fNew) { m_fScreenY = f_fNew; }
	void set_screenPosition(float f_fX, float f_fY) { m_fScreenX = f_fX; m_fScreenY = f_fY; }

	void set_screenWidth(float f_fNew) { m_fScreenWidth = f_fNew; }
	void set_screenHeight(float f_fNew) { m_fScreenHeight = f_fNew; }
	void set_screenSize(float f_fX, float f_fY) { m_fScreenWidth = f_fX; m_fScreenHeight = f_fY; }

	void set_lockToWorld(bool f_bNew) { m_bLockToWorld = f_bNew; }
	void set_lockId(int f_iLock){m_iLockId = f_iLock;}
	void set_yLowest(int f_iNew){ m_iYLowest = (float)f_iNew;}
	void set_yHighest(int f_iNew){ m_iYHighest = (float)f_iNew;}

	void set_visable(bool f_bNew) { m_bVisable = f_bNew; }
	void set_scrolledUp(bool f_bNew) { m_bScrolledUp = f_bNew; }

	void set_isLevelEditor(bool f_bData) { m_bLevelEditor = f_bData; }

	int const get_x() { return m_iLocationX; }
	int const get_y() { return m_iLocationY; }

	int const get_worldWidth() { return m_iWorldWidth; }
	int const get_worldHeight() { return m_iWorldHeight; }

	float const get_screenX() { return m_fScreenX; }
	float const get_screenY() { return m_fScreenY; }

	float const get_screenWidth() { return m_fScreenWidth; }
	float const get_screenHeight() { return m_fScreenHeight; }

	bool get_visable() { return m_bVisable; }

	int get_lockId(){return m_iLockId;}
	int get_yLowest(){return m_iYLowest;}
	int get_yHighest(){return m_iYHighest;}

	bool get_scrolledUp() { return m_bScrolledUp; }

	void MoveCamera(float f_iX, float f_iY);

private:
	void update_lockToWorld();
	bool update_cameraScroll();
	bool update_cameraLevelEditor();
};

