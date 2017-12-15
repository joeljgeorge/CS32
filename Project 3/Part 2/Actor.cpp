#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include "GraphObject.h"
#include <string>
#include <iostream>
#include <set>

using namespace std;

//////////////////////////////////////////////////////////////////////////
					//ACTOR Definitions//
//////////////////////////////////////////////////////////////////////////
Actor::Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* sw, int depth = 0):
GraphObject(imageID, startX, startY, dir, depth) 
{
	student_world = sw;
}

int Actor::doSomething() {
	return 1;
}

GraphObject::Direction Actor::randDirection() {
	switch (randInt(up, left)) {
	case up:
		return up;
	case right:
		return right;
	case down:
		return down;
	case left:
		return left;
	default:
		return none;
	}
}

StudentWorld* Actor::getStudentWorld() {
	return student_world;
}

/*void Actor::setActorType(std::string type) {
	actorType = type; 
}

std::string Actor::getActorType() const{
	return actorType;
}*/

/*int Actor::getHP() {
	return hitPoints;
}*/

/*int Actor::setHP(int value) {
	hitPoints = value;
	return hitPoints;
}*/

bool Actor::enemyStatus(int colonyNumber) {
	return false;
}

bool Actor::isAnthill(int colonyNumber) {
	return false;
}

bool Actor::isPheromone(int colonyNumber) {
	return false;
}

bool Actor::isDangerous(int colonyNumber) {
	return enemyStatus(colonyNumber); 
}

bool Actor::isEdible(){
	return false;
}

bool Actor::isStunnable() {
	return false;
}

bool Actor::canBlock() {
	return false;
} 

bool Actor::isLiving() {
	return false;
}

Actor::~Actor() {//TODO
	return;
}

//////////////////////////////////////////////////////////////////////////
					//EnergyHolder Definitions//
//////////////////////////////////////////////////////////////////////////
EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth) : Actor(imageID, startX, startY, startDir, sw, depth) {
	m_hitpoints = energy;
	m_isAlive = true;
	m_foodInHand = false;
}

bool EnergyHolder::setLifeStatus(bool value) {
	m_isAlive = value;
	return m_isAlive;
}

bool EnergyHolder::getLifeStatus() const {
	return m_isAlive;
}

int EnergyHolder::getHP() const {
	return m_hitpoints;
}

void EnergyHolder::setHP(int amt) {
	m_hitpoints = amt;
}

void EnergyHolder::addFood(int amt) {
	getStudentWorld()->makeFood(getX(), getY(), amt);
}

int EnergyHolder::pickupFood(int amt) {
	int foodCollected = getStudentWorld()->eat(getX(), getY(), amt); 
	m_foodInHand += foodCollected;  
	return foodCollected;
}

int EnergyHolder::pickupAndEatFood(int amt) {
	int foodPickedUp = pickupFood(amt);
	setHP(getHP() + foodPickedUp);
	m_foodInHand = m_foodInHand - foodPickedUp;
	return foodPickedUp;
}

int EnergyHolder::isDead() {
	if (getHP() <= 0) {
		setLifeStatus(false);
		return 2;
	}
	return 1;
}

int EnergyHolder::doSomething(){
	setHP(getHP() - 1);
	return isDead();
}

int EnergyHolder::getfoodInHand() {
	return m_foodInHand;
}

bool EnergyHolder::isLiving() {
	return true;
}

void EnergyHolder::setfoodInHand(int amt) {
	m_foodInHand = amt;
}

//////////////////////////////////////////////////////////////////////////
					//PEBBLE Definitions//
//////////////////////////////////////////////////////////////////////////
Pebble::Pebble(int startX, int startY, StudentWorld* sw) :
	Actor(IID_ROCK, startX, startY, right, sw, 1) {
}

bool Pebble::canBlock() {
	return true;
}

Pebble::~Pebble() {}//TODO

//////////////////////////////////////////////////////////////////////////
					//FOOD Definitions//
//////////////////////////////////////////////////////////////////////////

