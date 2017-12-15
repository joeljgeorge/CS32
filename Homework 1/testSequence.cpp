#include "Sequence.h"
#include <iostream>

int main() {
	Sequence a;
	if (a.empty())
		std::cout << "empty" << std::endl;
	else
		std::cout << "full" << std::endl;
	std::cout << "Size of list A: " <<a.size() << std::endl;
	if (a.insert(0, 1100)) {
		std::cout << "worked!" << std::endl;
	}
	else
		std::cout << "failed" << std::endl;
	a.insert(1, 2122);
	a.insert(1, 2123);
	if (a.insert(2222) == -1)
		std::cout << "failed" << std::endl;
	else
		std::cout << "worked again!" << std::endl;
	std::cout << "Size of list A: "<< a.size() << std::endl;
	if (a.erase(3))
		std::cout << "Value erased!" << std::endl;
	else
		std::cout << "erasing failed..." << std::endl;
	std::cout << "Size of list A: " <<a.size() << std::endl;
	a.insert(1);
	a.insert(2);
	a.insert(3);
	a.insert(1);
	std::cout << "List A Contents: " << std::endl;
	a.dump();

	if (a.remove(1) == 2)
		std::cout << "worked yet again!" << std::endl;
	else
		std::cout << "nope" << std::endl;
	std::cout << "Size of list A: " << a.size() << std::endl;
	std::cout << "List A Contents: " << std::endl;
	a.dump(); 
	if (a.find(3) == 1 && a.find(2123) == 3)
		std::cout << "worked stupendously!" << std::endl;
	else
		std::cout << "well this is awkward" << std::endl;
	Sequence b;
	b.insert(1);
	b.insert(11);
	b.insert(111);
	b.insert(1111);
	b.swap(a);
	std::cout << "List A Contents: " << std::endl;
	a.dump();
}