#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include <iostream>
#include <string>

class Vector2D
{
public:
	int m_iX = 0, m_iY = 0;
	Vector2D();
	Vector2D(int x, int y);
	~Vector2D();

	void set_value(int f_iX, int f_iY) { m_iX = f_iX; m_iY = f_iY; }

	void add_values(int f_iX, int f_iY) { m_iX += f_iX; m_iY += f_iY;}
	void add_valueX(int f_iAdd) { m_iX += f_iAdd; }
	void add_valueY(int f_iAdd) { m_iY += f_iAdd; }
};

