// Binary Search Tree 이진 탐색 트리

#include <iostream>
#include <time.h>
using namespace std;

#include "BinarySearchTree.h"

#define COUNT 10

void BuildSearchTree(Tree<int>& tree) {

	for (int i = 0; i < COUNT; i++) {
		int temp = rand() % COUNT;
		tree.InsertNode(new TreeNode<int>(temp));
	}
}

int main() {
	srand(time(NULL));

	Tree<int> tree(COUNT);

	BuildSearchTree(tree);
	cout << endl;

	// Tree 순회
	cout << "Tree Traversal" << endl;
	// PreOrder
	cout << "PreOrder >> ";
	tree.PreOrder(tree.GetRoot());
	cout << endl;

	// InOrder
	cout << "InOrder >> ";
	tree.InOrder(tree.GetRoot());
	cout << endl;

	// PostOrder
	cout << "PostOrder >> ";
	tree.PostOrder(tree.GetRoot());
	cout << endl;
	
	cout << endl;

	// Tree에서 노드 찾기
	int number;
	cout << "Input number to search >> " << endl;
	//cin >> number;

	TreeNode<int>* found = NULL;
	
	int cnt = 0;

	while (found == NULL || cnt < 5) {
		number = rand() % COUNT;
		cout << "Number : " << number << " >> ";
		found = tree.Search(tree.GetRoot(), number);

		if (found != NULL) {
			cout << "Found Node." << endl;
		}
		else {
			cout << "Not Found Node." << endl;
		}
		cnt++;
	}

	return 0;
}