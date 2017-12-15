#ifndef MYMAP
#define MYMAP

#include <iostream>
template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node {
		KeyType key;
		ValueType value;
		Node *left, *right;
	};
	Node *rootPtr;
	Node* createNewNode(const KeyType& key, const ValueType& value); 
	void deleteAllNodes(Node* nodePtr);
	ValueType* search(Node* nodePtr, const KeyType& key) const;
	void addNode(KeyType key, ValueType value);
	int sizeOfTree;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() {
	 rootPtr = nullptr;
	 sizeOfTree = 0;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap() {
	deleteAllNodes(rootPtr);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear() {
	deleteAllNodes(rootPtr);
	rootPtr = nullptr;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const{
	return sizeOfTree;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
	addNode(key, value);
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const {
	return search(rootPtr, key);
}

//helper functions
template<typename KeyType, typename ValueType>
typename MyMap<KeyType, ValueType>::Node* MyMap<KeyType, ValueType>::createNewNode(const KeyType& key, const ValueType& value) {
	Node* nodePtr;
	nodePtr = new Node;
	nodePtr->key = key;
	nodePtr->value = value; 
	nodePtr->left = nullptr;
	nodePtr->right = nullptr;
	sizeOfTree++;
	return nodePtr;
}

template<typename KeyType, typename ValueType> 
void MyMap<KeyType, ValueType>::deleteAllNodes(Node* nodePtr) {
	if (nodePtr == nullptr)
		return;
	if(nodePtr->left != nullptr)
		deleteAllNodes(nodePtr->left);
	if (nodePtr->right != nullptr)
		deleteAllNodes(nodePtr->right);
	delete nodePtr;
	nodePtr = nullptr;
	return;
}

template<typename KeyType, typename ValueType>
ValueType* MyMap<KeyType, ValueType>::search(Node* nodePtr, const KeyType& key) const{
	if (nodePtr == nullptr)
		return nullptr;
	else if (key == nodePtr->key)
		return &(nodePtr->value);
	else if (key < nodePtr->key) {
		return search(nodePtr->left, key);
	}
	else if (key > nodePtr->key) {
		return search(nodePtr->right, key);
	}
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::addNode(KeyType key, ValueType value) {
	if (rootPtr == nullptr)
	{
		rootPtr = createNewNode(key, value);
		return;
	}
	Node* newNodePtr = createNewNode(key, value);
	Node* traversalPtr = rootPtr;
	for (;;) {
		if (traversalPtr->key == key) {
			traversalPtr->value = value;
			delete newNodePtr;
			newNodePtr = nullptr;
			return;
		}
		else if (key < traversalPtr->key) {
			if (traversalPtr->left != nullptr)
				traversalPtr = traversalPtr->left;
			else {
				traversalPtr->left = newNodePtr;
				return;
			}
		}
		else if (key > traversalPtr->key) {
			if (traversalPtr->right != nullptr)
				traversalPtr = traversalPtr->right;
			else {
				traversalPtr->right = newNodePtr;
				return;
			}
		}
	}
	return;
}

#endif MYMAP