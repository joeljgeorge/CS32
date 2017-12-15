#include "StudentWorld.h"
#include <string>
#include "Field.h" 
#include "GameWorld.h"
#include "Compiler.h"
#include <sstream>
#include <list>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

//TODO: Replace all instances where you check string of subclass with a function in the base class that returns true or false for that value

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

//////////////////////////////////////////////////////////////////////////
							//INIT FUNCTION//
//////////////////////////////////////////////////////////////////////////
int StudentWorld::init() {
	//initialize data structure used to keep track of your simulation's virtual world
	//-already initialized in class declaration
	Compiler *compilerForEntrant0 = nullptr;
	Compiler *compilerForEntrant1 = nullptr;
	Compiler *compilerForEntrant2 = nullptr;
	Compiler *compilerForEntrant3 = nullptr;

	compilerPointers.push_back(compilerForEntrant0); //TODO: Turn all of this compiler stuff into function (buildCompilers('4 Compiler* parameters))
	compilerPointers.push_back(compilerForEntrant1);
	compilerPointers.push_back(compilerForEntrant2);
	compilerPointers.push_back(compilerForEntrant3);

	vector<string> fileNames = getFilenamesOfAntPrograms();

	for (int i = 0; i < fileNames.size(); i++) {
		compilerPointers[i] = new Compiler;
		string error;
		if (!compilerPointers[i]->compile(fileNames[i], error))
		{
			setError(fileNames[i] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}

	}
	

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

//////////////////////////////////////////////////////////////////////////
						//MOVE FUNCTION//
//////////////////////////////////////////////////////////////////////////
int StudentWorld::move() {//TODO: Iterate through map

	elapsedTicks++;
	 
	map<string, list<Actor*>*>::iterator it = virtualField.begin();
	map<string, list<Actor*>*>::iterator temp_it;
	list<Actor*>::iterator it2;
	list<Actor*>::iterator temp_it2;
	int doSomethingVal = 0;

	
	while( it != virtualField.end()){
		temp_it = next(it);//store next value into an iterator, just in case current position is erased
		it2 = (*(it->second)).begin();
		while (it2 != (*(it->second)).end()) {
			temp_it2 = next(it2);
			doSomethingVal = (*it2)->doSomething();  
			if (doSomethingVal == 0) {
				break;
			}
			else if (doSomethingVal == 3) {
				Actor* adult_grasshopper;
				adult_grasshopper = new AdultGrasshopper((*it2)->getX(), (*it2)->getY(), this);  
				(*(it->second)).push_back(adult_grasshopper); 
			}
			it2 = temp_it2;
			if (it2 == (*(it->second)).end())
				break;
			temp_it2 = next(it2);
		}
		it = temp_it;
		if (it == virtualField.end())
			break; 
		temp_it = next(it);
	}
	removeDeadActors();

	setDisplayText();

	if (elapsedTicks == 2000) {
		if (winningAnt == -1)
			return GWSTATUS_NO_WINNER;
		else
			setWinner((compilerPointers[winningAnt])->getColonyName());
		return GWSTATUS_PLAYER_WON;
	}
	return GWSTATUS_CONTINUE_GAME;
}


//////////////////////////////////////////////////////////////////////////
					//CLEANUP FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::cleanUp(){//TODO

	map<string, list<Actor*>*>::iterator it = virtualField.begin(); 
	list<Actor*>::iterator it2;
	vector<Compiler*>::iterator it3 = compilerPointers.begin();
	
	for (; it != virtualField.end(); it++) {
		if (it->second != nullptr) {
			for (it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
				delete (*it2);
			}
			delete it->second;
			it->second = nullptr;
		}
	}
	for (; it3 != compilerPointers.end(); it3++) {
		if (*it3 != nullptr) {
			delete (*it3);
			(*it3) = nullptr;
		}
	}
	compilerPointers.clear();

	return;
}


//////////////////////////////////////////////////////////////////////////
							//MOVEACTOR FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::moveActor(int oldX, int oldY, int newX, int newY, Actor* actor) {//TODO
	//cout << oldX << " " << oldY << " " << newX << " " << newY << endl;
	string oldLocation = convertToString(oldX, oldY);
	string newLocation = convertToString(newX, newY);
	bool deleted = false;
	//finding oldLocation block
	map<string, list<Actor*>*>::iterator it = virtualField.find(oldLocation);
	
	//Once oldLocation is found, find the pointer to the current actor in the subclass
	list<Actor*>::iterator beginning = (*(it->second)).begin();
	list<Actor*>::iterator end = (*(it->second)).end();
	list<Actor*>::iterator it2 = find(beginning, end, actor); 

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
		(*(it->second)).push_back(actor);
	}
	
	return deleted;
}


