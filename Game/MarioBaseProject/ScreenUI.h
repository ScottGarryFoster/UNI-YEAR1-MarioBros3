#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include "TextureSelfManaged.h"
#include <iostream>
#include <string>

#include "Sprite.h"
#include "SPRITEBATCH.h"
class ScreenUI
{
private:

protected:
	SpriteBatch *p_sb = nullptr;
public:
	ScreenUI();
	ScreenUI(SpriteBatch *f_sb);
	virtual ~ScreenUI();
};

