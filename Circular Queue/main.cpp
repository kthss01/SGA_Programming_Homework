// Circular Queue (È¯Çü Å¥)

#include <iostream>
#include <time.h>
using namespace std;

#include "CircularQueue.h"

#define CAPACITY 12

int main() {
	srand(time(NULL));

	

	CircularQueue<int> queue(CAPACITY);

	for (int i = 0; i < CAPACITY; i++) {
		int temp = rand() % CAPACITY;
		cout << "\tEnque : " << temp << endl;
		queue.Enqueue(temp);

		if (i % (CAPACITY / 4) == 0 ||
			i == CAPACITY - 1) {
			cout << "Queue : ";
			queue.Show();
			cout << endl;
		}
	}

	cout << "\tDeque : " << queue.Dequeue() << endl;
	cout << "\tDeque : " << queue.Dequeue() << endl;
	cout << "\tDeque : " << queue.Dequeue() << endl;
	
	cout << "Queue : ";
	queue.Show();
	cout << endl;

	return 0;
}