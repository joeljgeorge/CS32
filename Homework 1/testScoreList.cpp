#include <iostream>
#include "ScoreList.h"

int main() {
	ScoreList a;
	if (a.maximum() == NO_SCORE && a.minimum() == NO_SCORE & a.size() == 0) {
		std::cout << "Empty scorelist, no max or min yet." << std::endl;
	}
	if (a.add(97))
		std::cout << "worked" << std::endl;
	a.add(72);
	a.add(97);
	a.add(-4); 
	std::cout << "current size: " << a.size() << std::endl;
	std::cout << "Contents of list: " << std::endl;
	//a.scoreList.dump();
	std::cout << "Minimum is: " << a.minimum() << std::endl;
	std::cout << "Maximum is: " << a.maximum() << std::endl;
	if (a.remove(97))
		std::cout << "removed!" << std::endl;
	else
		std::cout << "wasn't removed" << std::endl;
	std::cout << "current size: " << a.size() << std::endl;
	std::cout << "Contents of list: " << std::endl;
	//a.scoreList.dump();
	std::cout << "Minimum is: " << a.minimum() << std::endl;
	std::cout << "Maximum is: " << a.maximum() << std::endl;
	ScoreList b;
	b.add(78);
	b.add(78);
	b.add(78);
	std::cout << "Contents of list: " << std::endl;
	//b.scoreList.dump();
	std::cout << "Minimum is: " << b.minimum() << std::endl;
	std::cout << "Maximum is: " << b.maximum() << std::endl;
	ScoreList c = b;
	std::cout << "Contents of list: " << std::endl;
	//c.scoreList.dump();
	std::cout << "Minimum is: " << c.minimum() << std::endl;
	std::cout << "Maximum is: " << c.maximum() << std::endl;
	b = a;
	std::cout << "Contents of list: " << std::endl;
	//b.scoreList.dump();
	std::cout << "Minimum is: " << b.minimum() << std::endl;
	std::cout << "Maximum is: " << b.maximum() << std::endl;

}
