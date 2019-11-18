#include "FileMethods.h"



FileMethods::FileMethods()
{
}


FileMethods::~FileMethods()
{
}

bool FileMethods::open_fileForWritting(string f_sLocation, ofstream &f_outfile)
{
	f_outfile.open(f_sLocation);
	if (!f_outfile.is_open())
	{
		ifstream l_infileCopy(f_sLocation);

		if (l_infileCopy.is_open())// Check if the file was opened.
		{
			l_infileCopy.close();
		}
		f_outfile.open(f_sLocation);
		if (f_outfile.is_open())
		{
			cout << "FileMethods::open_fileForWritting: File could not be openned: " << f_sLocation << endl;
			return false;
		}
		return true;
	}
	return true;
}

bool FileMethods::open_fileForReading(string f_sLocation, ifstream &f_infile)
{
	f_infile.open(f_sLocation);
	if (!f_infile.is_open())
	{
		cout << "FileMethods::open_fileForReading: File could not be openned: " << f_sLocation << endl;
		return false;
	}
	return true;
}

bool FileMethods::copy_file(string f_sOrigin, string f_sDestinaiton, bool f_bDebug)
{
	bool l_bCheck = false;
	ifstream l_infileOrigin;
	l_bCheck = open_fileForReading(f_sOrigin, l_infileOrigin);
	if (!l_bCheck) { cout << "FileMethods::copy_file: open_fileForReading failed." << endl; return false; }
	ofstream l_outfileDestination;
	l_bCheck = open_fileForWritting(f_sDestinaiton, l_outfileDestination);
	if (!l_bCheck) { cout << "FileMethods::copy_file: open_fileForWritting failed." << endl; return false; }
	string l_sGetLine = "";
	while (getline(l_infileOrigin, l_sGetLine))
	{
		l_outfileDestination << l_sGetLine + "\n";
	}
	l_outfileDestination.close();
	l_infileOrigin.close();
	return true;
}