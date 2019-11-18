#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include <iostream>
#include "SPRITEBATCH.h"
#include "Controls.h"
#include "Audio.h"

using namespace::std;

class GameEngine
{
private:
	SDL_Window * gWindow = NULL;
	SDL_Renderer *m_sdlRenderer = nullptr;

	SDL_Texture *m_img = nullptr;
	//long long int m_illOldTime = 0;
	Uint8 m_illOldTime = 0;
	float m_illTimeDelta = 0;
	//double deltaTime = 0;//(double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
	double deltaTime = 0;//(double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

	bool m_bQuitPressedInGame = false;
protected:
	SDL_Event *m_sdlEvent = nullptr;

	SpriteBatch *p_spriteBatch = nullptr;
	Controls *p_controls = nullptr;
	Audio *p_audio = nullptr;
public:
	GameEngine(int argc, char* args[]);
	~GameEngine();

	//SDL Built
	bool InitSDL();
	void CloseSDL();
protected:
	//Program
	void GraphicsUpdate();
	bool EventUpdate();//Updates overall events
	
	void GameEngine_BeforeLoop();
	virtual void LoadContent();
	virtual void AfterContent();
	virtual bool GameLoop(float f_deltaTime);

	void PressQuit() { m_bQuitPressedInGame = true; }
};

