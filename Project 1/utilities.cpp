#include <iostream>
#include "globals.h"
#include "Arena.h"
#include <iostream> 
#include <cstdlib>
#include <random> 
#include <cctype>
#include <utility>
#include <string>

using namespace std; 
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	int rnew = r;
	int cnew = c;
	switch (dir)
	{
	case NORTH:  if (r <= 1)        return false; else rnew--; break;
	case EAST:   if (c >= a.cols()) return false; else cnew++; break;
	case SOUTH:  if (r >= a.rows()) return false; else rnew++; break;
	case WEST:   if (c <= 1)        return false; else cnew--; break;
	}
	r = rnew;
	c = cnew;
	return true;
}

int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif


