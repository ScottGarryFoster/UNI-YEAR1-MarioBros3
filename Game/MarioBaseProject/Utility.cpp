#include "Utility.h"
#include <string>

using namespace::std;

Utility::Utility()
{
	//p_bDebug_SplitString = false;
}


Utility::~Utility()
{
}

char* Utility::StringToCharArray(string f_sData)
{
	char *l_charReturn = &f_sData[0u];
	return l_charReturn;
}

bool Utility::SplitStringAtPoint(string *f_sLeft, string *f_sRight, char f_cDelim)
{
	if (f_sLeft == nullptr) return false;
	if (f_sRight == nullptr) return false;

	string f_sWorkingLeft = *f_sLeft;

	string f_sReturnLeft = "", f_sReturnRight = "";
	char f_cCurrent;

	bool f_bFoundDelim = false;
	int i = -1;
	while (true)
	{
		i++;
		if (f_sWorkingLeft[i] == '\0')
			break;

		f_cCurrent = f_sWorkingLeft[i];
		if(p_bDebug_SplitString) cout << "CHAR: " << f_cCurrent << ", " << endl;
		if (!f_bFoundDelim)
		{
			if (f_cCurrent == f_cDelim)
			{
				if(p_bDebug_SplitString) cout << "FOUND" << endl;
				f_bFoundDelim = true;
				continue;
			}
			else
				f_sReturnLeft += f_cCurrent;
		}
		else
		{
			f_sReturnRight += f_cCurrent;
		}
	}
	if(p_bDebug_SplitString) cout << "---------- OUT OF SPLIT LOOP" << endl;
	if (!f_bFoundDelim)
	{
		//cout << "bool Utility::SplitStringAtPoint: Deliminator not found: " << f_cDelim << endl;
		return false;
	}
	*f_sLeft = f_sReturnLeft;
	*f_sRight = f_sReturnRight;
	return true;
}

int Utility::SearchStringForChar(string f_sData, char f_cSearch)
{
	if (f_sData == "") return -1;
	if (f_cSearch == '\0') return -1;

	for (int i = 0; i < f_sData.length(); i++)
		if (f_sData[i] == f_cSearch)
			return i;
	return -1;
}

int Utility::convert_stringToInt(string f_sData, bool *f_bSuccess)
{
	int l_iReturn = -1;
	string::size_type sz;
	try
	{
		l_iReturn = stoi(f_sData, &sz);
	}
	catch (...)
	{
		if(f_bSuccess != nullptr)
			*f_bSuccess = false;
		return -1;
	}
	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return l_iReturn;
}
int Utility::convert_charToInt(char f_cData)
{
	int i = f_cData - 64;
	return i;
}

char Utility::convert_intToChar(int f_iData)
{
	char c = (char)(f_iData + 64);
	return c;
}

bool Utility::GetRowAndColFromNumber(int f_iNumberOfRows, int f_iNumberOfCols, int f_iConvertNumber, int &f_iResultRows, int &f_iResultCols)
{
	if (f_iNumberOfRows < 0)
	{
		//cout << "Utility::GetRowAndColFromNumber: f_iNumberOfRows < 0" << endl;
		return false;
	}
	if (f_iNumberOfCols < 0)
	{
		//cout << "Utility::GetRowAndColFromNumber: f_iNumberOfCols < 0" << endl;
		return false;
	}
	if (f_iConvertNumber < 0)
	{
		//cout << "Utility::GetRowAndColFromNumber: f_iConvertNumber < 0" << endl;
		return false;
	}

	f_iResultRows = f_iConvertNumber / f_iNumberOfCols;
	f_iResultCols = f_iConvertNumber - (f_iResultRows * f_iNumberOfCols);
	return true;
}

bool Utility::ReturnCollision(SDL_Rect l_rectOne, SDL_Rect l_rectTwo)
{
	//cout << "l_rectOne.y" << l_rectOne.y << endl;
	if (l_rectOne.y + l_rectOne.h < l_rectTwo.y)
		return false;
	if (l_rectTwo.y + l_rectTwo.h < l_rectOne.y)
		return false;
	if (l_rectOne.x + l_rectOne.w < l_rectTwo.x)
		return false;
	if (l_rectTwo.x + l_rectTwo.w < l_rectOne.x)
		return false;
	return true;
}

