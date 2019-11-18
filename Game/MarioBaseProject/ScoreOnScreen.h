#pragma once
#include "Platformer_Character.h"
class ScoreOnScreen : public Platformer_Character
{
private:
	float m_fScoreTick = 0;
	int m_iScoreCombo = 0;

	float m_fOriginalX = -1;
	float m_fOriginalY = -1;

	float m_fGoalX = -1;
	float m_fGoalY = -1;

	int m_iLastCombo = -1;
	bool m_bReachedPeak = false;

	int *m_iPoints = nullptr;
public:
	ScoreOnScreen();
	virtual ~ScoreOnScreen();

	virtual bool Update(float f_deltaTime);

	void set_pointsPtr(int *f_iPoints) { m_iPoints = f_iPoints; }

	bool new_score(float f_fX, float f_fY, bool f_bAdditive = true);

	bool new_scoreKnown(float f_fX, float f_fY, int f_iScore);
};

