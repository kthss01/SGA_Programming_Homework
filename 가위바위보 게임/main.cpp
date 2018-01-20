// 1.15(월) Day 3
// 과제 : 반복문과 조건문, rand()를 이용한 가위 바위 보 게임

// 진행 방식
// 플레이어 2명 1명은 나, 한명은 컴퓨터
// 가위(0), 바위(1), 보(2)
// 나는 하나 선택
// 총 5번 플레이
// 매 플레이마다 누가 어떻게 이겼다. 라는 문구 출력
// ex) 사용자가 가위로 컴퓨터를 이겼다
// 최종적으로 이긴 횟수, 진 횟수, 비긴 횟수 출력
// 카페에 스샷찍어서 올리기

#include <iostream>
#include <time.h>
using namespace std;

void makeCover(int star) {
	for (int i = 0; i < star; i++) {
		cout << "*";
	}
	cout << endl;
}

void main()
{
	int star = 49;
	bool isSelect = false;
	int player;

	makeCover(star);

	cout << "*\t\t가위 바위 보 게임\t\t*" << endl;

	makeCover(star);

	while (isSelect == false) {
		cout << "가위 바위 보 중 하나를 입력하세요" << endl;
		cout << "숫자키 가위(0), 바위(1), 보(2)" << endl;
		cout << "입력 : ";
		
		cin >> player;
		cout << endl;

		switch (player) {
		case 0:
			cout << "[가위(0)]를 입력 하셨습니다." << endl;
			isSelect = true;
			break;
		case 1:
			cout << "[바위(1)]를 입력 하셨습니다." << endl;
			isSelect = true;
			break;
		case 2:
			cout << "[보(2)]를 입력 하셨습니다." << endl;
			isSelect = true;
			break;
		default:
			cout << "잘못 입력하셨습니다. 다시 입력하세요" << endl;
			cout << "입력하신 숫자는 " << player << "입니다." << endl;
			break;
		}
	}

	cout << endl;

	makeCover(star);
	
	cout << "*\t\t게임을 시작합니다.\t\t*" << endl;

	makeCover(star);

	srand(time(NULL));

	// 플레이 횟수
	int num = 5;
	int win, lose, draw;
	win = lose = draw = 0;
	for (int i = 0; i < num; i++) {
		cout << i + 1 << "번째 게임" << endl;
		
		// 컴퓨터 상태
		int com = rand() % 3;
		cout << "\t컴퓨터는 ";
		
		switch (com) {
		case 0:
			cout << "가위(0)를 냈습니다." << endl;
			break;
		case 1:
			cout << "바위(1)를 냈습니다." << endl;
			break;
		case 2:
			cout << "보(2)를 냈습니다." << endl;
			break;
		}

		cout << "\t당신은... ";

		// draw
		if (com == player) {
			cout << "비겼습니다." << endl;
			draw++;
		}
		// player win
		// 컴 보(2), 플레이어 가위(2)일 때 와 숫자 클 때 이김
		else if (com < player || (com == 2 && player == 0)) {
			cout << "이겼습니다." << endl;
			win++;
		}
		// lose
		else {
			cout << "졌습니다." << endl;
			lose++;
		}
	}

	makeCover(star);
	cout << "*\t\t게임 결과\t\t\t*" << endl;
	cout << "*\t\t" << win << "승, " << lose << "패, " << draw << "무" << "\t\t\t*" << endl;
	makeCover(star);
}