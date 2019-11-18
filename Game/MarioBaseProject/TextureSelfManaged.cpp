#include "TextureSelfManaged.h"

TextureSelfManaged::TextureSelfManaged()
{
	m_bHasLoadedTexture = false;
}

TextureSelfManaged::TextureSelfManaged(SDL_Renderer* f_sdlRenderer, string f_sLocation)
{
	LoadTextureFromLocation(f_sdlRenderer, f_sLocation);
}
TextureSelfManaged::TextureSelfManaged(SDL_Renderer* f_sdlRenderer, SDL_Texture *m_sdlTexture, string f_sLocation)
{
	if(f_sdlRenderer != nullptr)
		set_textureFromPointer(m_sdlTexture, f_sLocation);
}

TextureSelfManaged::~TextureSelfManaged()
{
	if(!m_bLoadedFromPointer && m_sdlTexture != nullptr)
		SDL_DestroyTexture(m_sdlTexture);
	m_sdlTexture = nullptr;
}

bool TextureSelfManaged::LoadTextureFromLocation(SDL_Renderer* f_sdlRenderer, string f_sLocation)
{
	if (m_sLastTextureLoaded == f_sLocation) return true; 
	if (f_sdlRenderer == nullptr) return false;

	SDL_Texture *l_texture = nullptr;
	//Load:
	SDL_Surface *l_surface = IMG_Load(f_sLocation.c_str());
	if (l_surface != nullptr)
	{
		l_texture = SDL_CreateTextureFromSurface(f_sdlRenderer, l_surface);
		if (l_texture == nullptr)
		{
			cout << "SPRITEBATCH::LoadTextureFromFile: Could not create texture from surface. Error: " << SDL_GetError() << endl;
		}
		else
		{
			SDL_DestroyTexture(m_sdlTexture);
			m_sdlTexture = l_texture;
			m_bHasLoadedTexture = true;
			m_sLastTextureLoaded = f_sLocation;
		}

	}
	else
	{
		cout << "SPRITEBATCH::Could not load image. " << f_sLocation << " . Error: " << SDL_GetError() << endl;
	}
	SDL_FreeSurface(l_surface);
	l_surface = nullptr;
	m_bLoadedFromPointer = false;
	return m_bHasLoadedTexture;
}

bool TextureSelfManaged::set_textureFromPointer(SDL_Texture* f_sdlTexture, string f_sLocation)
{
	if (f_sdlTexture != nullptr)
	{
		m_sdlTexture = f_sdlTexture;
		m_bHasLoadedTexture = true;
		if (f_sLocation != "")
			m_sLastTextureLoaded = f_sLocation;
		m_bLoadedFromPointer = true;
	}
	return m_bHasLoadedTexture;
}