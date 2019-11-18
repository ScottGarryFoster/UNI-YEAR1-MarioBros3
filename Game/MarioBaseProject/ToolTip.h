#pragma once
#include "Sprite.h"
#include "SprLetters.h"
#include "SPRITEBATCH.h"
#include "Controls.h"
#include "Vector2D.h"
#include <string>

using namespace std;

class ToolTip
{
private:
	SpriteBatch *p_sb = nullptr;
	Controls *p_controls = nullptr;

	Sprite *m_sprBackground = nullptr;
	SprLetters *m_sprLetters = nullptr;
	string m_sWords = "";
	TextureSelfManaged *m_tsmLetters = nullptr;
	
	SDL_Rect m_rectLetters;
	Vector2D m_v2dCursorLocation;

	bool m_bShowToolTip = false;
	int m_iSetID = -1;
public:
	ToolTip(SpriteBatch *f_sb, Controls *f_controls);
	virtual ~ToolTip();

	//Defaults:
	bool Update(float f_deltaTime);
	bool Draw();

	//Set
	void set_showToolTip(bool f_bData) { m_bShowToolTip = f_bData; }
	void set_words(string f_sData) { m_sWords = f_sData; }
	void set_id(int f_iData) { m_iSetID = f_iData; }

	//Gets
	int get_id() { return m_iSetID; }
};