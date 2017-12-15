#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <string>
#include "Compiler.h"

class StudentWorld;


//Actor class
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* sw, int depth);

	virtual int doSomething();
	Direction randDirection();
	StudentWorld *getStudentWorld();

	virtual bool enemyStatus(int colonyNumber);
	virtual bool isAnthill(int colonyNumber);
	virtual bool isPheromone(int colonyNumber);
	virtual bool isDangerous(int colonyNumber);
	virtual bool isEdible();
	virtual bool canBlock();
	virtual bool isStunnable();
	virtual bool isLiving();

	virtual ~Actor();

private:
	StudentWorld* student_world;

};

//Pebble class
class Pebble : public Actor {
public:
	Pebble(int startX, int startY, StudentWorld* sw);
	virtual bool canBlock();
	virtual ~Pebble();

};
//EnergyHolder Class
class EnergyHolder : public Actor
{
public: 
	EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);

	virtual int isDead();

	virtual int doSomething();

	bool getLifeStatus() const;

	bool setLifeStatus(bool status);

	// Get this actor's amount of energy (for a Pheromone, same as strength).
	int getHP() const;

	// Adjust this actor's amount of energy upward or downward.
	void setHP(int amt);

	// Add an amount of food to this actor's location.
	void addFood(int amt);

	// Have this actor pick up an amount of food.
	int pickupFood(int amt);

	// Have this actor pick up an amount of food and eat it.
	int pickupAndEatFood(int amt);

	int getfoodInHand();

	void setfoodInHand(int amt);

	virtual bool isLiving();

	virtual ~EnergyHolder() {};
	// Does this actor become food when it dies?
	//virtual bool becomesFoodUponDeath() const;
private:
	int m_hitpoints; 
	bool m_isAlive;
	int m_foodInHand;
};

//Food class
class Food : public EnergyHolder {
public:
	Food(int startX, int startY, StudentWorld* sw);
	virtual int doSomething();
	virtual bool isEdible();
	virtual ~Food();
};

//Pheremone class
class Pheromone : public EnergyHolder {
public:
	Pheromone(int startX, int startY, int colonyNumber, StudentWorld* sw);
	int getColonyNumber();
	virtual bool isPheromone(int colonyNumber);
	virtual ~Pheromone();
private:
	int m_colonyNumber;
};

//Anthill class
class Anthill : public EnergyHolder {
public:
	Anthill(int startX, int startY, StudentWorld* sw, Compiler* c, int ColonyNumber);
	virtual int doSomething();
	virtual bool isAnthill(int colonyNumber);
	int getColonyNumber();
	virtual ~Anthill();
	//TODO: OTHER FUNCTIONS
private:
	Compiler* m_compiler;
	int m_colonyNumber;

};

//TriggerableActor class
class TriggerableActor : public Actor
{
public:
	TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
	virtual bool isDangerous(int colony) const;
	virtual ~TriggerableActor() {};
};

//Pool of Water Class
class PoolOfWater : public TriggerableActor {
public:
	PoolOfWater(int startX, int startY, StudentWorld* sw);
	virtual int doSomething();
	virtual ~PoolOfWater();

};

//Poison Class
class Poison : public TriggerableActor {
public:
	Poison(int startX, int startY, StudentWorld* sw);
	virtual int doSomething();
	virtual ~Poison();
};

class Insect : public EnergyHolder
{
public:
	Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
	virtual int doSomething();
	virtual bool enemyStatus(int colonyNumber);
	void updateCoordinates(int& newX, int& newY, Direction dir) const;
	int moveForward();
	void addSleepTicks(int value);
	void setSleepTicks(int value);
	virtual bool isStunnable();
	bool getStunnedStatus();
	void setStunnedStatus(bool status);
	void setBitten(bool val);
	bool getBittenStatus();
	bool getJustBlocked();
	void setJustBlocked(bool val);
	virtual bool biteBack();
	virtual ~Insect() {};


private:
	int getSleepTicks();
	void setTick(int tick);
	int getTick();
	bool alreadyStunned;
	bool justBitten;
	int currentTick;
	int sleep_ticks;
	bool justBlocked;
};

//Ant class
class Ant : public Insect {
public: 
	Ant(int startX, int startY, StudentWorld* sw, Compiler* c, int ColonyNumber);
	int Interpreter(Compiler* c);
	virtual int doSomething();
	int getColonyNumber();
	virtual bool enemyStatus(int colonyNumber);
	virtual ~Ant();

private:
	int m_commandCount;
	int m_lastRandomNumber;
	int m_colonyNumber;
	Compiler* m_compiler;
	int commandsProcessed; 
	//helper functions:
	bool conditionTriggered(Compiler::Command command);
	void incrementCounters(int &x, int &y);

};

class Grasshopper : public Insect
{
public:
	Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID, bool moult, bool jump, bool bite);
	virtual int doSomething();
	virtual ~Grasshopper() {};
private:
	int distance;
	bool can_moult;
	bool can_jump;
	bool can_bite;
};

//Adult Grasshopper class
class AdultGrasshopper : public Grasshopper {
public:
	AdultGrasshopper(int startX, int startY, StudentWorld* sw);
	virtual bool biteBack();
	virtual bool isStunnable(); 
	virtual ~AdultGrasshopper();

};
//BabyGrasshopper class
class BabyGrasshopper : public Grasshopper {
public:
	BabyGrasshopper(int startX, int startY, StudentWorld* sw);
	virtual ~BabyGrasshopper();
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
