// Sequence.cpp

#include "Sequence.h"
#include <iostream>

Sequence::Sequence()
	: m_size(0)
{
	head = nullptr;
	tail = nullptr;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > size())//if position is negative or greater than the size of the array (i.e. it is beyond the last position), return false (bounds check)
		return false;
	uncheckedInsert(pos, value);
	return true;
}

int Sequence::insert(const ItemType& value)
{
	Node *navigator = head;
	int pos;
	for (pos = 0; pos < size(); pos++)//find position of first value that is less than or equal to passed in value
	{
		if (value <= navigator->value)
			break;
		navigator = navigator->next;
	}
	uncheckedInsert(pos, value);//insert value
	return pos;
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= size())//bounds check
		return false;
	if (pos == 0 && size() == 1) {//if there's only one item in the list, delete the node and set head and tail to nullptr
		delete head;
		head = nullptr;
		tail = nullptr;
		m_size--;
		return true;
	}
	if (pos == 0) //erasing node at beginning of linked list
	{//IF THIS DOESN'T WORK, LOOK AT PAGE 141 OF COURSE READER!!!
		Node *temp = head->next;//saving address of next node
		delete head; 
		head = temp;//moving head to next node
		head->prev = nullptr;
		m_size--;
		return true;
	}
	else if (pos == size() - 1)//erasing node at end of linked list
	{
		Node *temp = tail->prev;//saving address of previous node
		delete tail;
		tail = temp;//moving tail to previous node
		tail->next = nullptr;
		m_size--;
		return true;

	}
	else {//erasing node in the middle
		Node *temp = head;
		for (int i = 0; i < pos; i++) {
			temp = temp->next; //traversing through the linked list until temp points at node in desired position
		}
		temp->prev->next = temp->next;//set the next pointer of previous node to node that follows current node
		temp->next->prev = temp->prev;//set the prev pointer of next node to node that precedes the current node
		delete temp;//delete current node
		temp = nullptr;
		m_size--;
		return true;
	}
	
}

int Sequence::remove(const ItemType& value)
{
	int numRemoved = 0;
	Node *temp = head; 

	for (int i = 0; i < size(); i++) {
		if (temp == nullptr)//probably unnecessary
			break;
		Node *placeHolder = temp->next;
		if (temp->value == value) {//if node value equals parameter value, erase it, increment numRemoved, and decrement i to avoid skipping over nodes
			erase(i);
			numRemoved++;
			i--;
		}
		temp = placeHolder;
	} 
	return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= size())//bounds check
		return false;
	Node *temp = head;
	for (int i = 0; i < pos; i++) {//traverse through array until temp points at node in desired position
		temp = temp->next;
	}
	value = temp->value; //set value parameter equal to value of node
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= size())//bounds check
		return false;
	Node *temp = head;
	for (int i = 0; i < pos; i++) {//again, list traversal
		temp = temp->next;
	}
	temp->value = value;//set value at node equal to parameter value
	return true;
}

int Sequence::find(const ItemType& value) const
{
	Node *temp = head;
	for (int i = 0; i < size(); i++) {
		if (temp->value == value)//if value of node equals value, return position of node
			return i;
		temp = temp->next;
	}

	return -1;
}

void Sequence::swap(Sequence& other)
{
	Node *head_temp = this->head; //first, swap the head pointers of both lists
	this->head = other.head;
	other.head = head_temp;

	Node *tail_temp = this->tail; //next, swap the tail pointers of both lists
	this->tail = other.tail;
	other.tail = tail_temp;
	
	int tempSize = m_size; //finally, swap the sizes of both lists
	m_size = other.m_size;
	other.m_size = tempSize;
}

void Sequence::uncheckedInsert(int pos, const ItemType& value)
{
	Node *p;
	p = new Node;
	p->value = value; 
	if (head == nullptr) {//if linked list is empty
		head = p;//set head to point to node
		p->prev = nullptr;
		p->next = nullptr;
		tail = p;//set tail to also point to node
		m_size++;//increment size of list
	}
	else if (pos == size()) {//when trying to insert at end of linked list
		tail->next = p;//make the node at the end of the list at new node
		p->prev = tail; //set new node's prev pointer to the node that tail still points to
		p->next = nullptr;//set new node's next pointer to nullptr
		tail = p;//set tail to point to node p
		m_size++;
	}
	else if (pos == 0) {//when trying to insert at beginning of linked list
		p->prev = nullptr;//pretty similar protocol to that of insertion at end of list
		p->next = head;
		head->prev = p;
		head = p;
		m_size++;
	}
	else //when trying to insert a node somewhere in the middle
	{
		Node *temp = head;
		for (int i = 0; i < pos; i++)
		{
			temp = temp->next;//traverse list until temp points at node in desired position
		}
		p->next = temp;//set new node's next pointer to temp
		p->prev = temp->prev;//set new node's prev pointer to node that precedes temp
		p->prev->next = p;//set next pointer of node preceding new node to p
		p->next->prev = p;//set prev pointer of node that temp points to p
		m_size++;
	}
}

Sequence::~Sequence() {
	Node *temp = head;
	while (temp != nullptr) {//deallocating every node
		Node *n = temp->next;
		delete temp;
		temp = n;
	}
}

