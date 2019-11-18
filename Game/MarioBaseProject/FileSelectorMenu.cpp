#include "FileSelectorMenu.h"



FileSelectorMenu::FileSelectorMenu(SpriteBatch *f_sb, Audio *f_audio, string *f_sFileStruct, FileSelectorCompletePackage *f_complete)
{
	p_sb = f_sb;
	p_audio = f_audio;
	s_sFileStructure = f_sFileStruct;
	s_fileSelectorCompletePackage = f_complete;
}


FileSelectorMenu::~FileSelectorMenu()
{
	delete m_sprBackground;


	delete m_sprLettersTitle;
	delete m_sprLettersDescription;
	delete m_sprConfirmCancel;

	for (int i = 0; i < m_iMaxNumberOfFilesInEitherFolder; i++)
	{
		delete m_sprLettersTop[i];
		delete m_sprLettersSub[i];
	}
	delete m_llFileNames;
	delete m_sprLettersTopHeading;
	delete m_sprLettersSubHeading;

	delete m_letterTexture;
}

bool FileSelectorMenu::Update(float f_deltaTime)
{
	if (m_sTopFolderLocation == "")
	{
		load_metaData();
		load_audio();
	}

	/*if (!m_bDebug)
		debug_outputFileNames();
	m_bDebug = true;*/

	update_words();
	update_select();
	update_background(f_deltaTime);
	return true;
}

bool FileSelectorMenu::Draw()
{
	if (p_sb == nullptr) return false;
	if (m_sprBackground != nullptr)
	{
		m_sprBackground->RenderToScreen(p_sb->get_renderer());
		Vector2D *l_v2dBackground = new Vector2D(m_sprBackground->get_locationX(), m_sprBackground->get_locationY());
		l_v2dBackground->add_valueX(-1596);
		m_sprBackground->RenderToScreen(p_sb->get_renderer(), l_v2dBackground);
		delete l_v2dBackground;
	}

	if(m_sprLettersTitle != nullptr && m_sTitle != "")
		m_sprLettersTitle->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sTitle);

	if (m_sprLettersDescription != nullptr && m_sDescription != "")
		m_sprLettersDescription->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sDescription);

	if (m_sprLettersTopHeading != nullptr)
		m_sprLettersTopHeading->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sTopHeading);

	if(m_sprLettersSubHeading != nullptr)
		m_sprLettersSubHeading->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sSubHeading);
	for (int i = 0; i < m_iMaxNumberOfFilesInEitherFolder; i++)
	{
		if (m_sprLettersTop[i] != nullptr)
			m_sprLettersTop[i]->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sLettersTop[i]);

		if (m_sprLettersSub[i] != nullptr)
			m_sprLettersSub[i]->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sLettersSub[i]);
	}

	if (m_sprConfirmCancel != nullptr && m_sConfirmCancelText != "")
		m_sprConfirmCancel->RenderAsLetters(p_sb->get_renderer(), nullptr, m_sConfirmCancelText);

	return true;
}

bool FileSelectorMenu::load_metaData()
{
	ifstream infile(*s_sFileStructure);
	if (!infile.is_open())
	{
		cout << "bool  FileSelectorMenu::load_metaData(string " << *s_sFileStructure << "):  Could not open file." << endl;
		return false;
	}

	string f_sGetLine = "", f_sContent = "";
	int f_iLine = 0;
	LinkedList *l_llLine = nullptr;
	LinkedList *l_llCol = nullptr;
	LinkedList *l_llNew = nullptr;
	while (getline(infile, f_sGetLine))
	{
		if (f_sGetLine == "Meta")
		{
			while (f_sGetLine != "---")
			{
				getline(infile, f_sGetLine);
				f_sContent += f_sGetLine + "~";
			}
			if (!setup_meta(f_sContent))
				cout << "bool Item::load_fromFile: Animation not loaded" << endl;
		}
		else
		{
			if (m_llFileNames == nullptr)
			{
				m_llFileNames = new LinkedList(f_sGetLine);
				l_llLine = m_llFileNames;
				continue;
			}
			
			if (f_sGetLine[0] == '-')
			{
				if (l_llLine == nullptr)
					if (m_llFileNames != nullptr)
						l_llLine = m_llFileNames;
				l_llCol = l_llLine->get_data(nullptr);
				string l_sShaveFirstChar = Utility::StringLeft(f_sGetLine, 1);
				if (l_llCol == nullptr)
				{
					l_llNew = new LinkedList(l_sShaveFirstChar);
					l_llLine->set_data(l_llNew);
				}
				else
					l_llCol->AddNewToEnd(l_sShaveFirstChar);
				
			}
			else
			{
				l_llLine = m_llFileNames->AddNewToEnd(f_sGetLine);
				f_iLine++;
			}
		}
	}
	infile.close();
	return true;
}

