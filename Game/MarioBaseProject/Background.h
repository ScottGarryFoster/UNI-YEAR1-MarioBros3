#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "SPRITEBATCH.h"
#include "TextureSelfManaged.h"
#include "LinkedList.h"
#include "Utility.h"
#include "Camera.h"

#include <iostream>
#include <string>
#include <fstream>


class Background
{
private:
	SpriteBatch *m_sb = nullptr;
	TextureSelfManaged *m_tsmTexture = nullptr;
	TextureSelfManaged *m_tsmTransparent = nullptr;

	int m_iWidth = 0, m_iHeight = 0;
	bool m_bRepeat = true;
	float m_fScaleSize = 1;
	int m_iScaledWidthAndHeight = 0;

	bool m_bLockToCamera = false;
protected:
	Camera *p_camera = nullptr;
	Camera *p_alternativeCameras[10] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
public:
	Background();
	Background(SpriteBatch* f_sb);
	~Background();

	void set_spriteBath(SpriteBatch* f_sb) { m_sb = f_sb; }
	void set_camera(Camera *f_camera) { p_camera = f_camera; }
	void set_camera(Camera *f_camera, bool f_bLock) { p_camera = f_camera; m_bLockToCamera = f_bLock; }
	void set_camera(bool f_bLock, Camera *f_camera) { p_camera = f_camera; m_bLockToCamera = f_bLock; }
	void set_cameraLock(bool f_bLock) { m_bLockToCamera = f_bLock; }

	void set_altCamera(Camera *f_camera, int f_iIndex) { p_alternativeCameras[f_iIndex] = f_camera;  }

	bool load_background(string f_sLocation, int f_iWidth, int f_iHeight, bool f_bRepeat = true);

	bool Draw();
private:
	bool Draw_eachCamera(Camera *f_camera = nullptr);
};

