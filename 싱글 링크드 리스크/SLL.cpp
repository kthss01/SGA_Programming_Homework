#include "SLL.h"



SLL::SLL()
	:pHead(NULL)
{
}


SLL::~SLL()
{
	if (pHead != NULL)
		pHead->Delete();
}

void SLL::Add(int data)
{
	Node * pNewNode = new Node(data);

	if (pHead == NULL) {
		pHead = pNewNode;
	}
	else {
		pHead->AddNode(pNewNode);
	}
}

void SLL::Add(int data, int num)
{
	Node * pNewNode = new Node(data);

	Node *p, *q;
	p = pHead;

	int cnt = 0;
	while (p->GetNextNode() != NULL) {
		q = p->GetNextNode();
		cnt++;
		if (cnt == num)
			break;
		p = q;
	}
	pNewNode->SetNextNode(q);
	p->SetNextNode(pNewNode);
}

void SLL::Erase()
{
	pHead->EraseNode();
}

void SLL::Erase(int num)
{
	Node *p, *q;
	p = pHead;
	int cnt = 0;
	while (p->GetNextNode() != NULL) {
		q = p->GetNextNode();
		cnt++;
		if (cnt == num)
			break;
		p = q;
	}
	p->SetNextNode(q->GetNextNode());
	delete q;
}

void SLL::Reverse()
{
	Node *p, *q, *r;
	p = pHead;
	q = NULL;
	
	while (p != NULL) {
		r = q;
		q = p;
		p = p->GetNextNode();
		q->SetNextNode(r);
	}

	pHead = q;
}

void SLL::Print()
{
	if (pHead != NULL)
		pHead->Print();
}
