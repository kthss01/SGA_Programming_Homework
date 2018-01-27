#include "Ssudda.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <string>

using namespace std;

Ssudda::Ssudda()
{
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
// µÎ Ä«µå¸¦ ¼¸´Ù Á·º¸¸¦ ÅëÇØ ·¹º§·Î ¹Ù²ãÁÖ´Â ÇÔ¼ö
int Ssudda::CheckCombination(int * card)
{
	/*
		1, 3, 8 ¹øÂ° Ä«µå°¡ ±¤
	*/

	// °è»êÇÏ±â ÆíÇÏ°Ô ·¹º§·Î ±¸ºÐ
	combo level;

	int card1 = card[0] % 10 + 1;
	int card2 = card[1] % 10 + 1;

	// 38±¤¶¯
	if (card[0] + 1 == 3 && card[1] + 1 == 8) {
		level = GwangTtaeng38;
	}
	// ±¤¶¯
	else if (card[0] + 1 == 1 && card[1] + 1 == 8) {
		level = GwangTtaeng;
	}
	// Àå¶¯(10)
	else if (card1 == card2 && card1 == 10) {
		level = ZangTtaeng;
	}
	// 9¶¯
	else if (card1 == card2 && card1 == 9) {
		level = Ttaeng9;
	}
	// 8¶¯
	else if (card1 == card2 && card1 == 8) {
		level = Ttaeng8;
	}
	// 7¶¯
	else if (card1 == card2 && card1 == 7) {
		level = Ttaeng7;
	}
	// 6¶¯
	else if (card1 == card2 && card1 == 6) {
		level = Ttaeng6;
	}
	// 5¶¯
	else if (card1 == card2 && card1 == 5) {
		level = Ttaeng5;
	}
	// 4¶¯
	else if (card1 == card2 && card1 == 4) {
		level = Ttaeng4;
	}
	// 3¶¯
	else if (card1 == card2 && card1 == 3) {
		level = Ttaeng3;
	}
	// 2¶¯
	else if (card1 == card2 && card1 == 2) {
		level = Ttaeng2;
	}
	// »æ¶¯(1)
	else if (card1 == card2 && card1 == 1) {
		level = PingTtaeng;
	}
	// ¾Ë¸® 1 2
	else if (card1 == 1 && card2 == 2) {
		level = Ali;
	}
	// µ¶»ç 1 4
	else if (card1 == 1 && card2 == 4) {
		level = Dogsa;
	}
	// ±¸»æ 1 9
	else if (card1 == 1 && card2 == 9) {
		level = GuPing;
	}
	// Àå»æ 1 10
	else if (card1 == 1 && card2 == 10) {
		level = ZangPing;
	}
	// Àå»ç 4 10
	else if (card1 == 4 && card2 == 10) {
		level = ZangSa;
	}
	// ¼¼·ú 4 6
	else if (card1 == 4 && card2 == 6) {
		level = SeYug;
	}
	// °©¿À µÎ ²ýÀÇ ÇÕ 9
	else if ((card1 + card2) % 10 == 9) {
		level = Gabo;
	}
	// 8²ý
	else if ((card1 + card2) % 10 == 8) {
		level = Keut8;
	}
	// 7²ý
	else if ((card1 + card2) % 10 == 7) {
		level = Keut7;
	}
	// 6²ý
	else if ((card1 + card2) % 10 == 6) {
		level = Keut6;
	}
	// 5²ý
	else if ((card1 + card2) % 10 == 5) {
		level = Keut5;
	}
	// 4²ý
	else if ((card1 + card2) % 10 == 4) {
		level = Keut4;
	}
	// 3²ý
	else if ((card1 + card2) % 10 == 3) {
		level = Keut3;
	}
	// 2²ý
	else if ((card1 + card2) % 10 == 2) {
		level = Keut2;
	}
	// 1²ý
	else if ((card1 + card2) % 10 == 1) {
		level = Keut1;
	}
	// ¸ÁÅë µÎ ²ýÀÇ ÇÕ 0
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
		card = "ÃµÇÏ¹«Àû 38±¤¶¯";
		break;
	case Ssudda::GwangTtaeng:
		card = "±¤¶¯";
		break;
	case Ssudda::ZangTtaeng:
		card = "Àå¶¯";
		break;
	case Ssudda::Ttaeng9:
		card = "9¶¯";
		break;
	case Ssudda::Ttaeng8:
		card = "8¶¯";
		break;
	case Ssudda::Ttaeng7:
		card = "7¶¯";
		break;
	case Ssudda::Ttaeng6:
		card = "6¶¯";
		break;
	case Ssudda::Ttaeng5:
		card = "5¶¯";
		break;
	case Ssudda::Ttaeng4:
		card = "4¶¯";
		break;
	case Ssudda::Ttaeng3:
		card = "3¶¯";
		break;
	case Ssudda::Ttaeng2:
		card = "2¶¯";
		break;
	case Ssudda::PingTtaeng:
		card = "»æ¶¯";
		break;
	case Ssudda::Ali:
		card = "¾Ë¸®";
		break;
	case Ssudda::Dogsa:
		card = "µ¶»ç";
		break;
	case Ssudda::GuPing:
		card = "±¸»æ";
		break;
	case Ssudda::ZangPing:
		card = "Àå»æ";
		break;
	case Ssudda::ZangSa:
		card = "Àå»ç";
		break;
	case Ssudda::SeYug:
		card = "¼¼·ú";
		break;
	case Ssudda::Gabo:
		card = "°©¿À";
		break;
	case Ssudda::Keut8:
		card = "¿©´ü²ý";
		break;
	case Ssudda::Keut7:
		card = "ÀÏ°ö²ý";
		break;
	case Ssudda::Keut6:
		card = "¿©¼¸²ý";
		break;
	case Ssudda::Keut5:
		card = "´Ù¼¸²ý";
		break;
	case Ssudda::Keut4:
		card = "³×²ý";
		break;
	case Ssudda::Keut3:
		card = "¼¼²ý";
		break;
	case Ssudda::Keut2:
		card = "µÎ²ý";
		break;
	case Ssudda::Keut1:
		card = "ÇÑ²ý";
		break;
	case Ssudda::MangTong:
		card = "¸ÁÅë";
		break;
	}

	return card;
}