Food::Food(int startX, int startY, StudentWorld* sw) : EnergyHolder(sw, startX, startY, right, 6000, IID_FOOD, 2) {}

int Food::doSomething() {
	return Actor::doSomething();
}

bool Food::isEdible(){
	return true;
}

Food::~Food() {}//TODO

//////////////////////////////////////////////////////////////////////////
					//Anthill Definitions//
//////////////////////////////////////////////////////////////////////////
Anthill::Anthill(int startX, int startY, StudentWorld* sw, Compiler* c, int ColonyNumber) : EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2) {  
	m_compiler = c;
	m_colonyNumber = ColonyNumber;
}

int Anthill::doSomething() {
	if (m_compiler == nullptr) 
		return 1;
	if (EnergyHolder::doSomething() == 2)
		return 2;
	int hp_bonus = pickupAndEatFood(10000); 
	if (hp_bonus > 0) {
		//setHP(getHP() + hp_bonus);
		return 1;
	}
	if (getHP() > 2000)
	{
		getStudentWorld()->createNewAnt(getX(), getY(), m_compiler, m_colonyNumber);
		setHP(getHP() - 1500);
		getStudentWorld()->increaseScore(getColonyNumber());
	}
	return 1;
}

bool Anthill::isAnthill(int colonyNumber) {
	if (colonyNumber == getColonyNumber())
		return true;
	return false;
}

int Anthill::getColonyNumber() {
	return m_colonyNumber;
}

Anthill::~Anthill(){}

//////////////////////////////////////////////////////////////////////////
					//Pheromone Definitions//
//////////////////////////////////////////////////////////////////////////

Pheromone::Pheromone(int startX, int startY, int colonyNumber, StudentWorld* sw) : EnergyHolder(sw, startX, startY, right, 256, colonyNumber+11, 2) {
	m_colonyNumber = colonyNumber;
}

int Pheromone::getColonyNumber() {
	return m_colonyNumber;
}

bool Pheromone::isPheromone(int colonyNumber) {
	if (getColonyNumber() == colonyNumber)
		return true;
	return false;
}

Pheromone::~Pheromone() {}
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//////////////////////////////////////////////////////////////////////////
					//Triggerable_Actor Definitions//
//////////////////////////////////////////////////////////////////////////

TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID) : Actor(imageID, x, y, right, sw, 2){}

bool TriggerableActor::isDangerous(int colony) const {
	return true;
}

//////////////////////////////////////////////////////////////////////////
						//Poison Definitions//
//////////////////////////////////////////////////////////////////////////
Poison::Poison(int startX, int startY, StudentWorld* sw) : TriggerableActor(sw, startX, startY, IID_POISON) {}

int Poison::doSomething() {
	getStudentWorld()->poison(getX(), getY());
	return 1;
}

Poison::~Poison() {}

//////////////////////////////////////////////////////////////////////////
					//POOLofWATER Definitions//
//////////////////////////////////////////////////////////////////////////
PoolOfWater::PoolOfWater(int startX, int startY, StudentWorld* sw) :
	TriggerableActor(sw, startX, startY, IID_WATER_POOL) {}

int PoolOfWater::doSomething() {
	getStudentWorld()->stun(getX(), getY());
	return 1;
}


PoolOfWater::~PoolOfWater(){}

//////////////////////////////////////////////////////////////////////////
					//Insect Definitions//
//////////////////////////////////////////////////////////////////////////
Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID) : EnergyHolder(world, startX, startY, right, energy, imageID, 0) {
	setDirection(randDirection());
	setHP(energy);
	alreadyStunned = false;
	justBitten = false;
	justBlocked = false;
	currentTick = -1;
	sleep_ticks = 0;
}

int Insect::doSomething() {
	if (getTick() == getStudentWorld()->getCurrentTick()) {
		return 1;
	}
	setTick(getStudentWorld()->getCurrentTick());
	if (EnergyHolder::doSomething() == 2) {
		EnergyHolder::addFood(100);
		return 2;
	}
	int sleepTicks = getSleepTicks();
	if (sleepTicks > 0) {
		setSleepTicks(sleepTicks - 1);
		return 1;
	}
	return 3;
}