bool FileSelectorMenu::load_audio()
{
	if (p_audio == nullptr) return false;
	if (m_iUIClick == -1)
		m_iUIClick = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/LQ_Click_Button.wav");

	if (m_iAcceptSound == -1)
		m_iAcceptSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/FA_Select_Button_1_4.wav");

	if (m_iMessagePopupSound == -1)
		m_iMessagePopupSound = p_audio->find_soundEffectNeverFail("Audio/SoundEffect/gamemusicstingersanduisfxpackpack2/UI/CGM3_Error_Button_05_4.wav");
	return true;
}

bool FileSelectorMenu::setup_meta(string f_sData)
{
	LinkedList *l_llLines = Utility::SplitStringToLL(f_sData, '~');
	if (l_llLines->length() <= 1) return false;
	string l_sWholeLine = "";
	for (int line = 0; line < l_llLines->length(); line++)
	{
		l_sWholeLine = l_llLines->read_data(line, l_sWholeLine);
		//cout << "l_sWholeLine: " << l_sWholeLine << endl;
		if (l_sWholeLine == "") continue;
		if (!Utility::SearchStringForChar(l_sWholeLine, ' ')) continue;
		string f_sParameter = l_sWholeLine, f_sValue = "";
		if (!Utility::SplitStringAtPoint(&f_sParameter, &f_sValue, ' '))continue;
		set_byString(f_sParameter, f_sValue);
	}
	delete l_llLines;

	return true;
}

bool FileSelectorMenu::set_byString(string f_sParameter, string f_sValue)
{
	string f_sParaUpper = Utility::StringToUpperCase(f_sParameter);//Shouldn't be case sensative
	string f_sValueUpper = Utility::StringToUpperCase(f_sValue);//Shouldn't be case sensative

	//cout << "para: " << f_sParaUpper << endl;
	//cout << "value: " << f_sValueUpper << endl;

	if (f_sParaUpper == "" || f_sValueUpper == "")
		return false;
	else if (f_sParaUpper == "TOPFOLDER:")
	{
		m_sTopFolderLocation = f_sValue;
		return true;
	}
	else if (f_sParaUpper == "SUBFOLDER:")
	{
		m_sSubFolderLocation = f_sValue;
		return true;
	}
	else if (f_sParaUpper == "TITLE:")
	{
		m_sTitle = f_sValue;
		return true;
	}
	else if (f_sParaUpper == "DESCRIPTION:")
	{
		m_sDescription = f_sValue;
		return true;
	}
	return false;
}

bool FileSelectorMenu::debug_outputFileNames()
{
	if (m_llFileNames == nullptr) return false;

	LinkedList *l_llLine;
	string l_sData = "";
	if (m_llFileNames == nullptr) return false;
	for (int i = 0; i < m_llFileNames->length(); i++)
	{
		cout << "i: " << i << " : ";
		l_llLine = m_llFileNames->read_data(i, nullptr);
		l_sData = m_llFileNames->read_data(i, l_sData);
		cout << l_sData << endl;
		if (l_llLine != nullptr)
		{
			for (int j = 0; j < l_llLine->length(); j++)
			{
				//cout << "l_llLine->length(): " << l_llLine->length() << endl;
				cout << "- " << l_llLine->read_data(j, l_sData) << endl;
			}
		}
		cout << endl;
	}

	return true;
}

