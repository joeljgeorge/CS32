#include "newSequence.h"
#include <iostream>

Sequence::Sequence(int size) {
	seqArray = new ItemType[size];
	arr_size = size;
	item_size = 0;
}

Sequence::Sequence() {
	seqArray = new ItemType[DEFAULT_MAX_ITEMS];
	arr_size = DEFAULT_MAX_ITEMS;
	item_size = 0;
}

Sequence::~Sequence() {
	delete[] seqArray;
	arr_size = 0; 
	item_size = 0;
}

Sequence::Sequence(const Sequence &src) {
	arr_size = src.arr_size;
	item_size = src.item_size;
	seqArray = new ItemType[arr_size];
	for (int i = 0; i < item_size; i++) 
	{
		seqArray[i] = src.seqArray[i];
	}
}

Sequence& Sequence::operator=(const Sequence &src){
	if (&src == this)
		return *this;
	delete[] seqArray;
	arr_size = src.arr_size;
	seqArray = new ItemType[arr_size];
	item_size = src.item_size;
	for (int i = 0; i < item_size; i++) {
		seqArray[i] = src.seqArray[i];
	}
	return *this;
}

bool Sequence::empty() const {
	if (item_size == 0)
		return true;
	else
		return false;
}

int Sequence::size() const {
	return item_size;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos<0 || pos > item_size || item_size == arr_size)
		return false;
	else {
		ItemType temp = seqArray[pos];
		ItemType temp2;
		for (int i = pos + 1; i <= item_size; i++) {
			temp2 = seqArray[i];
			seqArray[i] = temp;
			temp = temp2;
		}
		seqArray[pos] = value;
		item_size++;
		return true;
	}
}

int Sequence::insert(const ItemType& value) {
	int p = item_size;
	for (int i = 0; i < item_size; i++) {
		if (value <= seqArray[i]) {
			p = i;
			break;
		}
	}
	if (insert(p, value))
		return p;
	else
		return -1;
}

bool Sequence::erase(int pos) {
	if (0 <= pos && pos < size()) {
		for (int i = pos; i < size(); i++) {
			seqArray[i] = seqArray[i + 1];
		}
		item_size--;
		return true;
	}
	else
		return false;
}

int Sequence::remove(const ItemType& value) {
	int numRemoved = 0;
	for (int i = 0; i < size(); i++) {
		if (seqArray[i] == value) {
			erase(i);
			numRemoved++;
			i--;
		}
	}
	return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const {
	if (0 <= pos && pos < size()) {
		value = seqArray[pos];
		return true;
	}
	else
		return false;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (0 <= pos && pos < size()) {
		seqArray[pos] = value;
		return true;
	}
	else
		return false;
}

int Sequence::find(const ItemType& value) const {
	int position = -1;
	for (int i = 0; i < size(); i++) {
		if (seqArray[i] == value) {
			position = i;
			break;
		}
	}
	return position;
}

void Sequence::swap(Sequence& other) {
	ItemType *temp = seqArray; 
	seqArray = other.seqArray; 
	other.seqArray = temp;

	int temp_isize = item_size; 
	item_size = other.item_size;
	other.item_size = temp_isize;

	int temp_asize = arr_size;
	arr_size = other.arr_size;
	other.arr_size = temp_asize;
}

/*void Sequence::dump() const {
	for (int i = 0; i < item_size; i++)
		std::cerr << seqArray[i] << std::endl;
}*/
