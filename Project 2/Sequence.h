#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string>

typedef std::string ItemType;

class Sequence
{
public:
	Sequence();
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
	void dump() const;
	~Sequence(); 
	Sequence(const Sequence &src); 
	Sequence &operator=(const Sequence &src);

private:
	struct Node {
		ItemType value; 
		Node* next;
		Node* prev;
	};
	Node *head, *tail; 
	int m_size;
	void uncheckedInsert(int pos, const ItemType& value);
};

inline
int Sequence::size() const
{
	return m_size;
}

inline
bool Sequence::empty() const
{
	return size() == 0;
}

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
#endif //SEQUENCE_H
