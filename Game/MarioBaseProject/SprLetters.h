#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Sprite.h"
#include "Utility.h"

#include <string>

using namespace::std;

class SprLetters : public Sprite
{
private:
	int m_iLettersStartRow = 0, m_iLettersStartCol = 0;
	int m_iNumbersStartRow = 0, m_iNumbersStartCol = 0;

	bool m_bDifforentUpperAndLower = false;

	int m_iLetterWidth = 0;

	SDL_Rect m_rectCollision;
public:
	SprLetters();
	~SprLetters();

	//Sets
	void set_lettersRow(int f_iInput) { m_iLettersStartRow = f_iInput; }
	void set_lettersCol(int f_iInput) { m_iLettersStartCol = f_iInput; }
	void set_lettersLocation(int f_iRow, int f_iCol) { m_iLettersStartRow = f_iRow; m_iLettersStartCol = f_iCol;}

	void set_numbersRow(int f_iInput) { m_iNumbersStartRow = f_iInput; }
	void set_numbersCol(int f_iInput) { m_iNumbersStartCol = f_iInput; }
	void set_numbersLocation(int f_iRow, int f_iCol) { m_iNumbersStartRow = f_iRow; m_iNumbersStartCol = f_iCol; }

	void set_difforentUpperAndLower(bool f_bInput){ m_bDifforentUpperAndLower = f_bInput; }

	void set_lineWidth(int f_i) { m_iLetterWidth = f_i; }

	//GETS
	SDL_Rect get_collision() { return m_rectCollision; }

	bool RenderAsLetters(SDL_Renderer* f_sdlRenderer, Vector2D *f_v2dLocation = nullptr, string f_sText = "");
};

