#include <iostream>
#include <conio.h>
#include <time.h>
#include <Windows.h>

using namespace std;

void shuffle(int* puzzle) {
	// 셔플
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		int dest = rand() % 8;
		int sour = rand() % 8;

		int temp = puzzle[dest];
		puzzle[dest] = puzzle[sour];
		puzzle[sour] = temp;
	}
}

// 셔플로 퍼즐 만드는 함수
void makePuzzle(int* puzzle) {
	shuffle(puzzle);
}

void makeLine() {
	for (int i = 0; i < 35; i++) {
		cout << "=";
	}
	cout << endl;
}

void showPuzzle(int* puzzle, int gameCount) {
	makeLine();
	cout << "\t슬라이딩 퍼즐 게임" << endl;
	makeLine();

	cout << endl;
	// 퍼즐판 보여주기
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0)
			cout << "\t";

		if (puzzle[i] == -1)
			cout << "X\t";
		else
			cout << puzzle[i] << "\t";
		if ((i + 1) % 3 == 0)
			cout << "\n\n";
	}
	cout << endl;

	makeLine();
	cout << "\t\t움직인 횟수 : " << gameCount << endl;
	cout << endl;
	//makeLine();
}

// X의 위치를 찾아주는 함수
int findX(int* puzzle) {
	for (int i = 0; i < 9; i++) {
		if (puzzle[i] == -1)
			return i;
	}
}

// X를 움직여주는 함수
void movePuzzle(char move, int* puzzle, int* moveCount) {
	int x = findX(puzzle);

	/*
		0	1	2
		3	4	5
		6	7	8
	
		4 기준 
		위로 -3
		아래로 +3
		왼쪽 -1
		오른쪽 +1

		끝인 경우는 안되게
	*/

	switch (move) {
	case 'w':
	case 'W':
		//cout << "Up" << endl;
		if (x > 2)
			swap(puzzle[x], puzzle[x - 3]);
		*moveCount += 1;
		break;
	case 's':
	case 'S':
		//cout << "Down" << endl;
		if (x < 6)
			swap(puzzle[x], puzzle[x + 3]);
		*moveCount += 1;
		break;
	case 'a':
	case 'A':
		//cout << "Left" << endl;
		if (x % 3 != 0)
			swap(puzzle[x], puzzle[x - 1]);
		*moveCount += 1;
		break;
	case 'd':
	case 'D':
		//cout << "Right" << endl;
		if (x % 3 != 2)
			swap(puzzle[x], puzzle[x + 1]);
		*moveCount += 1;
		break;
	default:
		break;
	}
}

// 랜덤하게 이동하는 방법으로 퍼즐 만드는 함수
void makePuzzle2(int* puzzle, char* moveArray, int* moveCount) {
	srand(time(NULL));
	int move = rand() % 11 + 5; // 몇번 이동으로 문제를 만들지 랜덤으로 결정

	for (int i = 0; i < move; ) {
		int direction = rand() % 4;
		switch (direction) {
		case 0:
			// up
			// 맨 위에 위치한 경우는 무시
			if (findX(puzzle) <= 2)
				continue;
			moveArray[*moveCount] = 's';
			movePuzzle('w', puzzle, moveCount);
			break;
		case 1:
			// down
			// 맨 아래에 위치한 경우 무시
			if (findX(puzzle) >= 6)
				continue;
			moveArray[*moveCount] = 'w';
			movePuzzle('s', puzzle, moveCount);
			break;
		case 2:
			// left
			// 맨 왼쪽에 위치한 경우 무시
			if (findX(puzzle) % 3 == 0)
				continue;
			moveArray[*moveCount] = 'd';
			movePuzzle('a', puzzle, moveCount);
			break;
		case 3:
			// right;
			// 맨 오른쪽에 위치한 경우 무시
			if (findX(puzzle) % 3 == 2)
				continue;
			moveArray[*moveCount] = 'a';
			movePuzzle('d', puzzle, moveCount);
			break;
		}
		i++;
		//showPuzzle(puzzle, *moveCount);
	}
}

void showCheat(char* moveArray, int moveCount, char* selectArray) {
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hC, &csbi);

	cout << " 치트 (총 " << moveCount <<"번 랜덤하게 움직였음)" << endl;
	for (int i = 0; i < moveCount; i++) {
		if (moveArray[moveCount - 1 - i] == selectArray[i]) {
			//continue;
			SetConsoleTextAttribute(hC, 10);	// 치트대로 친 경우 초록색으로 변경
		}
		cout << moveArray[moveCount - 1 - i] << " ";
		SetConsoleTextAttribute(hC, 7);	// 나머지 부분은 원래 색으로
	}
	cout << endl;
	makeLine();
}

// 퍼즐이 풀리는지 체크하는 함수
bool checkPuzzle(int* puzzle, int* number) {
	for (int i = 0; i < 9; i++) {
		if (puzzle[i] != number[i])
			return false;
	}
	return true;
}

void main() {
	int number[9];
	int puzzle[9];
	char moveArray[25];		// 퍼즐을 만들면서 움직인 방향을 모은 배열
	int moveCount = 0;			// 퍼즐을 만들면서 움직였던 횟수
	
	char selectMove;
	char selectArray[50] = { 0 };	// 게임을 진행하면서 움직였던 방향을 모은 배열
	int gameCount = 0;		// 움직인 횟수

	// 숫자 초기화
	for (int i = 0; i < 9; i++) {
		if (i == 8)
			number[i] = -1;
		else
			number[i] = i + 1;
		puzzle[i] = number[i];
	}

	//makePuzzle(puzzle, number);
	makePuzzle2(puzzle, moveArray, &moveCount);

	while (true) {
		showPuzzle(puzzle, gameCount);

		// 치트
		showCheat(moveArray, moveCount, selectArray);

		if (checkPuzzle(puzzle, number)) {
			cout << "\t   게임 클리어!!\n" << endl;
			cout << "\t\t총 움직인 횟수 : " << gameCount << endl;
			makeLine();
			break;
		}

		if (gameCount == 49) {
			cout << "\t   게임 오버\n" << endl;
			cout << "\t더 이상 움직일 수 없습니다." << endl;
			makeLine();
			break;
		}

		char move = getch();
		selectArray[gameCount] = move;
		movePuzzle(move, puzzle, &gameCount);

		system("cls");
	}
}