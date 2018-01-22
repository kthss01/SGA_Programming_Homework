#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;

void makeStar() {
	for (int i = 0; i < 65; i++)
		cout << "*";
	cout << endl;
}

void ShowCard(int shape, int number) {
	// 문양 선택
	switch (shape) {
	case 0:
		cout << "♠";
		break;
	case 1:
		cout << "♥";
		break;
	case 2:
		cout << "◆";
		break;
	case 3:
		cout << "♣";
		break;
	}

	// 카드 선택
	switch (number) {
	case 0:
		cout << "A";
		break;
	case 10:
		cout << "J";
		break;
	case 11:
		cout << "Q";
		break;
	case 12:
		cout << "K";
		break;
	default:
		cout << number + 1;
		break;
	}
	
	if (number < 6)
		cout << "(Low) \t";
	else if (number > 6)
		cout << "(High) \t";
	else
		cout << "(Seven) \t";
}

void Shuffle(int* card) {
	srand(time(NULL));

	int dest, sour, temp;

	for (int i = 0; i < 1000; i++) {
		sour = rand() % 52;
		dest = rand() % 52;

		temp = card[dest];
		card[dest] = card[sour];
		card[sour] = temp;
	}
}

// 참조로 하이 로우 세븐을 매개변수로 받아 카드 카운팅
void CardCounting(int* counts, int card) {
	// 로우 카운팅
	if (card % 13 < 6) {
		counts[0]--;
	}
	// 하이 카운팅
	else if (card % 13 > 6) {
		counts[1]--;
	}
	// 세븐 카운팅
	else {
		counts[2]--;
	}
}

void ShowCounting(int* counts) {
	float sum = 0;
	// 남아있는 로우 하이 세븐 갯수 더하기
	for (int i = 0; i < 3; i++)
		sum += counts[i];

	cout << endl << endl;;
	cout << "\t로우 하이 세븐 확률 계산" << endl;
	for (int i = 0; i < 3; i++) {
		float prob = (float)counts[i] / sum * 100;

		switch (i) {
		case 0:
			cout << "\t로우 : " << prob << "% (" << counts[i] << "장)"<< endl;
			break;
		case 1:
			cout << "\t하이 : " << prob << "% (" << counts[i] << "장)" << endl;
			break;
		case 2:
			cout << "\t세븐 : " << prob << "% (" << counts[i] << "장)" << endl;
			break;
		}
	}

	cout << endl;
}

// 확률에 근거하여 자동으로 로우 하이 세븐을 선택하는 함수
// 로우 하이 세븐 중 가장 확률이 높은 쪽을 택하여 반환
int AutoSelectLHS(int* counts) {
	int larger;
	// 로우 하이 중 더 확률이 높은 쪽 선택
	if (counts[0] > counts[1])
		larger = 0; // 로우
	else
		larger = 1; // 하이

	int max;
	// 선택된 쪽과 세븐 중 더 확률이 높은 쪽 선택
	if (counts[2] > counts[larger])
		max = 2;
	else
		max = larger;

	return max;
}