//////////////////////////////////////////////////////////////////////////
							//ISBLOCKED FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::isBlocked(int newX, int newY) {
	if (newX <= 0 || newX >= VIEW_WIDTH - 1 || newY <= 0 || newY >= VIEW_HEIGHT - 1)
		return true;
	string newLocation = convertToString(newX, newY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(newLocation);
	if (it == virtualField.end())
		return false;
	for (list<Actor*>::iterator it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
		if ((*it2)->canBlock())
			return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
							//CONVERTtoSTRING FUNCTION//
//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
				//CONVERTtoSTRING STRING OVERLOAD FUNCTION//
//////////////////////////////////////////////////////////////////////////
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

//LOADFILE FUNCTION//
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
//////////////////////////////////////////////////////////////////////////
							//DECODESTRING FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::decodeString(int &x, int &y, std::string location) {
	if (location.length() != 4)
		cout << "INVALID LOCATION" << endl;
	else {
		x = (location[0] - '0') * 10 + (location[1] - '0');
		y = (location[2] - '0') * 10 + (location[3] - '0');
	}
}

//////////////////////////////////////////////////////////////////////////
							//BUILDITEM FUNCTION//
//////////////////////////////////////////////////////////////////////////
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
		case Field::FieldItem::food: {

			Actor *food;
			food = new Food(startX, startY, this);
			return food;
		}
		case Field::FieldItem::water: {
			Actor *water;
			water = new PoolOfWater(startX, startY, this);
			return water;
		}
		case Field::FieldItem::poison: {
			Actor *poison;
			poison = new Poison(startX, startY, this);

			return poison;
		}
		case Field::FieldItem::anthill0: {
			if (getFilenamesOfAntPrograms().size() >= 1) {
				Actor* anthill0;
				anthill0 = new Anthill(startX, startY, this, compilerPointers[0], 0);
				return anthill0;
			}
			return nullptr;
		}
		case Field::FieldItem::anthill1: {
			if (getFilenamesOfAntPrograms().size() >= 2) {
				Actor* anthill1;
				anthill1 = new Anthill(startX, startY, this, compilerPointers[1], 1);
				return anthill1;
			}
			return nullptr;
		}
		case Field::FieldItem::anthill2: {
			if (getFilenamesOfAntPrograms().size() >= 3) {
				Actor* anthill2;
				anthill2 = new Anthill(startX, startY, this, compilerPointers[2], 2);
				return anthill2;
			}
			return nullptr;
		}
		case Field::FieldItem::anthill3: {
			if (getFilenamesOfAntPrograms().size() >= 4) {
				Actor* anthill4;
				anthill4 = new Anthill(startX, startY, this, compilerPointers[3], 3);
				return anthill4;
			}
			return nullptr;
		}
		default:
			return nullptr;
	}
}
//////////////////////////////////////////////////////////////////////////
							//makeFood FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::makeFood(int currentX, int currentY, int value) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation);

	list<Actor*>::iterator it_food = checkFood((*(it->second)));
	if (it_food != (*(it->second)).end()) {
		int food_units = static_cast<EnergyHolder*>((*it_food))->getHP(); 
		static_cast<EnergyHolder*>((*it_food))->setHP(food_units + value);
	}
	else {
		Actor* food;
		food = new Food(currentX, currentY, this); 
		static_cast<EnergyHolder*>(food)->setHP(value);
		(*(it->second)).push_back(food);
	}
}