bool FileSelectorMenu::update_words()
{
	if (p_sb == nullptr) return false;
	if (m_llFileNames == nullptr) return false;
	if (m_letterTexture == nullptr)
	{
		m_letterTexture = new TextureSelfManaged();
		if (!m_letterTexture->LoadTextureFromLocation(p_sb->get_renderer(), "Images/Mario3/Letters.png"))
			cout << "FileSelectorMenu::update_words(): Image file not loaded: " << "Images/Mario3/Letters.png" << endl;
	}
	if(m_sprLettersTopHeading == nullptr)
		setup_headings();
	LinkedList *l_llLine;
	string l_sData = "";
	int l_iLettersColour = 0;
	int l_iSubFoldersFound = 0;
	if (m_llFileNames == nullptr) return false;
	for (int i = 0; i < m_llFileNames->length(); i++)
	{
		if (m_iTopLevel == i)
			l_iLettersColour = 4;
		else
			l_iLettersColour = 0;

		l_sData = m_llFileNames->read_data(i, l_sData);
		update_letters(l_sData, true, i, l_iLettersColour);

		if (m_iTopLevel == i)
		{
			l_llLine = m_llFileNames->read_data(i, nullptr);
			if (l_llLine != nullptr)
			{
				for (int j = 0; j < l_llLine->length(); j++)
				{
					if (m_iSubLevel == j)
						l_iLettersColour = 6;
					else
						l_iLettersColour = 0;

					update_letters(l_llLine->read_data(j, l_sData), false, j, l_iLettersColour);
					l_iSubFoldersFound++;
				}
			}
		}
	}
	for (int i = m_iMaxNumberOfFilesInEitherFolder - 1; i >= l_iSubFoldersFound; i--)
	{
		if (m_sprLettersSub[i] != nullptr)
			delete m_sprLettersSub[i];
		m_sprLettersSub[i] = nullptr;
	}

	return true;
}

bool FileSelectorMenu::update_letters(string f_sData, bool f_bIsTop, int f_iNumber, int f_iRow)
{
	if (f_iNumber > m_iMaxNumberOfFilesInEitherFolder - 1) return false;
	SprLetters *l_sprLettersCurrent = nullptr;
	bool l_bMadeLetters = false;
	if (f_bIsTop)
	{
		if (m_sprLettersTop[f_iNumber] == nullptr)
		{
			m_sprLettersTop[f_iNumber] = new SprLetters();
			l_bMadeLetters = true;
		}
		l_sprLettersCurrent = m_sprLettersTop[f_iNumber];
		l_sprLettersCurrent->set_location(m_iXforTopFolder, (f_iNumber * (m_iLetterHeight + m_iLetterGap)) + m_iYMarginForFiles);
		m_sLettersTop[f_iNumber] = f_sData;
	}
	else
	{
		if (m_sprLettersSub[f_iNumber] == nullptr)
		{
			m_sprLettersSub[f_iNumber] = new SprLetters();
			l_bMadeLetters = true;
		}
		l_sprLettersCurrent = m_sprLettersSub[f_iNumber];
		l_sprLettersCurrent->set_location(m_iXforSubFolder, (f_iNumber * (m_iLetterHeight + m_iLetterGap)) + m_iYMarginForFiles);
		m_sLettersSub[f_iNumber] = f_sData;
	}
	if (l_bMadeLetters)//Genric things to setup the letters:
	{
		l_sprLettersCurrent->set_textureSelfManageFromPointer(m_letterTexture);
		l_sprLettersCurrent->set_singleSize(m_iLetterHeight, m_iLetterHeight);
	}

	l_sprLettersCurrent->set_lettersRow(f_iRow);
	l_sprLettersCurrent->set_numbersRow(f_iRow + 1);
	return true;
}

