#pragma once

#include <iostream>
using namespace std;

class Node
{
private:
	int data;
	Node* pNext;
public:
	Node();
	Node(int data, Node* pNext = NULL);
	~Node();

	// 노드 끝에 붙이는거 
	void AddNode(Node* pNext);

	void EraseNode();

	Node * GetNextNode() { return pNext; }
	void SetNextNode(Node * pNext) { this->pNext = pNext; }

	// 노드 다 지우는 함수
	void Delete();
	void Print();
};

/*
	과제
	중간 삽입 (몇번째에 넣을지)
	중간 삭제 (내가 삭제하고 싶은 값이 있을때 or 몇번째 노드 삭제)
	리버스 (뒤집기)
	ex) 1 2 3 -> 3 2 1

	Node 나 SLL 클래스 아무데나 과제 코드 구현

	reverse 는 헤더의 Next 널 다음 값이 헤더 가리키는 식으로 쭉쭉하면 될듯
*/

