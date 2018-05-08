#pragma once

#include "Node.h"

class SLL
{
private:
	Node * pHead;
public:
	SLL();
	~SLL();

	void Add(int data);
	void Add(int data, int num);
	void Erase();
	void Erase(int num);
	void Reverse();
	void Print();
};

