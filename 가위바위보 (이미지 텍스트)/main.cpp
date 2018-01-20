#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <windows.h>
using namespace std;

#define MAX_COUNT 5

string images[] = { "ASCII-main.txt", "ASCII-main2.txt",
"ASCII-scissors0.txt", "ASCII-scissors1.txt", "ASCII-scissors2.txt",
"ASCII-rock0.txt", "ASCII-rock1.txt", "ASCII-rock2.txt",
"ASCII-paper0.txt", "ASCII-paper1.txt", "ASCII-paper2.txt" };

int win, lose, draw;
int vs[MAX_COUNT];

void readImage(string str) {
	string s;

	ifstream in(str);
	while (!in.eof()) {
		getline(in, s);
		cout << s << endl;
	}
	in.close();
}

void compute(int player) {
	srand(time(NULL));

	for (int i = 0; i < MAX_COUNT; i++) {
		int computer = rand() % 3;
		
		// 가위
		if (player == 0) {
			// vs 가위
			if (computer == 0) {
				vs[i] = 0;
				draw++;
				readImage(images[2]);
			}
			// vs 바위
			else if (computer == 1) {
				vs[i] = -1;
				lose++;
				readImage(images[3]);
			}
			// vs 보
			else {
				vs[i] = 1;
				win++;
				readImage(images[4]);
			}
		}
		// 바위
		else if (player == 1) {
			// vs 가위
			if (computer == 0) {
				vs[i] = 1;
				win++;
				readImage(images[5]);
			}
			// vs 바위
			else if (computer == 1) {
				vs[i] = 0;
				draw++;
				readImage(images[6]);
			}
			// vs 보
			else {
				vs[i] = -1;
				lose++;
				readImage(images[7]);
			}
		}
		// 보
		else {
			// vs 가위
			if (computer == 0) {
				vs[i] = -1;
				lose++;
				readImage(images[8]);
			}
			// vs 바위
			else if (computer == 1) {
				vs[i] = 1;
				win++;
				readImage(images[9]);
			}
			// vs 보
			else {
				vs[i] = 0;
				draw++;
				readImage(images[10]);
			}
		}
	}
}

void makeScore(int color) {
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	SetConsoleTextAttribute(hC, color);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 100; j++)
			cout << " ";
		for (int j = 0; j < 100; j++) {
			cout << "*";
		}
		cout << endl;
	}

	// 원상태로
	SetConsoleTextAttribute(hC, 7);
}

void result() {
	// 빈칸
	for (int i = 0; i < 25; i++)
		cout << endl;

	/*
	// win
	for (int i = 0; i < win; i++) {
		makeScore(10);
		cout << endl;
	}
	// draw
	for (int i = 0; i < draw; i++) {
		makeScore(8);
		cout << endl;
	}
	// lose
	for (int i = 0; i < lose; i++) {
		makeScore(12);
		cout << endl;
	}
	*/

	for (int i = 0; i < MAX_COUNT; i++) {
		if (vs[i] == 1)
			makeScore(10);
		else if (vs[i] == 0)
			makeScore(8);
		else
			makeScore(12);
		cout << endl;
	}

	// 빈칸
	for (int i = 0; i < 25; i++)
		cout << endl;
}

int main() {
	
	// 테스트
	//for(int i=0; i<11; i++)
	//	readimage(images[i]);

	readImage(images[0]);
	readImage(images[1]);

	int player;
	cin >> player;
	
	while (true) {
		// 가위 바위 보만 입력 받음
		if (player >= 0 && player <= 2) {
			compute(player);
			break;
		}
	}

	result();

	return 0;
}