bool Utility::ReturnDrawForCulling(SDL_Rect *l_rectLocation, SDL_Rect *l_rectSource, float l_fCamX, float l_fCamScreenX, float l_fCamScreenWidth, float l_fScaleSize)
{
	//return true;
	float l_fRenderWidth = l_rectLocation->w;
	float l_fRenderHeight = l_rectLocation->h;
	float l_fScreenLocationX = l_rectLocation->x - l_fCamX + l_fCamScreenX;
	//float l_fScreenLocationY = l_rectLocation->y - m_camera->get_y() + m_camera->get_screenY();

	int l_iSourceX = l_rectSource->x;
	int l_iSourceY = l_rectSource->y;
	int l_iSourceWidth = l_rectSource->w;
	int l_iSourceHeight = l_rectSource->h;

	if (l_fScreenLocationX + l_fRenderWidth < l_fCamScreenX)
	{
		return false;
	}
	else if (l_fScreenLocationX < l_fCamScreenX)
	{
		//If they're moving off screen on the left
		//l_fRenderWidth -= l_fCamX - ((tile - 1) * l_rectLocation->w);
		l_fRenderWidth -= l_fCamX - (l_rectLocation->x);
		l_rectLocation->x -= l_fRenderWidth;
		l_rectLocation->x += l_rectLocation->w;
		l_iSourceWidth = l_fRenderWidth / l_fScaleSize;
		l_iSourceX += l_rectSource->w - l_iSourceWidth;
	}
	else if (l_fScreenLocationX > l_fCamScreenX + l_fCamScreenWidth)
	{
		return false;
	}
	else if (l_fScreenLocationX + l_fRenderWidth > l_fCamScreenX + l_fCamScreenWidth)
	{
		//If they're moving on screen on the right
		//l_fRenderWidth += l_fCamX + l_fCamScreenWidth - ((tile)*  l_rectLocation->w);
		l_fRenderWidth += l_fCamX + l_fCamScreenWidth - (l_rectLocation->x + l_fRenderWidth);
		l_iSourceWidth = l_fRenderWidth / l_fScaleSize;
	}

	l_rectLocation = {};

	return true;
}

string Utility::StringToUpperCase(string f_sInput)
{
	for (int i = 0; i < f_sInput.length(); i++)
		f_sInput[i] = toupper(f_sInput[i]);
	return f_sInput;
}

string Utility::AddLeadingZeros(int f_iInput, int f_iLength)
{
	string l_sReturn = to_string(f_iInput);
	int l_iLengthAdd = f_iLength - l_sReturn.length();
	for (int i = 0; i < l_iLengthAdd; i++)
		l_sReturn = '0' + l_sReturn;
	return l_sReturn;
}

LinkedList* Utility::SplitStringToLL(string f_sText, char f_cDelim)
{
	LinkedList *l_linkedList = new LinkedList();
	LinkedList *l_linkedListCurrent = l_linkedList;

	string l_sCurrentEntry = "";

	for (int i = 0; i < f_sText.length(); i++)
	{
		if (f_sText[i] == f_cDelim)
		{
			l_linkedListCurrent->set_data(l_sCurrentEntry);
			l_linkedListCurrent = l_linkedList->AddNewToEnd();
			l_sCurrentEntry = "";
			continue;
		}
		l_sCurrentEntry += f_sText[i];
	}
	if(l_sCurrentEntry != "")
		l_linkedListCurrent->set_data(l_sCurrentEntry);

	return l_linkedList;
}

string Utility::StringLeft(string f_sData, int f_iStartChar)
{
	string l_sReturn = f_sData;
	for (int i = 0; i < f_sData.length(); i++)
	{
		if (i == 0) l_sReturn = "";
		if (i >= f_iStartChar)
			l_sReturn += f_sData[i];
	}
	return l_sReturn;
}

