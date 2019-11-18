#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <math.h>
#include <string>
#include <iostream>
#include <string>

#include "LinkedList.h"
#include "CircleCollision.h"

using namespace::std;

class Utility
{
public:
	Utility();
	~Utility();

	//Debug
	static bool p_bDebug_SplitString;

	static char* StringToCharArray(string f_sData);

	static bool SplitStringAtPoint(string *f_sLeft, string *f_sRight, char f_cDelim);

	///Casts String to Int. Send a pointer to a bool to error check. -1 and false on no cast///
	static int convert_stringToInt(string f_sData, bool *f_bSuccess = nullptr);

	static int convert_charToInt(char f_cData);

	static char convert_intToChar(int f_iData);

	static bool GetRowAndColFromNumber(int f_iNumberOfRows, int f_iNumberOfCols, int f_iConvertNumber, int &f_iResultRows, int &f_iResultCols);

	static bool ReturnCollision(SDL_Rect l_rectOne, SDL_Rect l_rectTwo);

	static bool ReturnDrawForCulling(SDL_Rect *l_rectLocation, SDL_Rect *l_rectSource, float l_fCamX, float l_fCamScreenX, float l_fCamScreenWidth, float l_fScaleSize);

	static string StringToUpperCase(string f_sInput);

	static string AddLeadingZeros(int f_iInput, int f_iLength);

	static LinkedList* SplitStringToLL(string f_sText, char f_cDelim);

	static int SearchStringForChar(string f_sData, char f_cSearch);

	static string StringLeft(string f_sData, int f_iStartChar);

	static int RoundFloat(float f_fData, int f_iRound);

	static LinkedList* UniqueElements(LinkedList *f_llSource, string f_sData);

	static string LinkedListToString(LinkedList *f_llSource, string f_sJoin);

	static string LinkedListToString(LinkedList *f_llSource, char f_cJoin);

	static string charToString(char f_cData);

	static int circleCollisionDistance(CircleCollision f_ccFirst, CircleCollision f_ccSecond);
	static int circleCollisionDistance(CircleCollision *f_ccFirst, CircleCollision *f_ccSecond);
	static bool circleCollisionCheck(CircleCollision *f_ccFirst, CircleCollision *f_ccSecond);

	
	static double calulate_angleBetweenTwoPoints(Vector2D f_v2dFirst, Vector2D f_v2dSecond, bool f_bRadians = false);
	///Calulates the angle between two points. Set the last parameter to true if you want Degrees///

	static double calulate_gradientOfLineFromTwoPoints(Vector2D f_v2dFirst, Vector2D f_v2dSecond);

	static Vector2D calulate_vectorDirection(Vector2D f_v2dFirst, Vector2D f_v2dSecond);

	static double calulate_angelBetweenTwoPointsTan(Vector2D f_v2dFirst, Vector2D f_v2dSecond, bool f_bRadians = false);

	static char* stringToCharArray(string f_sData);
};