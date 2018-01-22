#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>

using namespace std;

// 구분선을 만들기 위한 함수
void makeStar() {
	for (int i = 0; i < 40; i++)
		cout << "*";
	cout << endl;
}
// 숫자로 이루어진 카드를 문양과 숫자로된 문자열로 만드는 함수
string showCard(int num) {
	string card;
	string cardType[4] = { "♠","♥","♣","◆" };
	int type, cardNum;

	/*
	스페이드	하트	다이아몬드	클럽 순으로 구분
	0~12		13~25	26~38		39~51
	*/

	// 이거 그냥 나누거나 나머지 계산하면 한 줄로 가능
	cardNum = num % 13;
	type = num / 13;

	/*
	// 스페이드
	if (num <= 12) {
		type = 0;
		cardNum = num;
	}
	// 하트
	else if (num <= 25) {
		type = 1;
		cardNum = num - 13;
	}
	// 다이아몬드
	else if (num <= 38) {
		type = 2;
		cardNum = num - 26;
	}
	// 클럽
	else {
		type = 3;
		cardNum = num - 39;
	}
	*/

	card = cardType[type];

	if (cardNum == 0)
		card += "A";
	else if (cardNum == 10)
		card += "J";
	else if (cardNum == 11)
		card += "Q";
	else if (cardNum == 12)
		card += "K";
	else
		// to_string(숫자) 숫자를 문자열로 바꿔주는 함수
		card += to_string(cardNum+1);

	// test
	//cout << card << endl;

	return card;
}

int cheat(int card1, int card2, int card3) {
	// 테스트를 위한 치트
	// 카드가 사이에 있는지 확인
	int min, max;
	if (card1 % 13 <= card3 % 13) {
		min = card1 % 13;
		max = card3 % 13;
	}
	else {
		min = card3 % 13;
		max = card1 % 13;
	}

	// 카드가 사이에 있으면 배팅
	if (min <= card2 % 13 && card2 % 13 <= max) 
		return 1;
	// 아닌 경우 패스
	else
		return 0;
}

void main()
{
	int money = 5000;
	int card[52];

	// 카드 만들기
	/*
		스페이드	하트	다이아몬드	클럽 순
		0~12		13~25	26~38		39~51
	*/
	for (int i = 0; i < 52; i++) {
		card[i] = i;
	}

	// test
	cout << "카드 보여주기 테스트" << endl;
	for (int i = 0; i < 52; i++) {
		cout << showCard(card[i]) << " ";
		if (i % 13 == 12)
			cout << endl;
	}
	cout << endl;

	// 셔플
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		int sour, dest, temp;
		sour = rand() % 52;
		dest = rand() % 52;
		
		temp = card[dest];
		card[dest] = card[sour];
		card[sour] = temp;
	}

	//test
	cout << "카드 셔플 후 테스트" << endl;
	for (int i = 0; i < 52; i++) {
		cout << showCard(card[i]) << " ";
		if (i % 3 == 2 || i == 51)
			cout << endl;
	}
	cout << endl;

	int GameCount = 0;

	while (true) {
		cout << GameCount << "번째 게임" << endl;
		cout << "\t\t현재 금액 : " << money << endl;

		int table[3];
		// 섞인 카드로부터 세 장을 뽑는다.
		for (int i = 0; i < 3; i++) {
			table[i] = card[i + GameCount * 3];
		}

		cout << endl;
		cout << "\t" << showCard(table[0]) << "\t??\t"
			<< showCard(table[2]) << endl;
		cout << endl;

		cout << "배팅하실꺼면 1번 패스는 2번 : " << endl;
		int select;
		//cin >> select;
		select = cheat(table[0], table[1], table[2]);

		if (select == 1) {
			cout << "배팅을 시작합니다." << endl;
			int bet;
			cout << "배팅액 입력 : ";
			//cin >> bet;
			// 치트 - 배팅값을 입력받지 않고 랜덤으로 배팅
			bet = (rand() % (money / 1000) + 1) * 1000;
			cout << "배팅액 : " << bet << endl;

			if (bet < 1000) {
				cout << "배팅액은 최소 1000 이상 가능합니다." << endl;
				continue;
			}
			else if (bet > money) {
				cout << "보유 금액을 넘어서 배팅 할 수 없습니다" << endl;
				continue;
			}

			cout << endl;
			cout << "오픈 :\t";

			for (int i = 0; i < 3; i++) {
				cout << showCard(table[i]) << "\t";
			}
			cout << endl;
			cout << endl;

			// 왼쪽과 오른쪽 카드 중 작은값과 큰 값을 구한다.
			/*
				5 10 이런 식 -> min = 5,  max = 10
				10 5 이런 식 -> min = 10, max = 5
			*/
			int min, max;
			if (table[0] % 13 <= table[2] % 13) {
				min = table[0] % 13;
				max = table[2] % 13;
			}
			else {
				min = table[2] % 13;
				max = table[0] % 13;
			}

			// 문양에 관계 없이 숫자만을 비교하기 위해 나머지를 구한다
			
			/*
			스페이드	하트		다이아몬드		클럽 순
			0~12		13~25		26~38			39~51
			0~12		13~25 % 13  26~38 % 13		39~51 % 13
						= 0~12		= 0~12			= 0~12
			*/
			if (min <= table[1] % 13 && table[1] % 13 <= max) {
				cout << "숫자가 사이에 있습니다. 배팅액 +" << bet << endl;
				money += bet;
			}
			else {
				cout << "숫자가 사이에 있지 않습니다. 배팅액 -" << bet << endl;
				money -= bet;
			}

			if (money <= 0) {
				cout << "금액이 모두 떨어졌습니다." << endl;
				break;
			}
		}
		else {
			cout << "패스합니다. 다음 게임으로 넘어갑니다." << endl;
		}

		if (GameCount == 16) {
			cout << "카드가 모두 소진되어 게임이 종료 되었습니다." << endl;
			cout << "최종 획득 금액 : " << money << endl;
			break;
		}
		makeStar();
		GameCount++;
	}
}

/*
	월남뽕 게임

	카드 총 52장
	카드 3장을 뽑고
	첫장과 마지막 장을 보여준 후
	베팅을 한 다음 
	가운데 카드가 안에 있는지를 맞추는 게임
	a ~ 10 j q k
	0~ 13
*/

/*
	문제점 몇 개 발견 -> 수정 완료
	1~10 사이로 해야되는데
	10 5 1 이런식일때는 체크 안됨

	배팅액 1000이상 가능임
*/