void main() {
	int card[52];
	int shape[4];
	int number[4];

	int selectBetting; // 배팅 할지 선택을 위한 변수
	int selectLHS; // 로우 하이 세븐 선택을 위한 변수
	int betting;
	int money = 10000;
	int hightestMoney = money; // 최대 도달한 금액
	int gameCount = 0;
	int hightestGameCount = gameCount; // 최대 금액에 도달했던 게임판
	int turn = 0;

	int counts[3];; // 로우 하이 세븐 카운팅을 위한 변수 
	counts[0] = 24;	// low 갯수 a ~ 6 * 4 = 24
	counts[1] = 24;	// high 갯수 8 ~ k * 4 = 24
	counts[2] = 4;	// sevn 갯수 7 * 4 = 24

	// 초기화
	for (int i = 0; i < 52; i++) {
		card[i] = i;
	}

	// 셔플
	// 카드 배열의 포인터로 배열을 함수의 인자로 넘겨준다.
	Shuffle(card);

	// 게임 플레이
	while (true) {
		makeStar();

		cout << gameCount << "번째 게임" << endl;

		cout << endl;
		for (int i = 0; i < 4; i++) {
			shape[i] = card[i + turn] / 13; // 문양 0 ~ 3
			number[i] = card[i + turn] % 13; // 숫자 0 ~ 12

			// 카드 카운팅 함수
			if(i != 3)
				CardCounting(counts, number[i]);

			// 치트
			if (i == 3)
				cout << "치트 : ";

			// 문양과 숫자 보여주는 카드 함수
			ShowCard(shape[i], number[i]);
		}
		cout << endl << endl;

		// 소지금 보여주고 배팅 선택하여 입력받기
		cout << "소지금 : " << money;
		
		cout << "\t배팅하시겠습니까? ( 배팅(1), 종료(0) ) : ";
		
		// 자동화를 위해 무조건 배팅하도록 설정
		selectBetting = 1;
		cout << 1 << endl;
		//cin >> selectBetting;
		
		if (selectBetting == 1) {
			// 확률 보여주는 함수
			ShowCounting(counts);

			cout << "배팅값 입력 : ";
			
			// 자동화를 위해 랜덤한 값 배팅하도록 설정
			betting = (rand() % money / 1000 + 1) * 1000;
			cout << betting << endl;
			//cin >> betting;

			if (betting < 1000 || betting > money) {
				cout << "배팅값 입력을 다시 해주세요" << endl;
				continue;
			}

			cout << "어디의 배팅하시겠습니까? ( 로우(0), 하이(1), 세븐(2) ) : ";
			
			// 자동화를 위해 치트가 아닌 확률에 근거하여 배팅하도록 설정
			selectLHS = AutoSelectLHS(counts);
			cout << endl;
			//cin >> selectLHS;

			if (selectLHS > 2) {
				cout << "다시 입력해주세요" << endl;
				continue;
			}

			cout << endl;
			switch (selectLHS) {
			case 0:
				cout << "로우의 " << betting << " 배팅을 하셨습니다.";
				break;
			case 1:
				cout << "하이의 " << betting << " 배팅을 하셨습니다.";
				break;
			case 2:
				cout << "세븐의 " << betting << " 배팅을 하셨습니다.";
				break;
			}
			cout << endl;

			// 빠른 자동화를 위해 슬립 제거
			//Sleep(1000);

			// 하이 로우 세븐 체크

			CardCounting(counts, number[3]);
			cout << "카드 오픈 ";
			ShowCard(shape[3], number[3]);

			// 로우(0) 성공
			if (number[3] < 6 && selectLHS == 0) {
				cout << "로우 배팅 성공 +" << betting << endl;
				money += betting;
			}
			// 하이(1) 성공
			else if (number[3] > 6 && selectLHS == 1) {
				cout << "하이 배팅 성공 +" << betting << endl;
				money += betting;
			}
			// 세븐(2) 성공
			else if(number[3] == 6 && selectLHS == 2) {
				cout << "세븐 배팅 성공 +" << betting << " * 13" << endl;
				money += betting * 13; // 13배 획득
			}
			// 세븐(2) 실패
			else if (selectLHS == 2) {
				cout << "세븐 배팅 실패 -" << betting << " * 13" << endl;
				money -= betting * 13;
			}
			else {
				cout << "배팅 실패 -" << betting << endl;
				money -= betting;
			}

			if (money > hightestMoney) {
				hightestMoney = money;
				hightestGameCount = gameCount;
			}
			
		} 
		else if (selectBetting == 0) {
			cout << "\n게임을 종료 합니다." << endl;
			cout << "총 획득 금액 : " << money << endl;
			cout << "최대 도달 금액 : " << hightestMoney << "(" << hightestGameCount << "번째 게임)" << endl;
			break;
		}

		if (money < 1000) {
			cout << "\n" << gameCount << "번째 게임에서 파산하였습니다." << endl;
			cout << "최대 도달 금액 : " << hightestMoney << " (" << hightestGameCount << "번째 게임)" << endl;
			break;
		}

		if (gameCount == 100) {
			cout << "\n100번째 게임까지 살아 남았습니다." << endl;
			cout << "최대 도달 금액 : " << hightestMoney << " (" << hightestGameCount << "번째 게임)" << endl;
			break;
		}

		// 자동확 확인을 위해 주석처리
		//system("cls");

		turn += 4;
		// test
		//cout << turn << "턴" << endl;
		// 카드 모두 소진 시 다시 셔플하여 시작
		if (turn == 52) {
			turn = 0;
			Shuffle(card);
			// 카운트 초기화
			counts[0] = 24;
			counts[1] = 24;
			counts[2] = 4;
		}
		gameCount++;
	}
}