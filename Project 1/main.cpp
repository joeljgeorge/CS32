#include <iostream> 
#include <cstdlib>
#include <random> 
#include <cctype>
#include <utility>
#include <string>
#include "Game.h"


int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(3, 5, 2);

	// Play the game
	g.play();
}



