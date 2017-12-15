/*#include <iostream>
#include <string>

using namespace std;*/

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {

	if (sr == er && sc == ec) {
		return true;
	}
	maze[sr][sc] = '#';
	bool solveable = false;
	if (sr - 1 >= 0 && maze[sr - 1][sc] == '.') {//NORTH
		solveable = pathExists(maze, nRows, nCols, sr - 1, sc, er, ec);
		if (solveable)
			return true;
	}
	if (sc + 1 < nCols && maze[sr][sc + 1] == '.') {//EAST
		solveable = pathExists(maze, nRows, nCols, sr, sc + 1, er, ec);
		if (solveable)
			return true;
	}
	if (sr + 1 < nRows && maze[sr + 1][sc] == '.') {//SOUTH
		solveable = pathExists(maze, nRows, nCols, sr + 1, sc, er, ec);
		if (solveable)
			return true;
	}
	if (sc - 1 >= 0 && maze[sr][sc - 1] == '.') {//WEST
		solveable = pathExists(maze, nRows, nCols, sr, sc - 1, er, ec);
		if (solveable)
			return true;
	}
	return false;
}