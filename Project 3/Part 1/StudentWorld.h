#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <map>
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), elapsedTicks(0)
	{}

	virtual int init();

	virtual int move();

	virtual void cleanUp();
	
	bool loadFile(Field &f);

	Actor* buildItem(Field::FieldItem item, int startX, int startY);

	bool moveActor(int oldX, int oldY, int newX, int newY, Actor* actor);

	bool isBlocked(int newX, int newY);

	std::string convertToString(int x, int y); 

	std::string convertToString(int x);

	void createNewEntries(int x, int y, Actor* actor);

	int getCurrentTick();

	void setDisplayText();

	~StudentWorld();


private:
	std::map<std::string, std::map<Actor*, std::string>*> virtualField;
	int elapsedTicks;


};

#endif // STUDENTWORLD_H_
