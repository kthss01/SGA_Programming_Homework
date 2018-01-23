#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;

typedef struct Storage {
	int board[5][5];
	int bingo;
	int gameCount;
} Store;

// 빙고판 출력하는 함수
// 2차원 배열 포인터로 받은거임
void showBoard(int board[5][5]);
// 빙고판 변경하는 함수
void changeBoard(int board[5][5], int number);

// 빙고 상태확인 하는 함수
int checkBoard(int board[5][5]);
// 빙고판에 숫자가 있는지 확인하는 함수
bool checkNumber(int board[5][5], int number);

// 빙고 상태를 저장소 구조체의 저장하는 함수
void saveBoard(Store* store, int* storeSize, int bingo, int board[5][5], int gameCount);
// 빙고 상태를 저장소 구조체로부터 읽어오는 함수
void loadBoard(Store* store, int storeSize);

// 2차원 배열 변경시 반영되는지 테스트
// 2차원 배열 변경시 반영됨
void testBoard(int board[5][5]) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			board[i][j] = 0;
		}
	}
}

void main() {
	int board[5][5];
	int number[25];

	bool isBoardNumber; // 빙고판에 숫자가 있는지 확인하는 변수

	int selectNumber;

	int bingo = 0;
	int oldBingo = 0; // 이전 빙고값을 저장하는 변수
	int gameCount = 0;

	Store store[5];
	int storeSize = 0;

	// 숫자 초기화
	for (int i = 0; i < 25; i++)
		number[i] = i + 1;

	// 셔플
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		int sour = rand() % 25;
		int dest = rand() % 25;

		int temp = number[dest];
		number[dest] = number[sour];
		number[sour] = temp;
	}

	// 셔플된 숫자 빙고판에 입력
	int cnt = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			board[i][j] = number[cnt];
			cnt++;
		}
	}

	// 빙고판 출력하는 함수
	//showBoard(board);

	// 2차원 배열 넘길시 변하는지 테스트
	//testBoard(board);
	//showBoard(board);
	
	// 게임 시작
	while (true) {
		system("cls");

		cout << gameCount << "번째 게임" << endl;

		// 숫자 입력
		cout << "숫자를 입력하세요 : ";
		// 자동화를 위해 gameCount+1을 그냥 넣어줌
		selectNumber = gameCount + 1;
		//cin >> selectNumber;
		
		// 숫자 입력 예외 처리
		if (selectNumber > 26 || selectNumber <= 0) {
			cout << "잘 못 입력했습니다. 다시 입력하세요" << endl;
			Sleep(1000);
			continue;
		}

		isBoardNumber = checkNumber(board, selectNumber);
		if (isBoardNumber == false) {
			cout << "이미 입력된 숫자입니다. 다시 입력하세요" << endl;
			Sleep(1000);
			continue;
		}

		// 빙고판 변경
		changeBoard(board, selectNumber);

		// 빙고판 출력
		cout << endl;
		showBoard(board);
		cout << endl;

		// 빙고 확인
		oldBingo = bingo;
		bingo = checkBoard(board);

		if (bingo >= 5) {
			cout << "5 빙고 완성, 게임 승리" << endl;
			
			// 저장해두었던 빙고 상태 읽어오기
			loadBoard(store, storeSize);

			break;
		}
		else if (bingo != oldBingo) {
			cout << bingo << " 빙고 완성, " << 5 - bingo << " 빙고 남았습니다." << endl;
			Sleep(1000);	// 자동으로 돌리면 sleep 동안에도 진행중이라 위에 문구 안보이는거

			// 잠깐 동안 멈추기위해 입력값 기다리는 방법
			//getchar(); // endl 남아있어서 한번더 입력받은거
			//getchar();

			// 빙고 변화가 있으면 빙고판과 게임 횟수 저장하기
			saveBoard(store, &storeSize, bingo, board, gameCount);
		}

		gameCount++;
	}
}
// 빙고판 출력하는 함수
void showBoard(int board[5][5]) {

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (board[i][j] < 10)
				cout << " ";

			// 이미 입력된 숫자는 -1로 변경되어있으므로 X로 출력
			if (board[i][j] == -1) 
				cout << "X" << " ";
			else
				cout << board[i][j] << " ";
		}
		cout << endl;
	}
}
// 빙고판 변경하는 함수
void changeBoard(int board[5][5], int number) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			// 입력된 숫자를 찾아 -1로 변경
			if (number == board[i][j])
				board[i][j] = -1;
		}
	}
}
// 빙고 상태확인 하는 함수
int checkBoard(int board[5][5]) {
	int rowSum, colSum, diagSum1, diagSum2;
	rowSum = colSum = diagSum1 = diagSum2 = 0;

	int bingo = 0;

	// 열 빙고와 대각선 빙고 확인
	for (int i = 0; i < 5; i++) {
		colSum = 0;
		for (int j = 0; j < 5; j++) {
			colSum += board[i][j];
			if (i == j)
				diagSum1 += board[i][j];
		}
		// -1로 된 입력된 값 5개가 모두 있을 경우 -5이므로 빙고
		if (colSum == -5)
			bingo++;
	}

	if (diagSum1 == -5)
		bingo++;

	// 행 빙고와 역대각선 빙고 확인
	for (int j = 0; j < 5; j++) {
		rowSum = 0;
		for (int i = 0; i < 5; i++) {
			rowSum += board[i][j];
			if (i + j == 4)
				diagSum2 += board[i][j];
		}
		if (rowSum == -5)
			bingo++;
	}

	if (diagSum2 == -5)
		bingo++;

	return bingo;
}
// 빙고판에 숫자가 있는지 확인하는 함수
bool checkNumber(int board[5][5], int number) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			// 숫자가 있으면 true 리턴
			if (board[i][j] == number) {
				return true;
			}
		}
	}
	// 숫자가 없으면 false 리턴
	return false;
}

// 빙고 상태를 저장소 구조체의 저장하는 함수
void saveBoard(Store* store, int* storeSize, int bingo, int board[5][5], int gameCount) {
	// 저장소에 빙고 상태 저장
	store[*storeSize].bingo = bingo;

	// 저장소에 빙고판 저장
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			store[*storeSize].board[i][j] = board[i][j];
		}
	}

	// 저장소에 빙고일때 게임판 수 저장
	store[*storeSize].gameCount = gameCount;

	// 스토어 인덱스 증가
	*storeSize = *storeSize + 1;
}

// 빙고 상태를 저장소 구조체로부터 읽어오는 함수
void loadBoard(Store* store, int storeSize) {
	for (int i = 0; i < 35; i++)
		cout << "*";
	cout << endl;

	cout << "빙고 기록표" << endl;

	for (int i = 0; i < storeSize; i++) {
		cout << endl;
		cout << store[i].gameCount << "번째 게임" << endl;

		cout << endl;
		showBoard(store[i].board);
		cout << endl;

		cout << store[i].bingo << " 빙고 완성, " << 5 - store[i].bingo << " 빙고 남음" << endl;
	}

	for (int i = 0; i < 35; i++)
		cout << "*";
	cout << endl;
}