//////////////////////////////////////////////////////////////////////////
							//makePheromone FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::makePheromone(int currentX, int currentY, int colonyNumber) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation);

	list<Actor*>::iterator it_pher = (*(it->second)).begin();//TODO: turn into function, to use in doSomethingVal == 3 case too
	for (; it_pher != (*(it->second)).end(); it_pher++) {
		if ((*it_pher)->isPheromone(colonyNumber)) {
			int potentialAmt = 786 - static_cast<EnergyHolder*>((*it_pher))->getHP();
			if (potentialAmt < 256) {
				static_cast<EnergyHolder*>((*it_pher))->setHP(786);
			}
			else
				static_cast<EnergyHolder*>((*it_pher))->setHP(static_cast<EnergyHolder*>((*it_pher))->getHP() + 256);
			return;
		}
	}
	Actor* pheromone;
	pheromone = new Pheromone(currentX, currentY, colonyNumber, this);
	(*(it->second)).push_back(pheromone);
	return;
}
//////////////////////////////////////////////////////////////////////////
							//CREATE_NEW_ANTS FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::createNewAnt(int currentX, int currentY, Compiler* c, int colonyNumber) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation);

	Actor* ant;
	ant = new Ant(currentX, currentY, this, c, colonyNumber);

	(*(it->second)).push_back(ant);

}

//////////////////////////////////////////////////////////////////////////
							//CREATENEWENTRIES FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::createNewEntries(int x, int y, Actor* actor) {
	string location = convertToString(x, y);
	list<Actor*>* mActor;
	mActor = new list<Actor*>;
	(*mActor).push_back(actor);
	virtualField[location] = mActor;  
	return;
}


//////////////////////////////////////////////////////////////////////////
							//GETCURRENTTICK FUNCTION//
//////////////////////////////////////////////////////////////////////////
int StudentWorld::getCurrentTick() {
	return elapsedTicks;
}


//////////////////////////////////////////////////////////////////////////
							//SETDISPLAYTEXT FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::setDisplayText() {
	string ticks = convertToString(2000 - elapsedTicks);

	ostringstream oss;
	oss << "Ticks:" ;
	oss << setw(5) << ticks << " - " ; 

	string fileName;
	vector<string> fileNames = getFilenamesOfAntPrograms();
	vector<int> colonyScores;

	colonyScores.push_back(numOfAnt0);
	colonyScores.push_back(numOfAnt1);
	colonyScores.push_back(numOfAnt2);
	colonyScores.push_back(numOfAnt3);
	int current_winningNumber;
	int current_winningAnt;
	if (fileNames.size() > 0) {
		current_winningNumber = colonyScores[0];
		current_winningAnt = 0;
	}
	int count = 0;

	for (int i = 1; i < fileNames.size(); i++) {
		if (colonyScores[i] > current_winningNumber) {
			current_winningNumber = colonyScores[i];
			current_winningAnt = i;
		}
	}

	if (current_winningNumber > winningNumber) {
		winningAnt = current_winningAnt;
		winningNumber = current_winningNumber;
	}

	if (winningNumber == 5)
		winningAnt = -1;

	int i = 0;
	for (vector<Compiler*>::iterator it = compilerPointers.begin(); it !=compilerPointers.end() && i<fileNames.size(); it++, i++) {
		fileName = (*it)->getColonyName();
		if (i == winningAnt) {
			oss << fileName << "*: " ;
			
		}
		else {
			oss << fileName << ": " ;
		}

		if (colonyScores[i] < 10) {
			oss << '0' << colonyScores[i] ;
		}
		else {
			oss << colonyScores[i] ;
		}
		oss << " ants  " ;
	}
	setGameStatText(oss.str());

}


//////////////////////////////////////////////////////////////////////////
							//CHECKITEM FUNCTION//
//////////////////////////////////////////////////////////////////////////
list<Actor*>::iterator StudentWorld::checkFood(list<Actor*> &subMap) {
	list<Actor*>::iterator it = subMap.begin();
	for (; it != subMap.end(); it++) {
		if ((*it)->isEdible())
			return it;
	};
	return it;
}


//////////////////////////////////////////////////////////////////////////
					//CHECKITEM BOOLEAN OVERLOAD FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::checkFood(int currentX, int currentY) {
	string currentLocation = convertToString(currentX, currentY);

	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation); 
	for (list<Actor*>::iterator it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
		if ((*it2)->isEdible())
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
							//CheckForEnemy FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::checkForEnemy(int currentX, int currentY, int colonyNumber) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation);
	for (list<Actor*>::iterator it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
		if ((*it2)->enemyStatus(colonyNumber))
			return true;
	}
	return false;

}