bool Insect::enemyStatus(int colonyNumber) {
	return true;
}

void Insect::updateCoordinates(int& newX, int& newY, Direction dir) const {
	switch (dir) {
	case up:
		newY++;
		break;
	case right:
		newX++;
		break;
	case down:
		newY--;
		break;
	case left:
		newX--;
		break;
	default:
		return;
	}
}

int Insect::moveForward() {
	int oldX = getX();
	int oldY = getY();
	int newX = oldX;
	int newY = oldY;
	bool moved = false;
	Direction dir = getDirection();
	updateCoordinates(newX, newY, dir);
	if (getStudentWorld()->isBlocked(newX, newY)) {
		justBlocked = true;
		return 1;
	}
	else {
		moved = getStudentWorld()->moveActor(oldX, oldY, newX, newY, this);
		setStunnedStatus(false);
		setBitten(false);
		justBlocked = false;
	}
	if (moved)
		return 0;
	return 1;
}

int Insect::getSleepTicks() {
	return sleep_ticks;
}

void Insect::addSleepTicks(int value) {
	sleep_ticks += value;
}

void Insect::setSleepTicks(int value) {
	sleep_ticks = value;
}

bool Insect::isStunnable() {
	return true;
}

bool Insect::getStunnedStatus() {
	return alreadyStunned;
}

void Insect::setStunnedStatus(bool status) {
	alreadyStunned = status;
}

int Insect::getTick() {
	return currentTick;
}

void Insect::setTick(int tick) {
	currentTick = tick;
}

void Insect::setBitten(bool val) {
	justBitten = val;
}
bool Insect::getBittenStatus() {
	return justBitten;
}

bool Insect::biteBack() {
	return false;
}

bool Insect::getJustBlocked() {
	return justBlocked;
}

void Insect::setJustBlocked(bool val) {
	justBlocked = val;
}


//////////////////////////////////////////////////////////////////////////
					//Ant Definitions//
//////////////////////////////////////////////////////////////////////////
Ant::Ant(int startX, int startY, StudentWorld* sw, Compiler* c, int ColonyNumber) : Insect(sw, startX, startY, 1500, ColonyNumber) { 
	m_lastRandomNumber = 0;
	m_commandCount = 0;
	m_compiler = c;
	m_colonyNumber = ColonyNumber; 
	commandsProcessed = 0;
}

int Ant::doSomething() {
	int doSomethingVal = Insect::doSomething();
	if (doSomethingVal == 2)
		return 2;
	else if (doSomethingVal == 1)
		return 1;
	int return_val = Interpreter(m_compiler);
	if (return_val == 5) {
		setLifeStatus(false);
		return 2;
	}
	else
		return return_val;
	return 1;
}


int Ant::getColonyNumber() {
	return m_colonyNumber;
}

bool Ant::enemyStatus(int colonyNumber) {
	if (colonyNumber != getColonyNumber())
		return true;
	return false;
}


