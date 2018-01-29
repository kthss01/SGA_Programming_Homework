#include "Ssudda.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <string>
#include <stdio.h>

using namespace std;

Ssudda::Ssudda()
{
	SelectPlayer();
	Init();
}


Ssudda::~Ssudda()
{
}

void Ssudda::Shuffle()
{
	srand(time(NULL));

	for (int i = 0; i < 1000; i++) {
		int dest = rand() % 20;
		int sour = rand() % 20;
		int temp = card[dest];
		card[dest] = card[sour];
		card[sour] = temp;
	}
}
// 두 카드를 섯다 족보를 통해 레벨로 바꿔주는 함수
int Ssudda::CheckCombination(int * card)
{
	/*
		1, 3, 8 번째 카드가 광
	*/

	// 계산하기 편하게 레벨로 구분
	combo level;

	int card1 = card[0] % 10 + 1;
	int card2 = card[1] % 10 + 1;

	// 38광땡
	if (card[0] + 1 == 3 && card[1] + 1 == 8) {
		level = GwangTtaeng38;
	}
	// 광땡 18, 13 광땡 두개임
	else if (card[0] + 1 == 1 && card[1] + 1 == 8
		|| card[0]+1 == 1 && card[1] + 1 == 3) {
		level = GwangTtaeng;
	}
	// 장땡(10)
	else if (card1 == card2 && card1 == 10) {
		level = ZangTtaeng;
	}
	// 9땡
	else if (card1 == card2 && card1 == 9) {
		level = Ttaeng9;
	}
	// 8땡
	else if (card1 == card2 && card1 == 8) {
		level = Ttaeng8;
	}
	// 7땡
	else if (card1 == card2 && card1 == 7) {
		level = Ttaeng7;
	}
	// 6땡
	else if (card1 == card2 && card1 == 6) {
		level = Ttaeng6;
	}
	// 5땡
	else if (card1 == card2 && card1 == 5) {
		level = Ttaeng5;
	}
	// 4땡
	else if (card1 == card2 && card1 == 4) {
		level = Ttaeng4;
	}
	// 3땡
	else if (card1 == card2 && card1 == 3) {
		level = Ttaeng3;
	}
	// 2땡
	else if (card1 == card2 && card1 == 2) {
		level = Ttaeng2;
	}
	// 삥땡(1)
	else if (card1 == card2 && card1 == 1) {
		level = PingTtaeng;
	}
	// 알리 1 2
	else if (card1 == 1 && card2 == 2) {
		level = Ali;
	}
	// 독사 1 4
	else if (card1 == 1 && card2 == 4) {
		level = Dogsa;
	}
	// 구삥 1 9
	else if (card1 == 1 && card2 == 9) {
		level = GuPing;
	}
	// 장삥 1 10
	else if (card1 == 1 && card2 == 10) {
		level = ZangPing;
	}
	// 장사 4 10
	else if (card1 == 4 && card2 == 10) {
		level = ZangSa;
	}
	// 세륙 4 6
	else if (card1 == 4 && card2 == 6) {
		level = SeYug;
	}
	// 갑오 두 끗의 합 9
	else if ((card1 + card2) % 10 == 9) {
		level = Gabo;
	}
	// 8끗
	else if ((card1 + card2) % 10 == 8) {
		level = Keut8;
	}
	// 7끗
	else if ((card1 + card2) % 10 == 7) {
		level = Keut7;
	}
	// 6끗
	else if ((card1 + card2) % 10 == 6) {
		level = Keut6;
	}
	// 5끗
	else if ((card1 + card2) % 10 == 5) {
		level = Keut5;
	}
	// 4끗
	else if ((card1 + card2) % 10 == 4) {
		level = Keut4;
	}
	// 3끗
	else if ((card1 + card2) % 10 == 3) {
		level = Keut3;
	}
	// 2끗
	else if ((card1 + card2) % 10 == 2) {
		level = Keut2;
	}
	// 1끗
	else if ((card1 + card2) % 10 == 1) {
		level = Keut1;
	}
	// 망통 두 끗의 합 0
	else
		level = MangTong;

	return level;
}

string Ssudda::ShowLevelToCard(int level)
{
	string card;

	switch (level)
	{
	case Ssudda::GwangTtaeng38:
		card = "38광땡";
		break;
	case Ssudda::GwangTtaeng:
		card = "광땡";
		break;
	case Ssudda::ZangTtaeng:
		card = "장땡";
		break;
	case Ssudda::Ttaeng9:
		card = "9땡";
		break;
	case Ssudda::Ttaeng8:
		card = "8땡";
		break;
	case Ssudda::Ttaeng7:
		card = "7땡";
		break;
	case Ssudda::Ttaeng6:
		card = "6땡";
		break;
	case Ssudda::Ttaeng5:
		card = "5땡";
		break;
	case Ssudda::Ttaeng4:
		card = "4땡";
		break;
	case Ssudda::Ttaeng3:
		card = "3땡";
		break;
	case Ssudda::Ttaeng2:
		card = "2땡";
		break;
	case Ssudda::PingTtaeng:
		card = "삥땡";
		break;
	case Ssudda::Ali:
		card = "알리";
		break;
	case Ssudda::Dogsa:
		card = "독사";
		break;
	case Ssudda::GuPing:
		card = "구삥";
		break;
	case Ssudda::ZangPing:
		card = "장삥";
		break;
	case Ssudda::ZangSa:
		card = "장사";
		break;
	case Ssudda::SeYug:
		card = "세륙";
		break;
	case Ssudda::Gabo:
		card = "갑오";
		break;
	case Ssudda::Keut8:
		card = "여덟끗";
		break;
	case Ssudda::Keut7:
		card = "일곱끗";
		break;
	case Ssudda::Keut6:
		card = "여섯끗";
		break;
	case Ssudda::Keut5:
		card = "다섯끗";
		break;
	case Ssudda::Keut4:
		card = "네끗";
		break;
	case Ssudda::Keut3:
		card = "세끗";
		break;
	case Ssudda::Keut2:
		card = "두끗";
		break;
	case Ssudda::Keut1:
		card = "한끗";
		break;
	case Ssudda::MangTong:
		card = "망통";
		break;
	}

	return card;
}

