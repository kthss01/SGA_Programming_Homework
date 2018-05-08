#include "Node.h"



Node::Node()
{
}


Node::Node(int data, Node * pNext)
	:data(data), pNext(pNext)
{
}

Node::~Node()
{
}

void Node::AddNode(Node * pNext)
{
	if (this->pNext == NULL) {
		this->pNext = pNext;
	}
	else {
		this->pNext->AddNode(pNext);
	}
}

void Node::EraseNode()
{
	if (this->pNext->pNext != NULL)
		this->pNext->EraseNode();
	else {
		delete (this->pNext);
		this->pNext = NULL;
	}
}

void Node::Delete()
{
	if (this->pNext != NULL)
		this->pNext->Delete();
	
	delete (this);
	//(*this) = NULL; // ¾ø¾îµµ µÊ
}

void Node::Print()
{
	if (this->pNext == NULL) {
		cout << data << endl;
	}
	else {
		cout << data << "->";
		this->pNext->Print();
	}
}