bool Ant::conditionTriggered(Compiler::Command command) {
	switch (stoi(command.operand1)) { 
	case Compiler::Condition::last_random_number_was_zero: {
		if (m_lastRandomNumber == 0)
			return true;
		return false;
	}
	case Compiler::Condition::i_am_carrying_food: {
		if (getfoodInHand() > 0)
			return true;
		return false;
	}
	case Compiler::Condition::i_am_hungry: {
		if (getHP() <= 25)
			return true;
		return false;
	}
	case Compiler::Condition::i_am_standing_with_an_enemy: {
		if (getStudentWorld()->checkForEnemy(getX(), getY(), getColonyNumber()))
			return true;
		return false;
	}
	case Compiler::Condition::i_am_standing_on_food: {
		if (getStudentWorld()->checkFood(getX(), getY())) 
			return true;
		return false;
	}
	case Compiler::Condition::i_am_standing_on_my_anthill: {
		if (getStudentWorld()->checkForMyAnthill(getX(), getY(), getColonyNumber()))
			return true;
		return false;
	}
	case Compiler::Condition::i_smell_pheromone_in_front_of_me: {
		int x = getX();
		int y = getY();
		int newX = x;
		int newY = y;
		updateCoordinates(newX, newY, getDirection());
		if (getStudentWorld()->checkForPheromone(newX, newY, getColonyNumber()))//TODO: check if this is just ant's own pheromone?
			return true;
		return false;
	}
	case Compiler::Condition::i_smell_danger_in_front_of_me: {
		int x = getX();
		int y = getY();
		int newX = x;
		int newY = y;
		updateCoordinates(newX, newY, getDirection());
		if (getStudentWorld()->checkForDanger(newX, newY, getColonyNumber()))
			return true;
		return false;
	}
	case Compiler::Condition::i_was_bit: {
		if (getBittenStatus())
			return true;
		return false;
	}
	case Compiler::Condition::i_was_blocked_from_moving: {
		if (getJustBlocked())
			return true;
		return false;
	}
	}
}


int Ant::Interpreter(Compiler* c) {
	Compiler::Command cmd;
	commandsProcessed = 0;
	for (;;) // keep running forever for now
	{
		if (commandsProcessed >= 10) {
			return 1;
		}
		// get the command from element ic of the vector
		if (!c->getCommand(m_commandCount, cmd)) {
			return 5;
		}
			 // error - no such instruction!
		switch (cmd.opcode) 
		{
		case Compiler::Opcode::moveForward: 
			incrementCounters(m_commandCount, commandsProcessed);
			 return moveForward();
		case Compiler::Opcode::generateRandomNumber:
			if (stoi(cmd.operand1) == 0)
				m_lastRandomNumber = 0;
			else
				m_lastRandomNumber = randInt(0, stoi(cmd.operand1) - 1);
			incrementCounters(m_commandCount, commandsProcessed);
			break;
		case Compiler::Opcode::if_command:
			if (conditionTriggered(cmd)) {
				m_commandCount = stoi(cmd.operand2);
			}
			else
				incrementCounters(m_commandCount, commandsProcessed);
			break;
		case Compiler::Opcode::goto_command:
			// just set ic the specified position
			// in operand1
			m_commandCount = stoi(cmd.operand1); 
			break;
		case Compiler::Opcode::eatFood: {
			if (getfoodInHand() > 0) {
				if (getfoodInHand() < 100) {
					setHP(getHP() + getfoodInHand());
					setfoodInHand(0);
				}
				else {
					setHP(getHP() + 100);
					setfoodInHand(getfoodInHand()-100);
				}
			}
			incrementCounters(m_commandCount, commandsProcessed);
			return 1;
		}
		case Compiler::Opcode::dropFood: {
			if (getfoodInHand() == 0) {
				commandsProcessed++;
				++m_commandCount;
				return 1;
			}
			getStudentWorld()->makeFood(getX(), getY(), getfoodInHand());
			setfoodInHand(0);
			incrementCounters(m_commandCount, commandsProcessed);
			return 1;
		}
		case Compiler::Opcode::bite: {
			getStudentWorld()->bite(getX(), getY(), this, getColonyNumber(), 15);
			incrementCounters(m_commandCount, commandsProcessed);
			return 1;
		}
		case Compiler::Opcode::pickupFood: {
			int possibleAmt = 1800 - getfoodInHand();
			if (possibleAmt >= 400) {
				int foodCollected = getStudentWorld()->eat(getX(), getY(), 400);
				setfoodInHand(getfoodInHand() + foodCollected);
				incrementCounters(m_commandCount, commandsProcessed);
				return 1;
			}
			else {
				int foodCollected = getStudentWorld()->eat(getX(), getY(), possibleAmt);
				setfoodInHand(getfoodInHand() + foodCollected);
				incrementCounters(m_commandCount, commandsProcessed);
				return 1;
			}
		}
		case Compiler::Opcode::emitPheromone: {
			getStudentWorld()->makePheromone(getX(), getY(), getColonyNumber());
			incrementCounters(m_commandCount, commandsProcessed);
			return 1;
		}
		case Compiler::Opcode::faceRandomDirection: {
			setDirection(randDirection());
			incrementCounters(m_commandCount, commandsProcessed);
			return 1;
		}
		case Compiler::Opcode::rotateClockwise: {
			Direction dir = getDirection();
			switch (dir) {
			case up:
				setDirection(right);
				break;
			case down:
				setDirection(left);
				break;
			case left:
				setDirection(up);
				break;
			case right:
				setDirection(down);
				break;
			default:
				break;
			}
			incrementCounters(m_commandCount, commandsProcessed);
			return 1;
		}
		case Compiler::Opcode::rotateCounterClockwise: {
			Direction dir = getDirection();
			switch (dir) {
			case up:
				setDirection(left);
				break;
			case down:
				setDirection(right);
				break;
			case left:
				setDirection(down);
				break;
			case right:
				setDirection(up);
				break;
			default:
				break;
			}
			incrementCounters(m_commandCount, commandsProcessed);
			return 1;
		}
		}
	}
}

