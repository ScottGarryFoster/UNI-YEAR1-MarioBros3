#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
class FileMethods
{
public:
	FileMethods();
	virtual ~FileMethods();

	static bool open_fileForWritting(string f_sLocation, ofstream &f_outfile);

	static bool open_fileForReading(string f_sLocation, ifstream &f_outfile);

	static bool copy_file(string f_sOrigin, string f_sDestinaiton, bool f_bDebug = false);
};

