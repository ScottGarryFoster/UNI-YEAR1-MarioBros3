#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include "Vector2D.h"
#include "TextureSelfManaged.h"
#include "Controls.h"
#include "Sprite.h"
#include "SprLetters.h"
#include "SPRITEBATCH.h"
#include "Audio.h"

#include <iostream>
#include <string>

using namespace::std;

class AlertBox
{
public:
	enum AlertBoxType : int {YesAndNo = 0};
private:
	Controls *m_controls = nullptr;
	SpriteBatch *m_sb = nullptr;
	Audio *m_audio = nullptr;
	
	Sprite *m_sprBlackfade = nullptr;
	Sprite *m_sprAlertBackground = nullptr;
	SprLetters *m_lettersTitle = nullptr;
	SprLetters *m_lettersContent = nullptr;

	Sprite *m_sprButtons = nullptr;
	Vector2D m_v2dSecondButton;
	SprLetters *m_lettersLeftButton = nullptr;
	SprLetters *m_lettersRightButton = nullptr;

	string m_sTitle = "";
	string m_sContent = "";
	string m_sLeftButton = "";
	string m_sRightButton = "";
	AlertBoxType m_abtype = YesAndNo;

	int m_iOptionSelected = 0;
	int m_iPreviousOptionSelected = 0;
	int m_iEnteredSelection = 0;
	bool m_bReadyForInput = false;

	int m_iReponceLeft = 1;
	int m_iReponceRight = 2;

	int m_iMouseStateRecorded = 0;
	Controls::mouseInfo* m_cMouseInfo = nullptr;

	//Audio
	int m_iUIClickSound = -1;
	int m_iAcceptSound = -1;
public:
	AlertBox(Controls *f_controls, SpriteBatch *f_sb);
	~AlertBox();

	bool Update();
	bool Draw();
	bool LoadContent();

	void set_audio(Audio *f_audio) { m_audio = f_audio; }
	void set_title(string f_s) { m_sTitle = f_s; }
	void set_content(string f_s) { m_sContent = f_s; }
	void set_leftButton(string f_s) { m_sLeftButton = f_s; }
	void set_rightButton(string f_s) { m_sRightButton = f_s; }

	void set_responceLeft(int f_s) { m_iReponceLeft = f_s; }
	void set_responceRight(int f_s) { m_iReponceRight = f_s; }

	int get_enteredSelection() { return m_iEnteredSelection; }
private:
	bool update_input();
	bool update_keyboardInput();
	bool update_mouseInput();
	bool update_selectionInput();
};

