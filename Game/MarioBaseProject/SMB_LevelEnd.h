#pragma once
#include <string>
#include "Sprite.h"
#include "SprLetters.h"
#include "SPRITEBATCH.h"
#include "Controls.h"
#include "Audio.h"

using namespace std;

class SMB_PlayerPlatformer;

class SMB_LevelEnd
{
protected:
	SpriteBatch *p_sb = nullptr;
	Audio *p_audio = nullptr;
private:
	int m_iPlayerCount = 1;
	SMB_PlayerPlatformer *m_playerOne = nullptr;
	SMB_PlayerPlatformer *m_playerTwo = nullptr;

	int *s_iEndOfLevel = nullptr;
	int *m_iPlayerOneFinishedLevel = 0;
	int *m_iPlayerTwoFinishedLevel = 0;

	bool m_bLoaded = false;
	TextureSelfManaged *m_tsmLetters = nullptr;
	SprLetters *m_sprlTopLine = nullptr;
	SprLetters *m_sprlSecondLine = nullptr;
	SprLetters *m_sprlThirdLine = nullptr;
	Sprite *m_sprTopPlayerCard = nullptr;
	Sprite *m_sprBottomPlayerCard = nullptr;

	string m_sFirstPlayerText = "";
	string m_sSecondPlayerText = "";

	//Audio
	int m_iMusicTrack = -1;
	bool m_bStartPlay = false;
	float m_fMusicLength = 5000;
	float m_fCurrentTimePassed = 0;
public:
	SMB_LevelEnd();
	virtual ~SMB_LevelEnd();

	//Defaults
	virtual bool Update(float f_deltaTime);
	virtual bool Draw();

	//Set
	void set_spritebatch(SpriteBatch *f_sb) { p_sb = f_sb; }
	void set_audio(Audio *f_audio){p_audio = f_audio;}
	void set_endOfLevel(int *f_iData) { s_iEndOfLevel = f_iData; }
	void set_playerOne(SMB_PlayerPlatformer *f_player) { m_playerOne = f_player; }
	void set_playerTwo(SMB_PlayerPlatformer *f_player) { m_playerTwo = f_player; }
	void set_playerOneEndCard(int *f_iCard) { m_iPlayerOneFinishedLevel = f_iCard; }
	void set_playerTwoEndCard(int *f_iCard) { m_iPlayerTwoFinishedLevel = f_iCard; }

private:
	bool load_content();

	bool update_textAndFont();
	bool update_endOfLevel(float f_deltaTime);
};