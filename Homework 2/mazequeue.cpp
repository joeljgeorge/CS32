#include <queue>
#include <iostream>
#include <string>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	queue<Coord> cqueue;
	Coord a(sr, sc);
	cqueue.push(a);
	maze[a.r()][a.c()] = '0';
	while (!(cqueue.empty())) {
		Coord b = cqueue.front();
		cqueue.pop();
		if (b.r() == er && b.c() == ec) {
			return true;
		}
		if ((b.r() - 1) >= 0 && maze[b.r() - 1][b.c()] != 'X' && maze[b.r() - 1][b.c()] != '0') {
			Coord c(b.r() - 1, b.c());
			cqueue.push(c);
			maze[b.r() - 1][b.c()] = '0';
		}
		if ((b.c() + 1) < nCols && maze[b.r()][b.c() + 1] != 'X' && maze[b.r()][b.c() + 1] != '0') {
			Coord c(b.r(), b.c() + 1);
			cqueue.push(c);
			maze[b.r()][b.c() + 1] = '0';
		}
		if ((b.r() + 1) < nRows && maze[b.r() + 1][b.c()] != 'X' && maze[b.r() + 1][b.c()] != '0') {
			Coord c(b.r() + 1, b.c());
			cqueue.push(c);
			maze[b.r() + 1][b.c()] = '0';
		}
		if ((b.c() - 1) >= 0 && maze[b.r()][b.c() - 1] != 'X' && maze[b.r()][b.c() - 1] != '0') {
			Coord c(b.r(), b.c() - 1);
			cqueue.push(c);
			maze[b.r()][b.c() - 1] = '0';
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
	for (int i = 0; i < 10; i++) {
		cout << maze[i] << endl;
	}
}


