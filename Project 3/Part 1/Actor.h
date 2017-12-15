#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <string>
class StudentWorld;
//Actor class
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* sw, int depth);
	virtual bool doSomething();
	Direction randDirection();
	StudentWorld *getStudentWorld();
	void setActorType(std::string type); 
	std::string getActorType() const;
	void updateCoordinates(int& newX, int& newY, Direction dir);
	bool getMoveStatus();
	void setMoveStatus(bool status);
	int getTick();
	int setTick(int tick);
	virtual ~Actor();

private:
	StudentWorld* student_world;
	std::string actorType; 
	bool alreadyMoved;
	int currentTick;
};

//Pebble class
class Pebble : public Actor {
public:
	Pebble(int startX, int startY, StudentWorld* sw);
	virtual ~Pebble();

};
//BabyGrasshopper class
class BabyGrasshopper : public Actor {
public:
	BabyGrasshopper(int startX, int startY, StudentWorld* sw);
	virtual bool doSomething();
	virtual ~BabyGrasshopper();
private:
	int hit_points;
	int distance;
	int sleep_ticks;
};


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
