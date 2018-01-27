#pragma once
#include "Player.h"
#include <string>

using namespace std;

class Ssudda
{
private:
	int card[20];
	int playerNumber;
	int tableMoney;			// 해당 판에 배팅된 총 금액
	Player player[4];

	enum combo { 
		GwangTtaeng38, GwangTtaeng, 
		ZangTtaeng, Ttaeng9, Ttaeng8, Ttaeng7, Ttaeng6,
		Ttaeng5, Ttaeng4, Ttaeng3, Ttaeng2, PingTtaeng,
		Ali, Dogsa, GuPing, ZangPing, ZangSa, SeYug, 
		Gabo, Keut8, Keut7, Keut6, Keut5, Keut4, Keut3, Keut2, Keut1,
		MangTong};

#define STARTMONEY 5000

public:
	Ssudda();
	~Ssudda();

	void Shuffle();
	// 두 카드를 섯다 족보를 통해 레벨로 바꿔주는 함수
	int CheckCombination(int* card);	
	string ShowLevelToCard(int level);
	
	void SelectPlayer();	// 플레이어 수 선택하는 함수

	void Init();
	void Release();
	void Update();
	void Render();
};

