#include "StudentWorld.h"
#include <string>
#include "Field.h" 
#include "GameWorld.h"
#include <sstream>
#include <list>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	//initialize data structure used to keep track of your simulation's virtual world
	//-already initialized in class declaration
	Field f;
	if (loadFile(f)) {
		for (int i = 0; i < VIEW_WIDTH; i++) {

			for (int j = 0; j < VIEW_HEIGHT; j++) {

				Field::FieldItem item = f.getContentsOf(i, j);
				Actor* actor = buildItem(item, i, j);
				if (actor != nullptr) {
					createNewEntries(i, j, actor);
				}
			}
		}
		return GWSTATUS_CONTINUE_GAME;
	}
	else {
		return GWSTATUS_LEVEL_ERROR;
	}
		
}

int StudentWorld::move() {//TODO: Iterate through map

	elapsedTicks++;
	if (elapsedTicks == 2000)
		return GWSTATUS_NO_WINNER;
	 
	map<string, map<Actor*, string>*>::iterator it = virtualField.begin();
	map<string, map<Actor*, string>*>::iterator temp_it;
	map<Actor*, string>::iterator it2;
	map<Actor*, string>::iterator temp_it2;

	while( it != virtualField.end()){
		it2 = (*(it->second)).begin();
		temp_it = next(it);
		while (it2 != (*(it->second)).end()) {
			temp_it2 = next(it2);
			if (it2->second == "pebble")
				break;
			if ((it2->first)->doSomething()) {
				break;
			}
			else {
				it2 = temp_it2;
				if (it2 == (*(it->second)).end())
					break;
				temp_it2 = next(it2);
			}
		}
		it = temp_it;
		if (it == virtualField.end())
			break;
		temp_it = next(it);
	}
	setDisplayText();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){//TODO

	map<string, map<Actor*, string>*>::iterator it = virtualField.begin();
	map<Actor*, string>::iterator it2;

	for (; it != virtualField.end(); it++) {
		if (it->second != nullptr) {
			for (it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
				delete it2->first;
			}
			delete it->second;
			it->second = nullptr;
		}
	}
	return;
}

bool StudentWorld::moveActor(int oldX, int oldY, int newX, int newY, Actor* actor) {//TODO
	string oldLocation = convertToString(oldX, oldY);
	string newLocation = convertToString(newX, newY);
	bool deleted = false;
	//finding oldLocation block
	map<string, map<Actor*, string>*>::iterator it = virtualField.find(oldLocation);
	
	//Once oldLocation is found, find the pointer to the current actor in the subclass
	map<Actor*, string>::iterator it2 = (*(it->second)).find(actor);

	//once found, erase that entry
	if ((*(it->second)).size() == 1) {//if that was the only actor at that position, delete that entry from the map;
		(*(it->second)).clear();
		delete it->second;
		virtualField.erase(it);
		deleted = true;
	}
	else {
		(*(it->second)).erase(it2);
	}
		

	//update actor's X and Y coordinates
	actor->moveTo(newX, newY);
	//traverse through first column of map; if newLocation is already an entry, add the actor and it's type to the submap
	it = virtualField.find(newLocation);
	//it newLocation is not an entry in the map, add it as a new entry
	if (it == virtualField.end()) {
		createNewEntries(newX, newY, actor);
	}
	else {
		(*(it->second))[actor] = actor->getActorType();
	}
	
	return deleted;
}

bool StudentWorld::isBlocked(int newX, int newY) {//TODO
	if (newX <= 0 || newX >= VIEW_WIDTH - 1 || newY <= 0 || newY >= VIEW_HEIGHT - 1)
		return true;
	string newLocation = convertToString(newX, newY);
	map<string, map<Actor*, string>*>::iterator it = virtualField.find(newLocation);
	if (it == virtualField.end())
		return false;
	for (map<Actor*, string>::iterator it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
		if (it2->second == "pebble")
			return true;
	}
	return false;
}

string StudentWorld::convertToString(int x, int y) {
	string x_coord;
	string y_coord;
	if (x < 10) {
		x_coord += '0';
		x_coord += x+'0';
	}
	else {
		x_coord += (x / 10 + '0');
		x_coord += (x % 10 + '0');
	}
	if (y < 10) {
		y_coord += '0';
		y_coord += y + '0';
	}
	else {
		y_coord += (y / 10 + '0');
		y_coord += (y % 10 + '0');
	}
	ostringstream oss;
	oss << x_coord << y_coord;
	return oss.str();
}

string StudentWorld::convertToString(int x) {
	string number = "";
	int digit = 0;
	char digit_holder;
	for (int i = x; i > 0; ) {
		digit = i%10;
		digit_holder = digit + '0';
		number = digit_holder + number;
		i = i / 10;
	}
	return number;
}

bool StudentWorld::loadFile(Field &f) {
	string fieldFile = getFieldFilename();
	string error;
	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return false; // error in data file
	}
	return true;
}


Actor* StudentWorld::buildItem(Field::FieldItem item, int startX, int startY) {//add other items
	switch (item) {
		case Field::FieldItem::rock: {
			Actor *rock;
			rock = new Pebble(startX, startY, this); 
			return rock;
		} 
		case Field::FieldItem::grasshopper: {
			Actor *grasshopper;
			grasshopper = new BabyGrasshopper(startX, startY, this);
			return grasshopper;
		} 
		default:
			return nullptr;
	}
}

void StudentWorld::createNewEntries(int x, int y, Actor* actor) {
	string location = convertToString(x, y);
	map<Actor*, std::string>* mActor;
	mActor = new map<Actor*, std::string>;
	(*mActor)[actor] = actor->getActorType();
	virtualField[location] = mActor;  
	return;
}

int StudentWorld::getCurrentTick() {
	return elapsedTicks;
}

void StudentWorld::setDisplayText() {
	string ticks = convertToString(2000-elapsedTicks);
	string gameText = "Ticks: " + ticks;//TODO: implement game text properly, as per the specs (justification, character count, etc.)
	setGameStatText(gameText);
}
StudentWorld::~StudentWorld(){//TODO
	cleanUp();
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