void Ssudda::SelectPlayer()
{
	while (true) {
		cout << "플레이할 사용자를 입력하세요 (2 ~ 4) : ";
		cin >> playerNumber;

		if (playerNumber >= 2 && playerNumber <= 4) {
			// 플레이어들에 초기 금액 설정하기
			for (int i = 0; i < playerNumber; i++) {
				player[i].SetMoney(STARTMONEY);
			}

			break;
		}
		else {
			cout << "다시 입력하세요" << endl;
		}
	}
}

void Ssudda::Init()
{
	for (int i = 0; i < 20; i++)
		card[i] = i;

	Shuffle();

	tableMoney = 0;		// 테이블 금액 초기화
}

void Ssudda::Release()
{
}

void Ssudda::Update()
{
	system("cls");

	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << endl;

	cout << "\t\t      섯다" << endl;
	cout << "\t\t\t\t플레이 인원 : " << playerNumber << "명" << endl;

	int draw = 0;

	// 플레이어들에게 카드 나눠주기
	for (int i = 0; i < playerNumber; i++) {
		player[i].SetCard(card[draw], card[draw + 1]);
		draw += 2;

		// test
		//player[0].SetCard(2, 7);

		// 플레이어들 모두 올인하도록 설정
		player[i].SetBetting(player[i].GetMoney());
		// 플레이어들 배팅 금액, 테이블 금액에 더하기
		tableMoney += player[i].GetBetting();

		// 섯다 족보를 통해 플레이어 레벨 구분하기
		player[i].SetLevel(CheckCombination(player[i].GetCard()));
	}

	// 화면에 카드 보여주기
	Render();

	/*
		승패 계산부분과 금액 계산부분 (배팅액에 따라 달라지게)
		수정하면 좋을듯 
		무승부에 대한 처리도
	*/

	bool isDraw = false;
	int winLevel = INT_MAX;
	int winner;
	// 승패 계산 , 레벨이 제일 낮은 플레이어가 승자
	for (int i = 0; i < playerNumber; i++) {
		// 레벨(족보)이 같은 경우 무승부
		if (winLevel == player[i].GetLevel())
			isDraw = true;
		// 레벨이 낮을 수록 승자 (제일 낮은게 38광땡)
		if (player[i].GetLevel() < winLevel) {
			winLevel = player[i].GetLevel();
			winner = i;
		}
	}

	if (isDraw == false) {
		// 금액 계산
		// 승자는 테이블의 모든 금액만큼 획득
		int money = player[winner].GetMoney();
		player[winner].SetMoney(money + tableMoney);

		// 승자 표시 및 플레이어들 금액 보여주기
		for (int i = 0; i < playerNumber; i++) {
			if (i == winner) {
				cout << "플레이어" << winner + 1 << " 승리 ";
				cout << "소지금 : " << player[winner].GetMoney();
				cout << " (승 +" << tableMoney << ")" << endl;
			} 
			else {
				cout << "플레이어" << i + 1 << " 패배 ";
				cout << "소지금 : " << player[i].GetMoney();
				cout << " (패 -" << player[i].GetBetting() << ")" << endl;
			}
		}
	}
	else {
		cout << "무승부 입니다." << endl;
		// 배팅된 금액 돌려받기
		for (int i = 0; i < playerNumber; i++) {
			// 플레이어의 금액
			int money = player[i].GetMoney();
			// 플레이어의 배팅액
			int bet = player[i].GetBetting();
			player[i].SetMoney(money + bet);
		}

		// 플레이어들 금액 보여주기
		for (int i = 0; i < playerNumber; i++) {
			cout << "플레이어" << i + 1 << " 무승부 ";
			cout << "소지금 : " << player[winner].GetMoney();
			cout << " (무 +" << player[i].GetBetting() << ")" << endl;
		}
	}

}

void Ssudda::Render()
{
	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << endl;

	for (int i = 0; i < playerNumber; i++) {
		/*cout << "플레이어 " << i + 1 << " 의 카드 : ";
		cout << player[i].GetCard()[0] % 10 + 1 << " " << player[i].GetCard()[1] % 10 + 1 << " ";
		cout << "(" << ShowLevelToCard(player[i].GetLevel()) << ") ";
		cout << "배팅액 : " << player[i].GetBetting() << endl;*/

		// printf함수를 통해 포맷을 정하여 콘솔에 출력
		printf("플레이어%d ", i + 1);
		
		if (player[i].GetMoney() == 0)
			printf("(All-in):");
		else
			printf("(%6d):", player[i].GetMoney());

		printf(" %2d %2d (%6s) 배팅액 : %d\n",
			player[i].GetCard()[0] % 10 + 1,
			player[i].GetCard()[1] % 10 + 1,
			ShowLevelToCard(player[i].GetLevel()).c_str(),
			player[i].GetBetting());
	}
	cout << "\n테이블 금액 : " << tableMoney << endl;

	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << endl;

}
