#include "SpriteBatch.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include <iostream>
#include <string>


SpriteBatch::SpriteBatch()
{
	m_sdlRenderer = nullptr;
	m_sdlTexture = nullptr;
	//m_sprite = new Sprite();
}


SpriteBatch::~SpriteBatch()
{
	//delete m_sprite; m_sprite = nullptr;
}

//void SpriteBatch::Start()
//{
//	m_sprite->LoadTextureFromFile(m_sdlRenderer, "Images/Mario3/Char_MarioSmall.png");
//
//	m_sprite->set_scaleSize(5);
//	m_sprite->set_singleSize(18, 18);
//	m_sprite->set_location(0, 0);
//	//Clear
//	SDL_SetRenderDrawColor(m_sdlRenderer, 0x00, 0x00, 0x00, 0x00);
//	SDL_RenderClear(m_sdlRenderer);
//
//	m_sprite->RenderToScreen(m_sdlRenderer);
//	
//
//	SDL_RenderPresent(m_sdlRenderer);
//}

/*
	DRAW PHASE:
*/

void SpriteBatch::Start()
{
	SDL_SetRenderDrawColor(m_sdlRenderer, m_iRedValue, m_iBlueValue, m_iGreenValue, m_iAlpha);
	SDL_RenderClear(m_sdlRenderer);
}


void SpriteBatch::End()
{
	SDL_RenderPresent(m_sdlRenderer);
}


void SpriteBatch::DrawSprite(Sprite *f_sprite)
{
	f_sprite->RenderToScreen(m_sdlRenderer);
}
void SpriteBatch::DrawSprite(Sprite *f_sprite, Vector2D* f_v2dLocation)
{
	f_sprite->RenderToScreen(m_sdlRenderer, f_v2dLocation);
}





SDL_Texture* SpriteBatch::LoadTextureFromFile(string f_sLocation)
{
	SDL_Texture *l_texture = nullptr;
	//Load:
	SDL_Surface *l_surface = IMG_Load(f_sLocation.c_str());
	if (l_surface != nullptr)
	{
		l_texture = SDL_CreateTextureFromSurface(m_sdlRenderer, l_surface);
		if (l_texture == nullptr)
			cout << "SPRITEBATCH::LoadTextureFromFile: Could not create texture from surface. Error: " << SDL_GetError() << endl;
	}
	else
		cout << "SPRITEBATCH::Could not load image. " << f_sLocation << " . Error: " << SDL_GetError() << endl;
	
	return l_texture;
}