bool FileSelectorMenu::update_select()
{
	if (p_controls == nullptr) return false;
	
	Controls::mouseInfo *l_mouseInfo = p_controls->get_mouseInfo();
	if (l_mouseInfo->state == Controls::mouseInfo::msLeft)
		m_bHasLeftClick = true;
	else
	{
		if (m_bHasLeftClick)
		{
			SDL_Rect l_rectMouse = { *l_mouseInfo->x, *l_mouseInfo->y, 1, 1 };
			//Top Folder and Sub Folder checks:
			for (int i = 0; i < m_iMaxNumberOfFilesInEitherFolder; i++)
			{
				SDL_Rect l_letterCol;
				if (m_sprLettersTop[i] != nullptr)
				{
					l_letterCol = m_sprLettersTop[i]->get_collision();
					if (Utility::ReturnCollision(l_rectMouse, l_letterCol))//They clicked on this one:
					{
						m_iTopLevel = i;
						m_iSubLevel = -1;
						p_audio->play_soundEffect(m_iUIClick, 0);
					}
				}
				if (m_sprLettersSub[i] != nullptr)
				{
					l_letterCol = m_sprLettersSub[i]->get_collision();
					if (Utility::ReturnCollision(l_rectMouse, l_letterCol))//They clicked on this one:
					{
						m_iSubLevel = i;
						p_audio->play_soundEffect(m_iUIClick, 0);
					}
				}
			}
			//Confirm and Cancel
			if (m_sprConfirmCancel != nullptr)
			{
				SDL_Rect l_letterCol = m_sprConfirmCancel->get_collision();
				if (Utility::ReturnCollision(l_rectMouse, l_letterCol))//They clicked on this one:
				{
					if (m_iTopLevel == -1 || m_iSubLevel == -1)//They pressed cancel
					{
						s_fileSelectorCompletePackage->m_bConfirm = false;
						p_audio->play_soundEffect(m_iMessagePopupSound, 0);
					}
					else
					{
						s_fileSelectorCompletePackage->m_bConfirm = true;
						p_audio->play_soundEffect(m_iAcceptSound, 0);
					}

					s_fileSelectorCompletePackage->m_bComplete = true;
					s_fileSelectorCompletePackage->m_sTopFolderLocation = m_sTopFolderLocation;
					s_fileSelectorCompletePackage->m_sSubFolderLocation = m_sSubFolderLocation;
					if(m_iTopLevel != -1)
						s_fileSelectorCompletePackage->m_sTopFolderSelection = m_sLettersTop[m_iTopLevel];
					if (m_iSubLevel != -1)
						s_fileSelectorCompletePackage->m_sSubFolderSelection = m_sLettersSub[m_iSubLevel];
				}
			}
		}
		m_bHasLeftClick = false;
	}

	return true;
}