int Utility::RoundFloat(float f_fData, int f_iRound)
{
	int l_iData = (int)f_fData;
	float l_fRemainder = l_iData - f_fData;

	if (l_fRemainder > 0 && f_iRound > 0)
		return l_iData + 1;
	else if (l_fRemainder < 0.5f && f_iRound == 0)
		return l_iData;
	else if (l_fRemainder > 0.5f && f_iRound == 0)
		return l_iData + 1;
	return l_iData;
}

LinkedList* Utility::UniqueElements(LinkedList *f_llSource, string f_sData)
{
	/*
		This basically checks if the element has appeared previouslys.
		If it has then s != f or the compare is never updated.
		If the element is unique it gets written to a LinkedList.
		Nullptr means you didn't have anything in any slot (Undefined/Unfound).

		It'll shave off any Undefined and Unfound.

		YOU WILL NEED TO GET RID OF THE LINKED LIST ON YOUR OWN
	*/
	//Test on string
	LinkedList *l_llUniqueList = nullptr, *l_llNewElement = nullptr, *l_llCurrent = nullptr;
	LinkedList *l_llOriginal = nullptr , *l_llCompare = nullptr;
	for (int f = 0; f < f_llSource->length(); f++)
	{
		l_llOriginal = f_llSource->ReadFromList(f);
		if (l_llOriginal == nullptr) continue;
		int s = 0;
		for (s = 0; s < f; s++)
		{
			l_llCompare = f_llSource->ReadFromList(s);
			if (l_llCompare->get_data(f_sData) == l_llOriginal->get_data(f_sData))
				break;
		}

		if (l_llCompare == nullptr)
			if (s == 0)//Always write the first one
				l_llCompare = l_llOriginal;//Because the loop above won't run if it's 0
			else 
				continue;
		
		if (f == s && l_llOriginal->get_data(f_sData) != "Undefined" && l_llOriginal->get_data(f_sData) != "Unfound")
		{
			if (l_llUniqueList == nullptr)
			{
				l_llUniqueList = new LinkedList();
				l_llNewElement = l_llUniqueList;
			}
			else
				l_llNewElement = l_llUniqueList->AddNewToEnd();

			l_llCurrent = l_llOriginal;//Don't want to do this a bunch of times

			l_llNewElement->set_data(l_llCurrent->get_data(f_sData));
			l_llNewElement->set_data(l_llCurrent->get_data(0));
			l_llNewElement->set_data(l_llCurrent->get_data(0.0f));
			l_llNewElement->set_data(l_llCurrent->get_data(' '));
			l_llNewElement->set_data(l_llCurrent->get_data(true));
			l_llNewElement->set_data(l_llCurrent->get_data(nullptr));
		}	
	}
	return l_llUniqueList;
}

string Utility::LinkedListToString(LinkedList *f_llSource, string f_sJoin)
{
	string l_sReturn = "", l_sCurrent = "";
	LinkedList *l_llCurrent = f_llSource;
	for (int f = 0; f < f_llSource->length(); f++)
	{
		l_sCurrent = l_llCurrent->get_data(l_sCurrent);
		if (l_sCurrent != "Undefined" && l_sCurrent != "Unfound")
			if (l_sReturn == "")
				l_sReturn = l_sCurrent;
			else
				l_sReturn += f_sJoin + l_sCurrent;
		l_llCurrent = l_llCurrent->get_ahead();
		if (l_llCurrent == nullptr)
			break;
	}
	return l_sReturn;
}

string Utility::LinkedListToString(LinkedList *f_llSource, char f_cJoin)
{
	string l_sReturn = "", l_sCurrent = "";
	LinkedList *l_llCurrent = f_llSource;
	for (int f = 0; f < f_llSource->length(); f++)
	{
		l_sCurrent = l_llCurrent->get_data(l_sCurrent);
		if (l_sCurrent != "Undefined" && l_sCurrent != "Unfound")
			if (l_sReturn == "")
				l_sReturn = l_sCurrent;
			else
				l_sReturn += f_cJoin + l_sCurrent;
		l_llCurrent = l_llCurrent->get_ahead();
		if (l_llCurrent == nullptr)
			break;
	}
	return l_sReturn;
}

