#include "GameSelect.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

//Debug for Utility Functions:
#include "Utility.h"
bool Utility::p_bDebug_SplitString = false;

using namespace std;

int main(int argc, char* args[])
{
	srand(time(NULL));
	GameSelect* mbGame = new GameSelect(argc, args);
	return 0;
}