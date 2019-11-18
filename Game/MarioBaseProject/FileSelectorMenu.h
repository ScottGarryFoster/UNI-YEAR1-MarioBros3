#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>
#include <fstream>

#include "SPRITEBATCH.h"
#include "LinkedList.h"
#include "Camera.h"
#include "Controls.h"
#include "SprLetters.h"
#include "Audio.h"

using namespace std;

/*
	NOTE FOR FUTURE SCOTT:
	Might be nice to add a 'NEW' option in this so you can make your own files.
	If you've time that is
*/
class FileSelectorMenu
{
public:
	struct FileSelectorCompletePackage
	{
		bool m_bComplete = false;
		bool m_bConfirm = false;
		string m_sTopFolderLocation = "";
		string m_sSubFolderLocation = "";
		string m_sTopFolderSelection = "";
		string m_sSubFolderSelection = "";
	};
private:
	bool m_bDebug = false;

	SpriteBatch *p_sb = nullptr;
	Controls *p_controls = nullptr;
	Audio *p_audio = nullptr;

	Sprite *m_sprBackground = nullptr;

	FileSelectorCompletePackage *s_fileSelectorCompletePackage = nullptr;
	string *s_sFileStructure = nullptr;
	string m_sTopFolderLocation = "";
	string m_sSubFolderLocation = "";

	SprLetters *m_sprLettersTitle = nullptr;
	string m_sTitle = "";
	SprLetters *m_sprLettersDescription = nullptr;
	string m_sDescription = "";
	SprLetters *m_sprConfirmCancel = nullptr;
	string m_sConfirmCancelText = "";

	int m_iMaxNumberOfFilesInEitherFolder = 34;//You'll still need to define the size in the arrays below
	LinkedList *m_llFileNames = nullptr;
	SprLetters *m_sprLettersTopHeading = nullptr;
	string m_sTopHeading = "";
	SprLetters *m_sprLettersTop[34] = { nullptr };
	string m_sLettersTop[34] = { "" };
	SprLetters *m_sprLettersSubHeading = nullptr;
	string m_sSubHeading = "";
	SprLetters *m_sprLettersSub[34] = { nullptr };
	string m_sLettersSub[34] = { "" };

	TextureSelfManaged *m_letterTexture = nullptr;

	int m_iTopLevel = -1;
	int m_iSubLevel = -1;

	int m_iLetterHeight = 16;
	int m_iLetterGap = 2;
	int m_iYMarginForHeadings = 75;
	int m_iYMarginForFiles = 100;
	int m_iXforTopFolder = 50;
	int m_iXforSubFolder = 500;

	//Mouse stuff:
	bool m_bHasLeftClick = false;

	//Audio
	int m_iUIClick = -1;
	int m_iAcceptSound = -1;
	int m_iMessagePopupSound = -1;
public:
	FileSelectorMenu(SpriteBatch *f_sb, Audio *f_audio, string *f_sFileStruct, FileSelectorCompletePackage *f_complete);
	virtual ~FileSelectorMenu();

	virtual bool Update(float f_deltaTime);
	virtual bool Draw();

	//SET
	void set_controls(Controls *f_controls) { p_controls = f_controls; }

private:
	bool load_metaData();
	bool load_audio();
	bool setup_meta(string f_sData);
	bool set_byString(string f_sParameter, string f_sValue);

	bool update_words();
	bool update_letters(string f_sData, bool f_bIsTop, int f_iNumber, int f_iRow = 0);
	bool update_select();
	bool update_background(float f_deltaTime);

	bool setup_headings();

	bool debug_outputFileNames();
};