string Utility::charToString(char f_cData)
{
	string l_sReturn = "";
	switch (f_cData)
	{
	case '1':return "1";
	case '2':return "2";
	case '3':return "3";
	case '4':return "4";
	case '5':return "5";
	case '6':return "6";
	case '7':return "7";
	case '8':return "8";
	case '9':return "9";
	case '0':return "0";
	case '=':return "=";
	case '-':return "-";
	case 'q':return "q";
	case 'w':return "w";
	case 'e':return "e";
	case 'r':return "r";
	case 't':return "t";
	case 'y':return "y";
	case 'u':return "u";
	case 'i':return "i";
	case 'o':return "o";
	case 'p':return "p";
	case '[':return "[";
	case ']':return "]";
	case 'a':return "a";
	case 's':return "s";
	case 'd':return "d";
	case 'f':return "f";
	case 'g':return "g";
	case 'h':return "h";
	case 'j':return "j";
	case 'k':return "k";
	case 'l':return "l";
	case ';':return ";";
	case '#':return "#";
	case 'z':return "z";
	case 'x':return "x";
	case 'c':return "c";
	case 'v':return "v";
	case 'b':return "b";
	case 'n':return "n";
	case 'm':return "m";
	case ',':return ",";
	case '.':return ".";
	case '/':return "/";
	case '\\':return "\\";
	case '|':return "|";
	case 'Q':return "Q";
	case 'W':return "W";
	case 'E':return "E";
	case 'R':return "R";
	case 'T':return "T";
	case 'Y':return "Y";
	case 'U':return "U";
	case 'I':return "I";
	case 'O':return "O";
	case 'P':return "P";
	case 'A':return "A";
	case 'S':return "S";
	case 'D':return "D";
	case 'F':return "F";
	case 'G':return "G";
	case 'H':return "H";
	case 'J':return "J";
	case 'K':return "K";
	case 'L':return "L";
	case 'Z':return "Z";
	case 'X':return "X";
	case 'C':return "C";
	case 'V':return "V";
	case 'B':return "B";
	case 'N':return "N";
	case 'M':return "M";
	case '<':return "<";
	case '>':return ">";
	case '?':return "?";
	case ':':return ":";
	case '@':return "@";
	case '~':return "~";
	case '{':return "{";
	case '}':return "}";
	case '+':return "+";
	case '£':return "£";
	case '$':return "$";
	case '%':return "%";
	case '^':return "^";
	case '&':return "&";
	case '*':return "*";
	case '(':return "(";
	case ')':return ")";
	case '!':return "!";
	default: return " ";
	}
	return l_sReturn;
}

int Utility::circleCollisionDistance(CircleCollision f_ccFirst, CircleCollision f_ccSecond)
{
	Vector2D l_v2dFirst = f_ccFirst.get_location();
	if (l_v2dFirst.m_iX == -1 || l_v2dFirst.m_iY == -1) return 9001;
	Vector2D l_v2dSecond = f_ccSecond.get_location();
	if (l_v2dSecond.m_iX == -1 || l_v2dSecond.m_iY == -1) return 9001;
	
	int l_ix1 = l_v2dFirst.m_iX;
	int l_ix2 = l_v2dSecond.m_iX;
	int l_iy1 = l_v2dFirst.m_iY;
	int l_iy2 = l_v2dSecond.m_iY;

	int l_iOtherPlayerDistance = sqrt(
		((l_ix2 - l_ix1) * (l_ix2 - l_ix1))
		+
		((l_iy2 - l_iy1) * (l_iy2 - l_iy1))
	);

	return l_iOtherPlayerDistance;
}

int Utility::circleCollisionDistance(CircleCollision *f_ccFirst, CircleCollision *f_ccSecond)
{
	if (f_ccFirst == nullptr) return 9001;
	if (f_ccSecond == nullptr) return 9001;
	Vector2D l_v2dFirst = f_ccFirst->get_location();
	if (l_v2dFirst.m_iX == -1 || l_v2dFirst.m_iY == -1) return 9001;
	Vector2D l_v2dSecond = f_ccSecond->get_location();
	if (l_v2dSecond.m_iX == -1 || l_v2dSecond.m_iY == -1) return 9001;

	int l_ix1 = l_v2dFirst.m_iX;
	int l_ix2 = l_v2dSecond.m_iX;
	int l_iy1 = l_v2dFirst.m_iY;
	int l_iy2 = l_v2dSecond.m_iY;

	int l_iOtherPlayerDistance = sqrt(
									((l_ix2 - l_ix1) * (l_ix2 - l_ix1))
									+
									((l_iy2 - l_iy1) * (l_iy2 - l_iy1))
									);
	return l_iOtherPlayerDistance;
}