bool FileSelectorMenu::setup_headings()
{
	if (m_sprLettersTopHeading == nullptr)
	{
		m_sprLettersTopHeading = new SprLetters();
		m_sprLettersTopHeading->set_location(m_iXforTopFolder, m_iYMarginForHeadings);
		m_sprLettersTopHeading->set_textureSelfManageFromPointer(m_letterTexture);
		m_sprLettersTopHeading->set_singleSize(m_iLetterHeight, m_iLetterHeight);
		m_sprLettersTopHeading->set_lettersRow(2);
		m_sprLettersTopHeading->set_numbersRow(3);
		
		LinkedList *l_llFolderName = Utility::SplitStringToLL(m_sTopFolderLocation, '/');
		m_sTopHeading = l_llFolderName->read_data(l_llFolderName->length() - 1, m_sTopHeading);
		delete l_llFolderName;
	}

	if (m_sprLettersSubHeading == nullptr)
	{
		m_sprLettersSubHeading = new SprLetters();
		m_sprLettersSubHeading->set_location(m_iXforSubFolder, m_iYMarginForHeadings);
		m_sprLettersSubHeading->set_textureSelfManageFromPointer(m_letterTexture);
		m_sprLettersSubHeading->set_singleSize(m_iLetterHeight, m_iLetterHeight);
		m_sprLettersSubHeading->set_lettersRow(2);
		m_sprLettersSubHeading->set_numbersRow(3);

		LinkedList *l_llFolderName = Utility::SplitStringToLL(m_sSubFolderLocation, '/');
		m_sSubHeading = l_llFolderName->read_data(l_llFolderName->length() - 1, m_sSubHeading);
		delete l_llFolderName;
	}

	if (m_sprBackground == nullptr)
	{
		m_sprBackground = new Sprite();
		if (!m_sprBackground->LoadTextureFromFile(p_sb->get_renderer(), "Images/GameSelectMain/controllerm_background.png"))
			cout << "FileSelectorMenu::setup_headings(): m_sprBackground texture not loaded" << endl;
		m_sprBackground->set_screenSize(1596, 720);
		m_sprBackground->set_singleSize(m_sprBackground->get_actualSizeWidth(), m_sprBackground->get_actualSizeHeight());
	}

	if (m_sprLettersTitle == nullptr)
	{
		m_sprLettersTitle = new SprLetters();
		m_sprLettersTitle->set_location(m_iXforTopFolder, 10);
		m_sprLettersTitle->set_textureSelfManageFromPointer(m_letterTexture);
		m_sprLettersTitle->set_singleSize(m_iLetterHeight, m_iLetterHeight);
		if(m_sTitle.length() <= 13)
			m_sprLettersTitle->set_screenSize(m_iLetterHeight * 2, m_iLetterHeight * 2);
		else
			m_sprLettersTitle->set_screenSize(m_iLetterHeight, m_iLetterHeight);
		m_sprLettersTitle->set_lettersRow(0);
		m_sprLettersTitle->set_numbersRow(1);
	}

	if (m_sprLettersDescription == nullptr)
	{
		m_sprLettersDescription = new SprLetters();
		m_sprLettersDescription->set_location(m_iXforSubFolder, 10);
		m_sprLettersDescription->set_textureSelfManageFromPointer(m_letterTexture);
		m_sprLettersDescription->set_singleSize(m_iLetterHeight, m_iLetterHeight);
		m_sprLettersDescription->set_lettersRow(0);
		m_sprLettersDescription->set_numbersRow(1);
		m_sprLettersDescription->set_lineWidth(48);
	}

	if (m_sprConfirmCancel == nullptr)
	{
		m_sprConfirmCancel = new SprLetters();
		m_sprConfirmCancel->set_location(1280 - (16 * 8), 688);
		m_sprConfirmCancel->set_textureSelfManageFromPointer(m_letterTexture);
		m_sprConfirmCancel->set_singleSize(m_iLetterHeight, m_iLetterHeight);
		m_sprConfirmCancel->set_lettersRow(0);
		m_sprConfirmCancel->set_numbersRow(1);
	}
	return true;
}

bool FileSelectorMenu::update_background(float f_deltaTime)
{
	if (m_sprBackground == nullptr) return false;

	m_sprBackground->move_location(f_deltaTime * 0.01f, 0);
	if (m_sprBackground->get_locationX() > 1596)
		m_sprBackground->set_location(m_sprBackground->get_locationX() - 1596, m_sprBackground->get_locationY());

	if (m_sprConfirmCancel != nullptr)
	{
		if (m_iTopLevel == -1 || m_iSubLevel == -1)
		{
			m_sConfirmCancelText = "Cancel";
			m_sprConfirmCancel->set_lettersRow(4);
			m_sprConfirmCancel->set_numbersRow(5);
		}
		else
		{
			m_sConfirmCancelText = "Confirm";
			m_sprConfirmCancel->set_lettersRow(8);
			m_sprConfirmCancel->set_numbersRow(9);
		}
	}
	return true;
}