#ifndef HISTORY_H
#define HISTORY_H

#include <iostream> 
#include <cstdlib>
#include <random> 
#include <cctype>
#include <utility>
#include <string>
#include "globals.h"
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
	//TODO: ADD DESTRUCTOR
private:
	int m_grid[MAXROWS][MAXCOLS];
	int m_rows;
	int m_cols;

};

#endif //HISTORY_H