bool Utility::circleCollisionCheck(CircleCollision *f_ccFirst, CircleCollision *f_ccSecond)
{
	if (f_ccFirst == nullptr) return false;
	if (f_ccSecond == nullptr) return false;
	Vector2D l_v2dFirst = f_ccFirst->get_location();
	if (l_v2dFirst.m_iX == -1 || l_v2dFirst.m_iY == -1) return false;
	Vector2D l_v2dSecond = f_ccSecond->get_location();
	if (l_v2dSecond.m_iX == -1 || l_v2dSecond.m_iY == -1) return false;
	int l_iOtherPlayerDistance = circleCollisionDistance(f_ccFirst, f_ccSecond);
	if (l_iOtherPlayerDistance < f_ccFirst->get_radius() + f_ccSecond->get_radius())
	{
		return true;
	}
	return false;
}

double Utility::calulate_angleBetweenTwoPoints(Vector2D f_v2dFirst, Vector2D f_v2dSecond, bool f_bRadians)
{
	
	int l_iUx = f_v2dFirst.m_iX;
	int l_iUy = f_v2dFirst.m_iY;
	int l_iVx = f_v2dSecond.m_iX;
	int l_iVy = f_v2dSecond.m_iY;
	//cout << "U: (" << l_iUx << ", " << l_iUy << ") V: (" << l_iVx << ", " << l_iVy << ")" << endl;

	int l_iUVDotProduct = (l_iUx * l_iVx) + (l_iUy * l_iVy);
	double l_fUmagnitude = sqrt((l_iUx * l_iUx) + (l_iUy * l_iUy) + 0.0);
	double l_fVmagnitude = sqrt((l_iVx * l_iVx) + (l_iVy * l_iVy) + 0.0);

	double l_fAngle = acos(
					l_iUVDotProduct /
					(l_fUmagnitude * l_fVmagnitude)
					);

	if(!f_bRadians)//Above is Radians so if false convert to degrees
		l_fAngle *= 180/ M_PI;

	//cout << "l_fAngle: " << l_fAngle << endl;
	//cout << "l_iUVDotProduct: " << l_iUVDotProduct << " l_fUmagnitude: " << l_fUmagnitude << " l_fVmagnitude: " << l_fVmagnitude << " (l_fUmagnitude * l_fVmagnitude):" << (l_fUmagnitude * l_fVmagnitude) << endl;
	return l_fAngle;
}

double Utility::calulate_gradientOfLineFromTwoPoints(Vector2D f_v2dFirst, Vector2D f_v2dSecond)
{
	int l_iUx = f_v2dFirst.m_iX;
	int l_iUy = f_v2dFirst.m_iY;
	int l_iVx = f_v2dSecond.m_iX;
	int l_iVy = f_v2dSecond.m_iY;

	if ((l_iUx - l_iVx) == 0) return 0;
	double l_dGradient = (l_iUy - l_iVy) / (l_iUx - l_iVx);

	return l_dGradient;
}

Vector2D Utility::calulate_vectorDirection(Vector2D f_v2dFirst, Vector2D f_v2dSecond)
{
	int l_iUx = f_v2dFirst.m_iX;
	int l_iUy = f_v2dFirst.m_iY;
	int l_iVx = f_v2dSecond.m_iX;
	int l_iVy = f_v2dSecond.m_iY;

	Vector2D l_v2dReturn = { (l_iUx - l_iVx), (l_iUy - l_iVy) };

	//cout << "X: " << l_v2dReturn.m_iX << ", Y: " << l_v2dReturn.m_iY << endl;
	return l_v2dReturn;
}