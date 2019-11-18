#include "GameEngine.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Utility.h"
#include "SPRITEBATCH.h"

#include <iostream>


GameEngine::GameEngine(int argc, char* args[])
{
	p_spriteBatch = new SpriteBatch();
	m_sdlEvent = new SDL_Event();
	p_controls = new Controls();
	p_audio = new Audio();
}


GameEngine::~GameEngine()
{
	delete p_spriteBatch; p_spriteBatch = nullptr;
	delete m_sdlEvent; m_sdlEvent = nullptr;
}

void GameEngine::GraphicsUpdate()
{
	GameEngine_BeforeLoop();
	LoadContent();
	bool l_bQuit = false;
	if (InitSDL())
	{
		while (!l_bQuit)
		{
			Uint8 l_illNewTime = SDL_GetTicks();
			m_illTimeDelta = (float)(l_illNewTime - m_illOldTime);
			if (m_illTimeDelta < 0)
			{
				m_illTimeDelta = 0;
				l_illNewTime = 0;
			}
			m_illOldTime = l_illNewTime;
			
			l_bQuit = GameLoop(m_illTimeDelta);
			
			
		}
			
	}

	CloseSDL();
}

void GameEngine::GameEngine_BeforeLoop()
{
	if (m_img == nullptr)
	{
		// load our image
		m_img = IMG_LoadTexture(m_sdlRenderer, "mario.png");
	}
	if (p_controls != nullptr)
	{
		p_controls->WipeKeys();
	}
}

void GameEngine::LoadContent()
{

}

void GameEngine::AfterContent()//Loads after window loads
{
}

bool GameEngine::InitSDL()
{
	//if (SDL_GameControllerAddMappingsFromFile("GameFiles/Controllers/gamecontrollerdb.txt") == -1) cout << "NOT ADDED" << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)//SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)//SDL_INIT_JOYSTICK |  SDL_INIT_VIDEO | SDL_INIT_EVERYTHING | SDL_INIT_GAMECONTROLLER
	{
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Games Engine Creation",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}
		m_sdlRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		p_spriteBatch->set_renderer(m_sdlRenderer);
		p_controls->set_spriteBatch(p_spriteBatch);
		p_controls->initialise_joysticks();
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			cout << "GameEngine::InitSDL: Mixer could not initialise. Error: " << Mix_GetError() << endl;
			return false;
		}
		else
			cout << "GameEngine::InitSDL: Mixer initialised" << endl;
	}

	AfterContent();
	return true;
}

void GameEngine::CloseSDL()
{
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit;
	SDL_Quit;
}

bool GameEngine::EventUpdate()
{
	SDL_PollEvent(m_sdlEvent);

	//p_controls->UpdateKeyboard(m_sdlEvent);

	switch (m_sdlEvent->type)
	{
	case SDL_QUIT:
		return true;
	case SDL_JOYDEVICEREMOVED:
		p_controls->initialise_joysticks();
		break;
	case SDL_JOYDEVICEADDED://m_sdlEvent->jdevice.which
		p_controls->initialise_joysticks();
		break;
	}
	if (m_bQuitPressedInGame) return true;
	return false;
}

bool GameEngine::GameLoop(float f_deltaTime)
{
	cout << "GameEngine::GameLoop(): Please use child class." << endl;
	return false;
}