void Ant::incrementCounters(int &x, int &y) {
	x++;
	y++;
}

Ant::~Ant(){}

//////////////////////////////////////////////////////////////////////////
					//Grasshopper Definitions//
//////////////////////////////////////////////////////////////////////////
Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID, bool moult, bool bite, bool jump) : Insect(sw, startX, startY, energy, imageID) {
	distance = randInt(2, 10);
	can_moult = moult;
	can_bite = bite;
	can_jump = jump;
}


int Grasshopper::doSomething() {
	int doSomethingVal = Insect::doSomething();
	if (doSomethingVal == 2)
		return 2;
	else if (doSomethingVal == 1)
		return 1;
	if (can_moult) {
		if (getHP() >= 1600) {
			addFood(100);
			setLifeStatus(false);
			return 3;
		}	
	}
	if (can_bite) {
		if (randInt(0, 2) == 0 && getStudentWorld()->bite(getX(), getY(), this, -5, 50)) {
			setSleepTicks(2);
			return 1;
		}
	}
	if (can_jump) {
		if (randInt(0, 9) == 0) {
			bool moved;
			if (getStudentWorld()->checkForSpaces(getX(), getY())) {
				moved = getStudentWorld()->jump(getX(), getY(), this);
				setSleepTicks(2);
				if (moved) {
					return 0;
				}
				return 1;
			}
		}
	}

	int hp_bonus = pickupAndEatFood(200);
	if (hp_bonus > 0) {
		//setHP(getHP() + hp_bonus);
		if (randInt(0, 1) == 0) {
			setSleepTicks(2);
			return 1;
		}
	}
	if (distance == 0) {
		setDirection(randDirection());
		distance = randInt(2, 10);
	}
	int return_val = moveForward();
	setSleepTicks(2);
	if (getJustBlocked()) {
		distance = 0;
	}
	else
		distance--;
	return return_val;
}

//////////////////////////////////////////////////////////////////////////
					//BABY_GRASSHOPPER Definitions//
//////////////////////////////////////////////////////////////////////////
BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld* sw): Grasshopper(sw, startX, startY, 500, IID_BABY_GRASSHOPPER, true, false, false){}

BabyGrasshopper::~BabyGrasshopper() {}

//////////////////////////////////////////////////////////////////////////
					//Adult Grasshopper Definitions//
//////////////////////////////////////////////////////////////////////////
AdultGrasshopper::AdultGrasshopper(int startX, int startY, StudentWorld* sw) : Grasshopper(sw, startX, startY, 1600, IID_ADULT_GRASSHOPPER, false, true, true) {}


bool AdultGrasshopper::biteBack() {
	return true;
}

bool AdultGrasshopper::isStunnable() {
	return false;
}

AdultGrasshopper::~AdultGrasshopper() {}



