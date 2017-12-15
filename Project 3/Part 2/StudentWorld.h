#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <map>
#include <string>
#include <list>
#include <set>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), elapsedTicks(0)
	{
		elapsedTicks = 0;
	}
	//THE "BIG THREE"
	virtual int init();

	virtual int move();

	virtual void cleanUp();

	//Supplimentary functions
	bool moveActor(int oldX, int oldY, int newX, int newY, Actor* actor);

	bool isBlocked(int newX, int newY);

	int getCurrentTick();

	bool checkFood(int currentX, int currentY);

	int eat(int currentX, int currentY, int value);

	void stun(int currentX, int currentY);

	void poison(int currentX, int currentY);

	bool bite(int currentX, int currentY, Actor* actor, int colonyNumber, int dmg);

	bool checkForSpaces(int currentX, int currentY);

	bool jump(int currentX, int currentY, Actor* actor);

	bool checkForEnemy(int currentX, int currentY, int colonyNumber);

	bool checkForMyAnthill(int currentX, int currentY, int colonyNumber);

	bool checkForPheromone(int currentX, int currentY, int colonyNumber);

	bool checkForDanger(int currentX, int currentY, int colonyNumber);

	void makeFood(int currentX, int currentY, int value);

	void makePheromone(int currentX, int currentY, int colonyNumber);

	void createNewAnt(int currentX, int currentY, Compiler* c, int colonyNumber); 

	void increaseScore(int colNumber);

	void decreaseScore(int colNumber);

	virtual ~StudentWorld();


private:
	std::map<std::string, std::list<Actor*>*> virtualField;
	std::vector<Compiler*> compilerPointers;
	int elapsedTicks;
	int numOfAnt0 = 0;
	int numOfAnt1 = 0;
	int numOfAnt2 = 0;
	int numOfAnt3 = 0;
	int winningAnt = 0; 
	int winningNumber = 5;

	//helper functions:

	std::string convertToString(int x, int y);

	std::string convertToString(int x);

	void decodeString(int &x, int &y, std::string location);

	void createNewEntries(int x, int y, Actor* actor);

	bool loadFile(Field &f);

	Actor* buildItem(Field::FieldItem item, int startX, int startY);

	double paramEqX(int radius, double angle);

	double paramEqY(int radius, double angle);

	void setDisplayText();

	void removeDeadActors();

	std::list<Actor*>::iterator checkFood(std::list<Actor*> &subMap);
};

#endif // STUDENTWORLD_H_
