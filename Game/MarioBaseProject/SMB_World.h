#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>

#include "SPRITEBATCH.h"
#include "LinkedList.h"
#include "Tilemap.h"
#include "Background.h"
#include "Camera.h"
#include "Controls.h"

#include "SMB_PlayerPlatformer.h"
#include "SMB_ScreenUI.h"

class SMB_World
{
private:
	Tilemap *m_tilemap = nullptr;
public:
	SMB_World();
	~SMB_World();
};

