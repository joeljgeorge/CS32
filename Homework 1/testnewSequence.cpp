#include <iostream>
#include "newSequence.h"

int main() {
	Sequence a;
	if (a.empty())
		std::cout << "empty" << std::endl;
	else
		std::cout << "full" << std::endl;
	std::cout << a.size() << std::endl;
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
	std::cout << "Size of list A" << a.size() << std::endl;
	a.erase(0);
	std::cout << a.size() << std::endl;
	a.insert(1);
	a.insert(2);
	a.insert(3);
	a.insert(1);
	std::cout << "Contents of List A: " << std::endl;
	//a.dump();

	if (a.remove(1) == 2)
		std::cout << "worked yet again!" << std::endl;
	else
		std::cout << "nope" << std::endl;
	std::cout << "Size of list A: " << a.size() << std::endl;
	std::cout << "Contents of List A: " << std::endl;
	//a.dump();
	if (a.find(3) == 1 && a.find(2123) == 2)
		std::cout << "worked stupendously!" << std::endl;
	else
		std::cout << "well this is awkward" << std::endl;
	Sequence b(4);
	b.insert(1);
	b.insert(11);
	b.insert(111);
	b.insert(1111);
	std::cout << "Contents of List B: " << std::endl;
	//b.dump();
	b.swap(a);
	std::cout << "Contents of List B: " << std::endl;
	//b.dump();
	std::cout << "Contents of List A: " << std::endl;
	//a.dump();
	if (a.insert(-10)==-1 && b.insert(100000))
		std::cout << "worked yet a- yet again!" << std::endl;
	else
		std::cout << "failed" << std::endl;
	std::cout << "Contents of List A: " << std::endl;
	//a.dump();
	std::cout << "Contents of List B: " << std::endl;
	//b.dump();
	Sequence c = a;
	std::cout << "Contents of List C: " << std::endl;
	//c.dump();
	std::cout << "Size of List C: " << c.size() << std::endl;
	a = b;
	std::cout << "Contents of List A: " << std::endl;
	//a.dump();
}