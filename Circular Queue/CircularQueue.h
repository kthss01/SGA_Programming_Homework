#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class CircularQueue
{
private:
	T * data; // 데이터 배열
	int capacity; // 큐 용량
	int front; // 전단 인덱스
	int rear; // 후단 인덱스
public:
	CircularQueue(int capacity) {
		capacity = capacity + 1; // 데이터 배열의 크기는 실제 용량에서 1을 더한 크기 (더미 공간 때문)
		data = new T[capacity]; // T 배열 capacity + 1개를 메모리 공간에 할당
		front = rear = 0; // 전단 후단 초기화
	}
	
	~CircularQueue() {
		delete[] data; // 데이터 배열 소멸
	}
	
	void Clear() {
		front = rear = 0;
	}
	
	void Enqueue(const T& data) {
		int index; // 데이터가 들어갈 인덱스
		index = rear;
		rear = (rear + 1) % (capacity);
		this->data[index] = data; // index 번째의 노드의 데이터에 data를 대입
	}

	T Dequeue() {
		int index = front; // index에 전단의 인덱스 대입
		front = (front + 1) % capacity;
		return data[index]; // 제외되는 데이터를 반환한다.
	}

	int GetSize() {
		// 전단의 인덱스가 후단의 인덱스와 같거나 그보다 작다면
		// 후단의 인덱스에서 전단의 인덱스를 뺀값을 반환
		if (front <= rear) {
			return rear - front;
		}
		// 전단의 인덱스가 후단의 인덱스보다 크다면
		else
			// 용량에서 전단의 인덱스를 뺀 뒤에 후단의 인덱스를 더한 값을 반환
			return capacity - front + rear; 
	}

	bool IsEmpty() {
		return front == rear; 
	}

	bool IsFull() {
		return front == (rear + 1) % capacity;
	}

	int GetRear() { return rear; }
	int GetFront() { return front; }

	vector<T> GetTotalData() {
		int tempFront = front;
		vector<T> result;
		while (tempFront != rear) {
			result.push_back(data[tempFront]);
			tempFront = (tempFront + 1) % capacity;
		}
		return result;
	}

	void Show() {
		int tempFront = front;
		while (tempFront != rear) {
			cout << data[tempFront] << " ";
			tempFront = (tempFront + 1) % capacity;
		}
	}
};
