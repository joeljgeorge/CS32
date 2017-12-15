#include <iostream>
#include "Sequence.h"

Sequence::Sequence() {
	item_size = 0; 
}

bool Sequence::empty() const{
	if (item_size == 0)
		return true;
	else
		return false;
}

int Sequence::size() const{
	return item_size;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos<0 || pos > item_size || item_size == DEFAULT_MAX_ITEMS)//if pos is negative, greater than the current number of items, or the number of items
		return false;											   //is equal to the array size (i.e. the array is full), return false
	else {
		ItemType temp = seqArray[pos];//set temp equal to the value in the current array position
		ItemType temp2; //initialize temp
		for (int i = pos+1; i<=item_size; i++) {//starting at the next position, as long as i is less than or equal to the # of items (i.e.
			temp2 = seqArray[i];			    //the position does not exceed the array's current size), shift the values of the sequence up
			seqArray[i] = temp;
			temp = temp2; 
		}
		seqArray[pos] = value; //set the value at pos equal to the parameter value
		item_size++; //incremement the item_size
		return true; 
	}
}

int Sequence::insert(const ItemType& value) {
	int p = item_size; 
	for (int i = 0; i < item_size; i++) {
		if (value <=seqArray[i]) {
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
	int loopSize = this->size();
	if (other.size() >= this->size())
		loopSize = other.size();
	for (int i = 0; i < loopSize; i++)
	{
		ItemType temp = this->seqArray[i];
		this->seqArray[i] = other.seqArray[i];
		other.seqArray[i] = temp;
	}
	int tempSize = this->size();
	this->item_size = other.size();
	other.item_size = tempSize;
}

void Sequence::dump() const {
	for (int i = 0; i < item_size; i++)
		std::cerr << seqArray[i] << std::endl;
}
