#include "SprLetters.h"



SprLetters::SprLetters()
{
}


SprLetters::~SprLetters()
{

}

bool SprLetters::RenderAsLetters(SDL_Renderer* f_sdlRenderer, Vector2D *f_v2dLocation, string f_sText)
{
	if (f_sdlRenderer == nullptr) return false;
	if (f_sText == "")return false;
	Vector2D *l_v2dLocation = nullptr;
	bool f_bDidMakeVector2D = false;
	if (f_v2dLocation == nullptr)
	{
		l_v2dLocation = new Vector2D(get_locationX(), get_locationY());
		f_bDidMakeVector2D = true;
	}
	else
		l_v2dLocation = f_v2dLocation;

	int l_iV2DxLocation = l_v2dLocation->m_iX;

	m_rectCollision.x = l_v2dLocation->m_iX;
	m_rectCollision.y = l_v2dLocation->m_iY;
	m_rectCollision.h = get_actualSizeHeight();
	m_rectCollision.w = 0;

	if (!m_bDifforentUpperAndLower) f_sText = Utility::StringToUpperCase(f_sText);

	int l_iRow = m_iLettersStartRow, l_iCol = m_iLettersStartCol;
	int l_iCharAsInt = 0;
	string l_sToBeConverted = "";
	string l_sCurrentWord = "";

	LinkedList *l_llStringAsWords = Utility::SplitStringToLL(f_sText, ' ');

	bool *l_bErrorCheck = new bool();
	update_actualsize();

	int l_iLetterCount = 0;

	for (int word = 0; word < l_llStringAsWords->length(); word++)
	{
		//Okay let's see if we should draw the next word:
		l_sCurrentWord = l_llStringAsWords->read_data(word, l_sCurrentWord);
		if (l_sCurrentWord == "Unfound") continue;
		if (m_iLetterWidth > 0)
		{
			l_iLetterCount += l_sCurrentWord.length();
			if (l_iLetterCount > m_iLetterWidth)
			{
				l_v2dLocation->add_valueY(get_actualSizeHeight() * 1.5f);
				l_iLetterCount = 0;
				l_v2dLocation->set_value(l_iV2DxLocation, l_v2dLocation->m_iY);
			}
			l_iLetterCount++;//Spaces count
		}

		for (int i = 0; i < l_sCurrentWord.length(); i++)
		{
			l_iCol = m_iLettersStartCol;
			l_iCharAsInt = Utility::convert_charToInt(l_sCurrentWord[i]);
			if (l_sCurrentWord[i] == ' ') { l_v2dLocation->add_valueX(get_actualSizeWidth()); m_rectCollision.w += get_actualSizeWidth();  continue; }

			if (l_iCharAsInt > 26 || l_iCharAsInt < 1)
			{
				switch (l_sCurrentWord[i])
				{
				case '|':
					l_v2dLocation->add_valueY(get_actualSizeHeight() * 1.5f);
					l_v2dLocation->set_value(l_iV2DxLocation, l_v2dLocation->m_iY);
					l_iLetterCount = 0;
					continue;
					break;
				case '?':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 10;
					break;
				case '.':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 11;
					break;
				case '!':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 12;
					break;
				case '_':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 13;
					break;
				case '-':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 14;
					break;
				case ',':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 15;
					break;
				case '(':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 16;
					break;
				case ')':
					l_iRow = m_iNumbersStartRow;
					l_iCol = 17;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					//Is Number
					l_iRow = m_iNumbersStartRow;
					l_iCol = m_iNumbersStartCol;
					l_sToBeConverted = l_sCurrentWord[i];
					l_iCol += Utility::convert_stringToInt(l_sToBeConverted, l_bErrorCheck);
					if (!l_bErrorCheck) continue;
					break;
				}
			}
			else
			{
				//Is Letter
				l_iRow = m_iLettersStartRow;
				l_iCol += l_iCharAsInt - 1;
			}

			set_singleLocation(l_iRow, l_iCol);
			RenderToScreen(f_sdlRenderer, l_v2dLocation);
			l_v2dLocation->add_valueX(get_actualSizeWidth());
			m_rectCollision.w += get_actualSizeWidth();
		}
		l_v2dLocation->add_valueX(get_actualSizeWidth());
	}
	m_rectCollision.w += get_actualSizeWidth();

	delete l_bErrorCheck; l_bErrorCheck = nullptr;
	if(f_bDidMakeVector2D)//If we make it we delete it
		delete l_v2dLocation; l_v2dLocation = nullptr;

	delete l_llStringAsWords; l_llStringAsWords = nullptr;
	return true;
}