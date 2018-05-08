#include "SLL.h"

void main() {
	SLL sll;

	cout << "Last Node Add (for Loop) : ";
	for (int i = 0; i < 10; i++)
		sll.Add(i);
	sll.Print();
	cout << endl;

	cout << "Last Node Erase : ";
	sll.Erase();
	sll.Print();
	cout << endl;

	cout << "Nth Node Add (100, 3th) : ";
	sll.Add(100, 3);
	sll.Print();
	cout << endl;

	cout << "Nth Node Erase (3th) : ";
	sll.Erase(3);
	sll.Print();
	cout << endl;

	cout << "Node Reverse : ";
	sll.Reverse();
	sll.Print();
	cout << endl;
}