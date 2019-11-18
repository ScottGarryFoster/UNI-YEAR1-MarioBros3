#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Constants.h"
#include "TextureSelfManaged.h"
#include <iostream>
#include <string>

#include "Controls.h"
#include "Camera.h"

#include "Platformer_Character.h"
#include "SMB_ScreenUI.h"
#include "ScoreOnScreen.h"

class EnemyEffect
{
public:
	enum EnemyEffectEffect : int {eeeNone = 0, eeeIDie, eeeTheyTakeDamage, eeeChangeDirection, eeeStomped, eeeUpdateState, eeeKillNoneShell, eeeKillBothShells};
private:
	bool m_bSetup = false;
	int *s_iEnemyState = nullptr;

	//Setup:
	string m_sActor = "";
	bool m_bDirectionLeft = false;
	bool m_bDirectionRight = false;
	bool m_bDirectionTop = false;
	bool m_bDirectionBottom = false;
	EnemyEffectEffect m_effect = eeeNone;
	int m_iEffectState = -1;
	int m_iEffectConditionState = -1;
	string m_sEffectAddition = "";
public:
	EnemyEffect();
	virtual ~EnemyEffect();

	//Defaults
	bool Update(float f_deltaTime);
	bool Draw();

	bool setup_effect(string f_sData);
	bool set_byString(string f_sPara, string f_sValue);
	void set_enemyState(int *f_iData) { s_iEnemyState = f_iData; }

	int get_effectConditionState() { return m_iEffectConditionState; }
	int get_newState() { return m_iEffectState; }
	string get_newStateAddition() { return m_sEffectAddition; }

	bool update_actionActivation(int f_iDirection, int f_iActor, bool f_bPlayerSize, int *f_iFeedback);

	bool debug_enemyEffect();
};