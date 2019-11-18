#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include "Vector2D.h"
#include "Sprite.h"

#include <iostream>
#include <string>

using namespace::std;

class SpriteBatch
{
private:
	SDL_Renderer *m_sdlRenderer;
	SDL_Texture *m_sdlTexture;

	int m_iRedValue = 0;
	int m_iBlueValue = 0;
	int m_iGreenValue = 0;
	int m_iAlpha = 255;
public:
	SpriteBatch();
	~SpriteBatch();
	bool awake() { return true; }//Used in try catches for if instance exists

	///Sets the renderer in the sprite batch///
	void set_renderer(SDL_Renderer *f_sdlRenderer) { cout << "SET RENDERER" << endl; m_sdlRenderer = f_sdlRenderer; }
	void set_color(int f_iRed, int f_iBlue, int f_iGreen, int f_iAlpha = 255) { m_iRedValue = f_iRed; m_iBlueValue = f_iBlue; m_iGreenValue = f_iGreen; m_iAlpha = f_iAlpha; }
	///Gets the Renderer///
	SDL_Renderer* get_renderer() { return m_sdlRenderer; }

	void Start();
	void End();

	void DrawSprite(Sprite *f_sprite);
	void DrawSprite(Sprite *f_sprite, Vector2D* f_v2dLocation);

private:
	SDL_Texture * LoadTextureFromFile(string f_sLocation);
};

