#ifndef NEWSEQUENCE_H
#define NEWSEQUENCE_H

typedef unsigned long ItemType;
const int DEFAULT_MAX_ITEMS = 200;

class Sequence
{
public:
	Sequence(); 

	Sequence(int size);

	Sequence(const Sequence &src);

	Sequence &operator=(const Sequence &src);

	~Sequence();

	bool empty() const;

	int size() const;

	bool insert(int pos, const ItemType& value);

	int insert(const ItemType& value);

	bool erase(int pos);

	int remove(const ItemType& value);

	bool get(int pos, ItemType& value) const;

	bool set(int pos, const ItemType& value);

	int find(const ItemType& value) const;

	void swap(Sequence& other);

private:

	int item_size;
	int arr_size;
	ItemType *seqArray;

};

#endif //NEWSEQUENCE_H