//////////////////////////////////////////////////////////////////////////
							//CheckForMyAnthill FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::checkForMyAnthill(int currentX, int currentY, int colonyNumber) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation);
	for (list<Actor*>::iterator it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
		if ((*it2)->isAnthill(colonyNumber)) 
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
							//CheckForPheromone FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::checkForPheromone(int currentX, int currentY, int colonyNumber) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation);
	if (it == virtualField.end())
		return false;
	for (list<Actor*>::iterator it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
		if ((*it2)->isPheromone(colonyNumber))
			return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
							//CheckForDanger FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::checkForDanger(int currentX, int currentY, int colonyNumber) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it = virtualField.find(currentLocation);
	if (it == virtualField.end())
		return false;
	for (list<Actor*>::iterator it2 = (*(it->second)).begin(); it2 != (*(it->second)).end(); it2++) {
		if ((*it2)->isDangerous(colonyNumber))
			return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
					//EAT FUNCTION//
//////////////////////////////////////////////////////////////////////////

int StudentWorld::eat(int currentX, int currentY, int value) {//TODO: DOUBLE CHECK EAT FUNCTION WORKS PROPERLY
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it_1;
	list<Actor*>::iterator it_2;

	it_1 = virtualField.find(currentLocation); 
	it_2 = checkFood((*(it_1->second)));

	if (it_2 == (*(it_1->second)).end()) {
		return 0;
	}
	if (static_cast<EnergyHolder*>((*it_2))->getHP() <= value) {
		int hp = static_cast<EnergyHolder*>((*it_2))->getHP();
		static_cast<EnergyHolder*>((*it_2))->setLifeStatus(false);
		return hp;
	}
	else
		static_cast<EnergyHolder*>((*it_2))->setHP(static_cast<EnergyHolder*>((*it_2))->getHP() - value);
	return value;
}

//////////////////////////////////////////////////////////////////////////
					//STUN FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::stun(int currentX, int currentY) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it_1;
	list<Actor*>::iterator it_2;

	it_1 = virtualField.find(currentLocation);
	it_2 = (*(it_1->second)).begin();

	for (; it_2 != (*(it_1->second)).end(); it_2++) {
		if ((*it_2)->isStunnable()) {
			if (!(static_cast<Insect*>((*it_2))->getStunnedStatus())) {//static cast here
				static_cast<Insect*>((*it_2))->setStunnedStatus(true);
				static_cast<Insect*>((*it_2))->addSleepTicks(2);
			}
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
						//POISON FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::poison(int currentX, int currentY) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it_1;
	list<Actor*>::iterator it_2;

	it_1 = virtualField.find(currentLocation);
	it_2 = (*(it_1->second)).begin();

	for (; it_2 != (*(it_1->second)).end(); it_2++) {
		if ((*it_2)->isStunnable()) {
			int currentHP = static_cast<EnergyHolder*>((*it_2))->getHP();
			int reducedHP = currentHP - 150;
			static_cast<EnergyHolder*>((*it_2))->setHP(reducedHP);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
						//BITE FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::bite(int currentX, int currentY, Actor* actor, int colonyNumber, int dmg) {
	string currentLocation = convertToString(currentX, currentY);
	map<string, list<Actor*>*>::iterator it_1;
	list<Actor*>::iterator it_2;

	it_1 = virtualField.find(currentLocation);
	it_2 = (*(it_1->second)).begin();

	vector<int> vActorPosition;
	for (int i = 0; it_2 != (*(it_1->second)).end() && i<(*(it_1->second)).size(); it_2++, i++) {
		if ((*it_2)->enemyStatus(colonyNumber) && (*it_2)!=actor) {
			vActorPosition.push_back(i);
		}
	}
	if (vActorPosition.size() == 0)
		return false;

	int randomVectorPosition = randInt(0, vActorPosition.size() - 1);
	int position = vActorPosition[randomVectorPosition];

	it_2 = (*(it_1->second)).begin();

	for (int i = 0; it_2 != (*(it_1->second)).end() && i<=position; it_2++, i++) {
		if (i == position) {
			static_cast<EnergyHolder*>((*it_2))->setHP(static_cast<EnergyHolder*>((*it_2))->getHP() - dmg);//static cast to Insect pointers
			static_cast<Insect*>((*it_2))->setBitten(true);
			if (static_cast<Insect*>((*it_2))->biteBack() && static_cast<EnergyHolder*>((*it_2))->getHP()>0) {
				if (randInt(0, 1) == 0) {
					bite(currentX, currentY, (*it_2), -5, 50);
				}
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
					//REMOVE_DEAD_ACTORS FUNCTION//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::removeDeadActors() {

	map<string, list<Actor*>*>::iterator it = virtualField.begin();
	map<string, list<Actor*>*>::iterator temp_it;
	list<Actor*>::iterator it2;
	list<Actor*>::iterator temp_it2;

	while(it != virtualField.end()) {
		temp_it = next(it);
		it2 = (*(it->second)).begin();
		while(it2 != (*(it->second)).end()) 
		{
			temp_it2 = next(it2);
			if ((*it2)->isLiving() && !(static_cast<EnergyHolder*>(*it2)->getLifeStatus())) {//check to make sure that a) this is a living thing and b) that it is dead
				delete (*it2);
				(*(it->second)).erase(it2);
			}
			it2 = temp_it2;
		}
		if ((*(it->second)).size() == 0) {
			virtualField.erase(it);
		}
		it = temp_it;
	}
}


//////////////////////////////////////////////////////////////////////////
					//checkForOpenSpaces FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::checkForSpaces(int currentX, int currentY) {
	double x;
	double y;
	int x_coord, y_coord;
	for (int i = 0; i <= 10; i++) {
		for (double j = 0; j < 360; j += 1) {
			x = paramEqX(i, j*(3.14159/180));
			y = paramEqY(i, j*(3.14159 / 180));
			if (x < 0)
				x_coord = x - 0.5;
			else
				x_coord = x + 0.5;
			if (y < 0)
				y_coord = y - 0.5;
			else
				y_coord = y + 0.5;
			if (!(isBlocked(x_coord + currentX, y_coord + currentY)))
				return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
					//ParamEqX FUNCTION//
//////////////////////////////////////////////////////////////////////////
double StudentWorld::paramEqX(int radius, double angle) {
	return radius*cos(angle);
}


//////////////////////////////////////////////////////////////////////////
					//ParamEqY FUNCTION//
//////////////////////////////////////////////////////////////////////////
double StudentWorld::paramEqY(int radius, double angle) {
	return radius*sin(angle);
}

//////////////////////////////////////////////////////////////////////////
					//JUMP FUNCTION//
//////////////////////////////////////////////////////////////////////////
bool StudentWorld::jump(int currentX, int currentY, Actor* actor) {

	int randomRadius = randInt(0, 10);
	double randomAngle = randInt(0, 360) * (3.14159 / 180);

	double x = paramEqX(randomRadius, randomAngle);
	double y = paramEqY(randomRadius, randomAngle);

	int x_coord;
	int y_coord;

	if (x < 0)
		x_coord = x - 0.5;
	else
		x_coord = x + 0.5;
	if (y < 0)
		y_coord = y - 0.5;
	else
		y_coord = y + 0.5;

	if (!(isBlocked(x_coord + currentX, y_coord + currentY)))
		return moveActor(currentX, currentY, x_coord + currentX, y_coord + currentY, actor);
	else
		jump(currentX, currentY, actor);  

	/*int random = randInt(0, sl.size());
	set<string>::iterator it = sl.begin();
	for (int i = 0; it != sl.end() && i < random; it++, i++);
	int final_x;
	int final_y;
	decodeString(final_x, final_y, *it);*/
	
}
//////////////////////////////////////////////////////////////////////////
					//increment/decrease FUNCTIONS//
//////////////////////////////////////////////////////////////////////////
void StudentWorld::increaseScore(int colNumber) {
	switch (colNumber) {
	case 0:
		numOfAnt0++;
		break;
	case 1:
		numOfAnt1++;
		break;
	case 2:
		numOfAnt2++;
		break;
	case 3:
		numOfAnt3++;
		break;
	}
}

void StudentWorld::decreaseScore(int colNumber) {
	switch (colNumber) {
	case 0:
		numOfAnt0--;
		break;
	case 1:
		numOfAnt1--;
		break;
	case 2:
		numOfAnt2--;
		break;
	case 3:
		numOfAnt3--;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//incrementCol... FUNCTIONS//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
					//DESTRUCTOR FUNCTION//
//////////////////////////////////////////////////////////////////////////
StudentWorld::~StudentWorld(){//TODO
	cleanUp();
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