Sequence::Sequence(const Sequence &src) {
	Node *srcNavigator = src.head;//create pointer to traverse src linked list
	this->head = nullptr;//set current head pointer to nullptr (empty list)
	this->tail = nullptr;//set current tail pointer to nullptr 
	Node *thisNavigator = this->head;//create pointer to traverse this linked list
	this->m_size = src.size();//set current size to size of src linked list
	for (int i = 0; i < this->m_size; i++)
	{
		if (i == 0) {//creating first node
			Node *p = new Node;//create new node
			this->head = p;//make head and tail point to it
			this->tail = p;
			p->value = srcNavigator->value;//set the value of this new node to the value of the first node in src linked list
			p->prev = nullptr;//set prev and next to nullptr, as there are currently no other nodes
			p->next = nullptr;
			srcNavigator = srcNavigator->next;//move on to the next node in the linked list
			thisNavigator = head;//set the traversal pointer for this list to point to the first node
		}
		else {
			Node *p = new Node;//create new node
			thisNavigator->next = p;//thisNavigator points to the previous node, so this sets the next value of the previous node to the new node created
			p->value = srcNavigator->value;
			p->prev = thisNavigator;//set prev data member to thisNavigator
			p->next = nullptr;//set next to nullptr, as there is no node following this rn
			tail = p;//set tail to this newly created node at the end
			thisNavigator = thisNavigator->next;//move traversal pointer to this pointer
			srcNavigator = srcNavigator->next;
		}
	}
}

Sequence& Sequence::operator=(const Sequence &src) {
	if (this->head == src.head)//if aliased, do not do anything
		return *this;

	Node *temp = this->head;//delete all of the current nodes
	while (temp != nullptr) {
		Node *n = temp->next;
		delete temp;
		temp = n;
	}
	this->head = nullptr;
	this->tail = nullptr;
	//CODE FROM COPY CONSTRUCTOR
	Node *srcNavigator = src.head;//create pointer to traverse src linked list
	Node *thisNavigator = this->head;//create pointer to traverse this linked list
	this->m_size = src.size();//set current size to size of src linked list
	for (int i = 0; i < this->m_size; i++)
	{
		if (i == 0) {//creating first node
			Node *p = new Node;//create new node
			this->head = p;//make head and tail point to it
			this->tail = p;
			p->value = srcNavigator->value;//set the value of this new node to the value of the first node in src linked list
			p->prev = nullptr;//set prev and next to nullptr, as there are currently no other nodes
			p->next = nullptr;
			srcNavigator = srcNavigator->next;//move on to the next node in the linked list
			thisNavigator = head;//set the traversal pointer for this list to point to the first node
		}
		else {
			Node *p = new Node;//create new node
			thisNavigator->next = p;//thisNavigator points to the previous node, so this sets the next value of the previous node to the new node created
			p->value = srcNavigator->value;
			p->prev = thisNavigator;//set prev data member to thisNavigator
			p->next = nullptr;//set next to nullptr, as there is no node following this rn
			tail = p;//set tail to this newly created node at the end
			thisNavigator = thisNavigator->next;//move traversal pointer to this pointer
			srcNavigator = srcNavigator->next;
		}
	}
	return *this;
	
}
void Sequence::dump() const {
	Node *temp = head;
	for (int i = 0; i < size(); i++) {
		std::cerr << temp->value << std::endl;
		temp = temp->next;
	}

}

//non-member functions 

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	if (seq2.size() > seq1.size() || seq2.size() == 0)//if seq2 is larger than seq1 or if seq2 is empty, return -1
		return -1;
	int i = 0, j = 0;
	int count = 0;
	while (j < seq1.size() && i < seq2.size()) {
		ItemType temp1;
		seq1.get(j, temp1);//get value of seq1 at position j
		ItemType temp2;
		seq2.get(i, temp2);//get value of seq2 at position i 
		if (temp1 == temp2) {//if the two values are equal, increment both to the next value and increment count
			i++;
			j++;
			count++;
		}
		else {//if values are not equal, go back to the start of seq2 and set count back to 0
			if (i == 0)//if we're already at the beginning of seq2, increment j++ so as to avoid an infinite loop
				j++;
			i = 0;
			count = 0;
		}
	}
	if (count != seq2.size())//if seq2 is a subsequence of seq1, count will be equal to the size of seq2. If this condition is not met, that means seq2...
		return -1;//...is not a subsequence of seq1
	else
		return j - i;//if seq2 is a subsequence of seq1, i should be equal to the length of seq2, and j will be whatever position comes right after the end...
}					 //...of subsequence seq2. Subtracting i from j will give you the position at which the subsequence starts

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	int count = 1;//determines which sequence to copy value from (1 for seq1 and 2 for seq2)
	int j = 0, k = 0;//tracking position through seq1 and seq2
	Sequence result_temp; 
	for (int i = 0; i < seq1.size() + seq2.size(); i++) {//length of result should be the sum of the two sequences' sizes
		if (count == 1) {//if count == 1, copy value from seq1...
			if (j >= seq1.size()) {//...unless we've already accessed the last element in seq1
				ItemType value;//in that case, copy value from seq2
				seq2.get(k, value);
				result_temp.insert(i, value);
				k++;
			}
			else {//if we haven't gone through seq1 yet, copy the value from seq1
				ItemType value;
				seq1.get(j, value);
				result_temp.insert(i, value);
				j++;//increment j, so that next time we access seq1 we can copy the value in the next node
			}
			count++;//increment count, so that the next value will come from seq2 if possible
		}

		else if (count == 2) {//similar logic to above case, except applied to seq2 instead of seq1
			if (k >= seq2.size()) {
				ItemType value;
				seq1.get(j, value);
				result_temp.insert(i, value);
				j++;
			}
			else {
				ItemType value;
				seq2.get(k, value);
				result_temp.insert(i, value);
				k++;
			}
			count--;//decrement count, so that the next value will come from seq1
		}
	}
	result = result_temp;//copy result_temp into result
}
