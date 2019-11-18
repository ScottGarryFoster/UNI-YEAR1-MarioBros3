#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include "Vector2D.h"
#include "TextureSelfManaged.h"
#include "Camera.h"

#include <iostream>
#include <string>

using namespace::std;

class Sprite
{
public:
	enum SpriteFlip : int {None = 0, Horizontal, Vertical, Both};
private:
	//Debug
	bool m_bDebugTextureNull = false;//Used for debug messages

	int m_iId = -1;

	TextureSelfManaged *m_tsmTexture = nullptr;
	int m_iScreenWidth = 0, m_iScreenHeight = 0;
	int m_iActualWidth = 0, m_iActualHeight = 0;
	float m_iLocationX = 0, m_iLocationY = 0;

	int m_iSingleWidth = 0, m_iSingleHeight = 0;
	int m_iSpriteRow = 0, m_iSpriteCol = 0;
	SpriteFlip m_flip = None;

	float m_iScaleSize = 1;

	bool m_bHasLoadedTexture = false;
	string m_sLastTextureLoaded = "";

	bool m_bVisable = true;
	bool m_bDrawFromBottom = false;
	bool m_bLockToCamera = false;

	//Sprite Location Lock
	bool m_bLocationLockYTop = true;
	bool m_bLocationLockYBottom = true;
	bool m_bLocationLockXLeft = true;
	bool m_bLocationLockXRight = true;

	//Memroy Stuff
	bool m_bLoadedTextureFromPointer = false;
	bool m_bDidDrawThisFrame = false;
protected:
	Camera *p_camera = nullptr;
	Camera *p_alternativeCameras[10] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
public:
	Sprite();
	virtual ~Sprite();

	//SETS
	void set_id(int f_iValue) { m_iId = f_iValue; }
	void set_screenSize(int f_iWidth, int f_iHeight) { m_iScreenWidth = f_iWidth; m_iScreenHeight = f_iHeight; }
	void set_location(int f_iPosX, int f_iPosY) { m_iLocationX = (float)f_iPosX; m_iLocationY = (float)f_iPosY; }
	void move_location(int f_iX, int f_iY) { move_location((float)f_iX, (float)f_iY); }
	bool move_location(float f_iX, float f_iY);
	bool move_location(int f_iX, float f_iY) { return move_location((float)f_iX, (float)f_iY); }
	bool move_location(float f_iX, int f_iY) { return move_location((float)f_iX, (float)f_iY); }

	void set_singleSize(int f_iWidth, int f_iHeight) { m_iSingleWidth = f_iWidth; m_iSingleHeight = f_iHeight; }
	void set_singleLocation(int f_iRow, int f_iCol) { m_iSpriteRow = f_iRow;  m_iSpriteCol = f_iCol; }

	void set_scaleSize(int f_iScale) { if (f_iScale >= 0)m_iScaleSize = (float)f_iScale; else m_iScaleSize = 0; }
	void set_scaleSize(float f_iScale) { if (f_iScale >= 0)m_iScaleSize = f_iScale; else m_iScaleSize = 0; }

	void set_textureFromPointer(SDL_Texture* f_sdlTexture, string f_sLocation = "");
	void set_textureSelfManageFromPointer(TextureSelfManaged* l_tsm) { m_tsmTexture = l_tsm; m_bHasLoadedTexture = m_tsmTexture->get_hasLoadedTexture(); m_bLoadedTextureFromPointer = true;}

	//Note if camera lock is false and there are cameras, weird things may happen you have been warned
	void set_camera(Camera *f_camera){p_camera = f_camera; m_bLockToCamera= true;}
	void set_camera(Camera *f_camera, bool f_bLock) { p_camera = f_camera; m_bLockToCamera = f_bLock; }
	void set_camera(bool f_bLock, Camera *f_camera) { p_camera = f_camera; m_bLockToCamera = f_bLock; }
	void set_cameraLock(bool f_bLock) { m_bLockToCamera = f_bLock; }

	void set_altCamera(Camera *f_camera, int f_iIndex){ p_alternativeCameras[f_iIndex] = f_camera; m_bLockToCamera = true;}

	void set_visable(bool f_bValue = true) { m_bVisable = f_bValue; }
	void set_flip(SpriteFlip f_flip){ m_flip = f_flip; }

	void set_locks(bool f_bLeft, bool f_bRight, bool f_bTop, bool f_bDown) { m_bLocationLockXLeft = f_bLeft; m_bLocationLockXRight = f_bRight; m_bLocationLockYTop = f_bTop; m_bLocationLockYBottom = f_bDown; }
	void set_locksLeft(bool f_bLeft) { set_locks(f_bLeft, m_bLocationLockXRight, m_bLocationLockYTop, m_bLocationLockYBottom); };
	void set_locksRight(bool f_bRight) { set_locks(m_bLocationLockXLeft, f_bRight, m_bLocationLockYTop, m_bLocationLockYBottom); };
	void set_locksTop(bool f_bTop) { set_locks(m_bLocationLockXLeft, m_bLocationLockXRight, f_bTop, m_bLocationLockYBottom); };
	void set_locksBottom(bool f_bDown) { set_locks(m_bLocationLockXLeft, m_bLocationLockXRight, m_bLocationLockYTop, f_bDown); };
	void set_didDrawThisFrame(bool f_bData) { m_bDidDrawThisFrame = f_bData;  }

	//GETS
	string get_lastTextureLoaded() { return m_sLastTextureLoaded; }
	SDL_Texture* get_texturePointer() { return m_tsmTexture->get_texturePointer(); }
	TextureSelfManaged* get_textureSelfManageFromPointer(){return m_tsmTexture;}
	int get_locationX() { return m_iLocationX; }
	int get_locationY() { return m_iLocationY; }

	int get_singleWidth() { return m_iSingleWidth; }
	int get_singleHeight() { return m_iSingleWidth; }

	int get_actualSizeWidth() { update_actualsize(); return m_iActualWidth; }
	int get_actualSizeHeight() { update_actualsize();  return m_iActualHeight; }

	float get_scaleSize() { return m_iScaleSize; }

	int get_sourceRow() { return m_iSpriteRow; }
	int get_sourceCol() { return m_iSpriteCol; }

	bool get_visable(){return m_bVisable;}
	SpriteFlip get_flip() { return m_flip; }

	///True means it did draw within the last 10 frames.
	bool get_didDrawThisFrame() { return m_bDidDrawThisFrame; }

	//Updates
	bool update_actualsize();

	//Equals
	//Sprite& 

	bool LoadTextureFromFile(SDL_Renderer* f_sdlRenderer, string f_sLocation);

	bool RenderToScreen(SDL_Renderer* f_sdlRenderer, Vector2D *f_v2dLocation = nullptr);

	void debug_all();

private:

	bool RenderToScreenInner(SDL_Renderer* f_sdlRenderer, Vector2D* f_v2dLocation = nullptr, Camera *f_camera = nullptr);
};

