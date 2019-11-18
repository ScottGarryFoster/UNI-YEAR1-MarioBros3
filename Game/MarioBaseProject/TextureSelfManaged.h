#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <iostream>
#include <string>

using namespace::std;

class TextureSelfManaged
{
private:
	SDL_Texture *m_sdlTexture = nullptr;
	string m_sLastTextureLoaded = "";
	bool m_bHasLoadedTexture = false;
	bool m_bLoadedFromPointer = false;
public:
	TextureSelfManaged();
	TextureSelfManaged(SDL_Renderer* f_sdlRenderer, string f_sLocation);
	TextureSelfManaged(SDL_Renderer* f_sdlRenderer, SDL_Texture *m_sdlTexture, string f_sLocation = "");
	~TextureSelfManaged();

	SDL_Texture* get_texturePointer() { return m_sdlTexture; }
	string get_lastLocation() { return m_sLastTextureLoaded; }
	bool get_hasLoadedTexture() { return m_bHasLoadedTexture; }

	bool LoadTextureFromLocation(SDL_Renderer* f_sdlRenderer, string f_sLocation);
	bool set_textureFromPointer(SDL_Texture* f_sdlTexture, string f_sLocation = "");

};