void Ssudda::Init()
{
	for (int i = 0; i < 20; i++)
		card[i] = i;

	Shuffle();

	while (true) {
		cout << "ÇÃ·¹ÀÌÇÒ »ç¿ëÀÚ¸¦ ÀÔ·ÂÇÏ¼¼¿ä (2 ~ 4) : ";
		cin >> playerNumber;

		if (playerNumber >= 2 && playerNumber <= 4) {
			// ÇÃ·¹ÀÌ¾îµé¿¡ ÃÊ±â ±Ý¾× ¼³Á¤ÇÏ±â
			for (int i = 0; i < playerNumber; i++) {
				player[i].SetMoney(STARTMONEY);
			}

			break;
		}
		else {
			cout << "´Ù½Ã ÀÔ·ÂÇÏ¼¼¿ä" << endl;
		}
	}
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

	cout << "\t\t      ¼¸´Ù" << endl;
	cout << "\t\t\t\tÇÃ·¹ÀÌ ÀÎ¿ø : " << playerNumber << "¸í" << endl;

	int draw = 0;

	// ÇÃ·¹ÀÌ¾îµé¿¡°Ô Ä«µå ³ª´²ÁÖ±â
	for (int i = 0; i < playerNumber; i++) {
		player[i].SetCard(card[draw], card[draw + 1]);
		draw += 2;

		// ¼¸´Ù Á·º¸¸¦ ÅëÇØ ÇÃ·¹ÀÌ¾î ·¹º§ ±¸ºÐÇÏ±â
		player[i].SetLevel(CheckCombination(player[i].GetCard()));
	}

	// È­¸é¿¡ Ä«µå º¸¿©ÁÖ±â
	Render();

	/*
		½ÂÆÐ °è»êºÎºÐ°ú ±Ý¾× °è»êºÎºÐ (¹èÆÃ¾×¿¡ µû¶ó ´Þ¶óÁö°Ô)
		¼öÁ¤ÇÏ¸é ÁÁÀ»µí 
	*/

	int win = INT_MAX;
	int winner;
	// ½ÂÆÐ °è»ê , ·¹º§ÀÌ Á¦ÀÏ ³·Àº ÇÃ·¹ÀÌ¾î°¡ ½ÂÀÚ
	for (int i = 0; i < playerNumber; i++) {
		if (player[i].GetLevel() < win) {
			win = player[i].GetLevel();
			winner = i;
		}
	}

	cout << "½ÂÀÚ´Â " << winner + 1 << "¹øÂ° ÇÃ·¹ÀÌ¾î ÀÔ´Ï´Ù. ";
	cout << "(" << ShowLevelToCard(player[winner].GetLevel()) << " ½Â¸® ";
	cout << "+" << player[winner].GetMoney() << "¿ø)" << endl;
	// ½ÂÀÚ¿¡°Ô ±Ý¾× ¸ðµÎ ¸ô¾ÆÁÖ±â
	for (int i = 0; i < playerNumber; i++) {
		if (winner != i) {
			int loserMoney = player[i].GetMoney();
			int winnerMoney = player[winner].GetMoney();
			player[winner].SetMoney(winnerMoney + loserMoney);
			player[i].SetMoney(0);
			
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
		cout << "ÇÃ·¹ÀÌ¾î " << i + 1 << " ÀÇ Ä«µå : ";
		cout << player[i].GetCard()[0] % 10 + 1 << " " << player[i].GetCard()[1] % 10 + 1 << " ";
		cout << "(" << ShowLevelToCard(player[i].GetLevel()) << ")" << endl;
	}

	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << endl;

}
