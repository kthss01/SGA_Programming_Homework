// 1.16(화) Day 4
// 과제 : 숫자 야구 게임 만들기
// 중복되지 않는 숫자

#include <iostream>
#include <time.h>

using namespace std;

void main() {
	srand(time(NULL));

	int GameCount = 0;

	int number[10];
	int baseBallNumber[4];
	int selectNumber[4];

	for (int i = 0; i < 10; i++)
		number[i] = i ;

	// 셔플
	for (int i = 0; i < 1000; i++)
	{
		int des, src, temp;
		des = rand() % 10;
		src = rand() % 10;

		temp = number[des];
		number[des] = number[src];
		number[src] = temp;
	}

	// 테스트
	//for (int i = 0; i < 10; i++)
	//	cout << number[i] << " ";
	//cout << endl;

	// 셔플된 숫자 중 4개를 야구 숫자로 선택
	for (int i = 0; i < 4; i++)
		baseBallNumber[i] = number[i];

	while (true) {

		cout << GameCount << "번째 게임" << endl;
		
		// 치트
		cout << "[치트] 정답 : ";
		for (int i = 0; i < 4; i++)
			cout << baseBallNumber[i] << " ";
		cout << endl;

		int strike, ball;
		strike = ball = 0;

		bool isCheck = false;
		cout << "\t중복되지 않는 4개의 숫자를 입력하세요 : ";

		// 4개의 입력값 받기
		for (int i = 0; i < 4; i++) {
			cin >> selectNumber[i];
		}

		// 입력값 예외 처리
		// 잘못 입력된 예외
		for (int i = 0; i < 4; i++) {
			if (selectNumber[i] < 0 || selectNumber[i] > 9) {
				cout << "\t잘 못 입력하였습니다. 다시 입력하세요" << endl;
				isCheck = true;
				break;
			}
		}
		if (isCheck == false) {
			// 중복된 예외 처리
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					if (j == k)
						continue;
					else if (selectNumber[j] == selectNumber[k]) {
						cout << "\t중복된 값이 있습니다. 다시 입력하세요." << endl;
						isCheck = true;
						break;
					}
				}
				if (isCheck == true)
					break;
			}
		}

		if (isCheck == true) {
			continue;
		}

		// 선택한 숫자 비교
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (baseBallNumber[i] == selectNumber[j]) {
					if (i == j)
						strike++;
					else
						ball++;
				}
			}
		}

		cout << "\t게임 결과 : " << "스트라이크 " << strike << ","
			<< "볼 " << ball << ", " << "아웃 " << 4 - strike - ball << endl;
		
		if (strike == 4) {
			cout << "\t게임에서 승리하였습니다." << endl;
			break;
		}

		GameCount++;
		cout << endl;

		// 10번 이상시 패배
		if (GameCount > 10) {
			cout << "패배 하였습니다." << endl;
			break;
		}

	}
}

// int 에 char 형 cin으로 받을시 안들어가는 거 같음 
// 해결 방안 찾아보자