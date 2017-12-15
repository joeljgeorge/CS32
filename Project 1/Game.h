#ifndef GAME_H
#define GAME_H
#include <iostream> 
#include <cstdlib>
#include <random> 
#include <cctype>
#include <utility>
#include <string>
#include <string>
class Arena; 

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	std::string takePlayerTurn();
};

bool decodeDirection(char ch, int& dir);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);
#endif //for GAME_H
