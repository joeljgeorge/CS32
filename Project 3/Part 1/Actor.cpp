#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <string>
#include <iostream>
//ACTOR Definitions
Actor::Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* sw, int depth = 0):
GraphObject(imageID, startX, startY, dir, depth) 
{
	student_world = sw;
	alreadyMoved = false;
	switch (imageID) {
	case IID_ROCK: 
		setActorType("pebble");
		break;
	case IID_BABY_GRASSHOPPER:
		setActorType("baby_grasshopper");
		break;
	}
	currentTick = -1;
}

bool Actor::doSomething() {
	return false;
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

void Actor::setActorType(std::string type) {
	actorType = type; 
}

std::string Actor::getActorType() const{
	return actorType;
}

void Actor::updateCoordinates(int& newX, int& newY, Direction dir) {
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

bool Actor::getMoveStatus() {
	return alreadyMoved;
}

void Actor::setMoveStatus(bool status) {
	alreadyMoved = status;
	return;
}

int Actor::getTick() {
	return currentTick;
}

int Actor::setTick(int tick) {
	currentTick = tick;
	return currentTick; 
}

Actor::~Actor() {//TODO
	return;
}


//PEBBLE Definitions
Pebble::Pebble(int startX, int startY, StudentWorld* sw) :
	Actor(IID_ROCK, startX, startY, right, sw, 1) {
};

Pebble::~Pebble() {}//TODO


//BabyGrasshopper Definitions
BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld* sw):
	Actor(IID_BABY_GRASSHOPPER, startX, startY, randDirection(), sw){
	hit_points = 500;
	distance = randInt(2, 10);
	sleep_ticks = 2;
}

bool BabyGrasshopper::doSomething() {//OPTIMIZE
	if (getTick() == getStudentWorld()->getCurrentTick()) {
		return false;
	}
	setTick(getStudentWorld()->getCurrentTick());
	hit_points--;
	//TODO: turn baby grasshopper into food, set to dead, delete pointer
	if (sleep_ticks > 0) {
		sleep_ticks--;
		return false;
	}
	else if (distance == 0) {
		setDirection(randDirection());
		distance = randInt(2, 10);
	}
	int oldX = getX();
	int oldY = getY();
	int newX = oldX;
	int newY = oldY;
	bool moved = false;
	Direction dir = getDirection();
	updateCoordinates(newX, newY, dir);
	if (getStudentWorld()->isBlocked(newX, newY)) {
		distance = 0;
	}
	else {
		moved = getStudentWorld()->moveActor(oldX, oldY, newX, newY, this);
		distance--;
	}
	sleep_ticks = 2;
	return moved;
}

BabyGrasshopper::~BabyGrasshopper() {}



// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
